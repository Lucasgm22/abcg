#include "asteroids.hpp"

#include <glm/gtx/fast_trigonometry.hpp>

void Asteroids::create(GLuint program, int quantity) {
  destroy();

  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  m_program = program;

  // Get location of uniforms in the program
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Create asteroids
  m_asteroids.clear();
  m_asteroids.resize(quantity);

  for (auto &asteroid : m_asteroids) {
    asteroid = makeAsteroid();

    // Make sure the asteroid won't collide with the ship
    do {
      asteroid.m_translation = {m_randomDist(m_randomEngine),
                                m_randomDist(m_randomEngine)};
    } while (glm::length(asteroid.m_translation) < 0.5f);
  }
}

void Asteroids::paint() {
  abcg::glUseProgram(m_program);

  for (auto const &asteroid : m_asteroids) {
    abcg::glBindVertexArray(asteroid.m_VAO);

    abcg::glUniform4fv(m_colorLoc, 1, &asteroid.m_color.r);
    abcg::glUniform1f(m_scaleLoc, asteroid.m_scale);
    abcg::glUniform1f(m_rotationLoc, asteroid.m_rotation);

    for (auto i : {-2, 0, 2}) {
      for (auto j : {-2, 0, 2}) {
        abcg::glUniform2f(m_translationLoc, asteroid.m_translation.x + j,
                          asteroid.m_translation.y + i);

        abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, asteroid.m_polygonSides + 2);
      }
    }

    abcg::glBindVertexArray(0);
  }

  abcg::glUseProgram(0);
}

void Asteroids::destroy() {
  for (auto &asteroid : m_asteroids) {
    abcg::glDeleteBuffers(1, &asteroid.m_VBO);
    abcg::glDeleteVertexArrays(1, &asteroid.m_VAO);
  }
}

void Asteroids::update(const Ship &ship, float deltaTime) {
  for (auto &asteroid : m_asteroids) {
    asteroid.m_translation -= ship.m_velocity * deltaTime;
    asteroid.m_rotation = glm::wrapAngle(
        asteroid.m_rotation + asteroid.m_angularVelocity * deltaTime);
    asteroid.m_translation += asteroid.m_velocity * deltaTime;

    // Wrap-around
    if (asteroid.m_translation.x < -1.0f)
      asteroid.m_translation.x += 2.0f;
    if (asteroid.m_translation.x > +1.0f)
      asteroid.m_translation.x -= 2.0f;
    if (asteroid.m_translation.y < -1.0f)
      asteroid.m_translation.y += 2.0f;
    if (asteroid.m_translation.y > +1.0f)
      asteroid.m_translation.y -= 2.0f;
  }
}

Asteroids::Asteroid Asteroids::makeAsteroid(glm::vec2 translation,
                                            float scale) {
  Asteroid asteroid;

  auto &re{m_randomEngine}; // Shortcut

  // Randomly pick the number of sides
  std::uniform_int_distribution randomSides(6, 20);
  asteroid.m_polygonSides = randomSides(re);

  // Get a random color (actually, a grayscale)
  std::uniform_real_distribution randomIntensity(0.5f, 1.0f);
  asteroid.m_color = glm::vec4(randomIntensity(re));

  asteroid.m_color.a = 1.0f;
  asteroid.m_rotation = 0.0f;
  asteroid.m_scale = scale;
  asteroid.m_translation = translation;

  // Get a random angular velocity
  asteroid.m_angularVelocity = m_randomDist(re);

  // Get a random direction
  glm::vec2 const direction{m_randomDist(re), m_randomDist(re)};
  asteroid.m_velocity = glm::normalize(direction) / 7.0f;

  // Create geometry data
  std::vector<glm::vec2> positions{{0, 0}};
  auto const step{M_PI * 2 / asteroid.m_polygonSides};
  std::uniform_real_distribution randomRadius(0.8f, 1.0f);
  for (auto const angle : iter::range(0.0, M_PI * 2, step)) {
    auto const radius{randomRadius(re)};
    positions.emplace_back(radius * std::cos(angle), radius * std::sin(angle));
  }
  positions.push_back(positions.at(1));

  // Generate VBO
  abcg::glGenBuffers(1, &asteroid.m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, asteroid.m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &asteroid.m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(asteroid.m_VAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, asteroid.m_VBO);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  return asteroid;
}