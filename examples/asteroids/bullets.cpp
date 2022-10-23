#include "bullets.hpp"

#include <glm/gtx/rotate_vector.hpp>

void Bullets::create(GLuint program) {
  destroy();

  m_program = program;

  // Get location of uniforms in the program
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  m_bullets.clear();

  // Create geometry data
  auto const sides{10};
  std::vector<glm::vec2> positions{{0, 0}};
  auto const step{M_PI * 2 / sides};
  for (auto const angle : iter::range(0.0, M_PI * 2, step)) {
    positions.emplace_back(std::cos(angle), std::sin(angle));
  }
  positions.push_back(positions.at(1));

  // Generate VBO of positions
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Bullets::paint() {
  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_VAO);
  abcg::glUniform4f(m_colorLoc, 1, 1, 1, 1);
  abcg::glUniform1f(m_rotationLoc, 0);
  abcg::glUniform1f(m_scaleLoc, m_scale);

  for (auto const &bullet : m_bullets) {
    abcg::glUniform2f(m_translationLoc, bullet.m_translation.x,
                      bullet.m_translation.y);

    abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 12);
  }

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Bullets::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Bullets::update(Ship &ship, const GameData &gameData, float deltaTime) {
  // Create a pair of bullets
  if (gameData.m_input[gsl::narrow<size_t>(Input::Fire)] &&
      gameData.m_state == State::Playing) {
    // At least 250 ms must have passed since the last bullets
    if (ship.m_bulletCoolDownTimer.elapsed() > 250.0 / 1000.0) {
      ship.m_bulletCoolDownTimer.restart();

      // Bullets are shot in the direction of the ship's forward vector
      auto const forward{glm::rotate(glm::vec2{0.0f, 1.0f}, ship.m_rotation)};
      auto const right{glm::rotate(glm::vec2{1.0f, 0.0f}, ship.m_rotation)};
      auto const cannonOffset{(11.0f / 15.5f) * ship.m_scale};
      auto const bulletSpeed{2.0f};

      Bullet bullet{.m_dead = false,
                    .m_translation = ship.m_translation + right * cannonOffset,
                    .m_velocity = ship.m_velocity + forward * bulletSpeed};
      m_bullets.push_back(bullet);

      bullet.m_translation = ship.m_translation - right * cannonOffset;
      m_bullets.push_back(bullet);

      // Moves ship in the opposite direction
      ship.m_velocity -= forward * 0.1f;
    }
  }

  for (auto &bullet : m_bullets) {
    bullet.m_translation -= ship.m_velocity * deltaTime;
    bullet.m_translation += bullet.m_velocity * deltaTime;

    // Kill bullet if it goes off screen
    if (bullet.m_translation.x < -1.1f)
      bullet.m_dead = true;
    if (bullet.m_translation.x > +1.1f)
      bullet.m_dead = true;
    if (bullet.m_translation.y < -1.1f)
      bullet.m_dead = true;
    if (bullet.m_translation.y > +1.1f)
      bullet.m_dead = true;
  }

  // Remove dead bullets
  m_bullets.remove_if([](auto const &p) { return p.m_dead; });
}