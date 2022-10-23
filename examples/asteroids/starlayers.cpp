#include "starlayers.hpp"

void StarLayers::create(GLuint program, int quantity) {
  destroy();

  // Initialize pseudorandom number generator and distributions
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());
  std::uniform_real_distribution distPos(-1.0f, 1.0f);
  std::uniform_real_distribution distIntensity(0.5f, 1.0f);
  auto &re{m_randomEngine}; // Shortcut

  m_program = program;

  // Get location of uniforms in the program
  m_pointSizeLoc = abcg::glGetUniformLocation(m_program, "pointSize");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};
  auto const colorAttribute{abcg::glGetAttribLocation(m_program, "inColor")};

  for (auto &&[index, layer] : iter::enumerate(m_starLayers)) {
    // Create data for the stars of this layer
    layer.m_pointSize = 10.0f / (1.0f + index);
    layer.m_quantity = quantity * (gsl::narrow<int>(index) + 1);
    layer.m_translation = {};

    std::vector<glm::vec3> data;
    for ([[maybe_unused]] auto _ : iter::range(0, layer.m_quantity)) {
      data.emplace_back(distPos(re), distPos(re), 0);
      data.push_back(glm::vec3(distIntensity(re)));
    }

    // Generate VBO
    abcg::glGenBuffers(1, &layer.m_VBO);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, layer.m_VBO);
    abcg::glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec3),
                       data.data(), GL_STATIC_DRAW);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create VAO
    abcg::glGenVertexArrays(1, &layer.m_VAO);

    // Bind vertex attributes to current VAO
    abcg::glBindVertexArray(layer.m_VAO);

    abcg::glBindBuffer(GL_ARRAY_BUFFER, layer.m_VBO);
    abcg::glEnableVertexAttribArray(positionAttribute);
    abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE,
                                sizeof(glm::vec3) * 2, nullptr);
    abcg::glEnableVertexAttribArray(colorAttribute);
    abcg::glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE,
                                sizeof(glm::vec3) * 2,
                                reinterpret_cast<void *>(sizeof(glm::vec3)));
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    // End of binding to current VAO
    abcg::glBindVertexArray(0);
  }
}

void StarLayers::paint() {
  abcg::glUseProgram(m_program);

  abcg::glEnable(GL_BLEND);
  abcg::glBlendFunc(GL_ONE, GL_ONE);

  for (auto const &layer : m_starLayers) {
    abcg::glBindVertexArray(layer.m_VAO);
    abcg::glUniform1f(m_pointSizeLoc, layer.m_pointSize);

    for (auto const i : {-2, 0, 2}) {
      for (auto const j : {-2, 0, 2}) {
        abcg::glUniform2f(m_translationLoc, layer.m_translation.x + j,
                          layer.m_translation.y + i);

        abcg::glDrawArrays(GL_POINTS, 0, layer.m_quantity);
      }
    }

    abcg::glBindVertexArray(0);
  }

  abcg::glDisable(GL_BLEND);

  abcg::glUseProgram(0);
}

void StarLayers::destroy() {
  for (auto &layer : m_starLayers) {
    abcg::glDeleteBuffers(1, &layer.m_VBO);
    abcg::glDeleteVertexArrays(1, &layer.m_VAO);
  }
}

void StarLayers::update(const Ship &ship, float deltaTime) {
  for (auto &&[index, layer] : iter::enumerate(m_starLayers)) {
    auto const layerSpeedScale{1.0f / (index + 2.0f)};
    layer.m_translation -= ship.m_velocity * deltaTime * layerSpeedScale;

    // Wrap-around
    if (layer.m_translation.x < -1.0f)
      layer.m_translation.x += 2.0f;
    if (layer.m_translation.x > +1.0f)
      layer.m_translation.x -= 2.0f;
    if (layer.m_translation.y < -1.0f)
      layer.m_translation.y += 2.0f;
    if (layer.m_translation.y > +1.0f)
      layer.m_translation.y -= 2.0f;
  }
}