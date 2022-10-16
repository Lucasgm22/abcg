#include "window.hpp"

void Window::onCreate() {
  auto const *vertexShader{R"gl(#version 300 es
                                   
    layout(location = 0) in vec2 inPosition;
    layout(location = 1) in vec4 inColor;

    out vec4 fragColor;

    void main() { 
      gl_Position = vec4(inPosition, 0, 1);
      fragColor = inColor;
    }
  )gl"};

  auto const *fragmentShader{R"gl(#version 300 es
                                     
    precision mediump float;

    in vec4 fragColor;

    out vec4 outColor;

    void main() { outColor = fragColor; }
  )gl"};

  m_program = abcg::createOpenGLProgram(
      {{.source = vertexShader, .stage = abcg::ShaderStage::Vertex},
       {.source = fragmentShader, .stage = abcg::ShaderStage::Fragment}});

  abcg::glClearColor(0, 0, 0, 1);
  abcg::glClear(GL_COLOR_BUFFER_BIT);

  abcg::glEnable(GL_BLEND);
  abcg::glBlendEquation(GL_FUNC_ADD);
  abcg::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

  auto const seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);
}

void Window::onPaint() {
  // Check whether to render the next triangle
  if ((m_timer.elapsed() * 1000) < m_delay)
    return;
  m_timer.restart();

  setupModel();

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);
  abcg::glBindVertexArray(m_VAO);

  abcg::glDrawArrays(GL_TRIANGLES, 0, 3);

  abcg::glBindVertexArray(0);
  abcg::glUseProgram(0);
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  {
    auto const widgetSize{ImVec2(340, 280)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportSize.x - widgetSize.x - 5,
                                   m_viewportSize.y - widgetSize.y - 5));
    ImGui::SetNextWindowSize(widgetSize);
    auto windowFlags{ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar};
    ImGui::Begin(" ", nullptr, windowFlags);
    //checkbox
    ImGui::Checkbox("Solid Color", &m_solidColor);
    ImGui::Checkbox("Random Colors", &m_randomColors);

    // Edit vertex colors
    auto colorEditFlags{ImGuiColorEditFlags_NoTooltip |
                        ImGuiColorEditFlags_NoPicker};
    ImGui::PushItemWidth(215);
    ImGui::ColorEdit3("v0", &m_colors.at(0).x, colorEditFlags);
    ImGui::SliderFloat("alpha0", &m_colors.at(0)[3], 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_NoInput);

    if (!m_solidColor) {
      ImGui::ColorEdit3("v1", &m_colors.at(1).x, colorEditFlags);
      ImGui::SliderFloat("alpha1", &m_colors.at(1)[3], 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_NoInput);

      ImGui::ColorEdit3("v2", &m_colors.at(2).x, colorEditFlags);
      ImGui::SliderFloat("alpha2", &m_colors.at(2)[3], 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_NoInput);
    }

    //Slider delay
    ImGui::SliderInt("delay", &m_delay, 0, 500, "%d ms", ImGuiSliderFlags_NoInput);
    

    //Combo box to alpha function
    static std::size_t currentIndex{};
    std::vector comboItems{"GL_FUNC_ADD", "GL_FUNC_SUBTRACT", "GL_FUNC_REVERSE_SUBTRACT", "GL_MIN", "GL_MAX"};

    if (ImGui::BeginCombo("Alpha Function", comboItems.at(currentIndex))) {
      for (auto index{0U}; index < comboItems.size(); ++index) {
        bool const isSelected{currentIndex == index};
        if (ImGui::Selectable(comboItems.at(index), isSelected)) {
            currentIndex = index;
            switch (currentIndex)
            {
              case 0:
                abcg::glBlendEquation(GL_FUNC_ADD);
                break;
              case 1:
                abcg::glBlendEquation(GL_FUNC_SUBTRACT);
                break;
              case 2:
                abcg::glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
                break;
              case 3:
                abcg::glBlendEquation(GL_MIN);
                break;
              case 4:
                abcg::glBlendEquation(GL_MAX);
                break;
              }
        }

        // Set the initial focus when opening the combo (scrolling + keyboard
        // navigation focus)
        if (isSelected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }


    ImGui::PopItemWidth();

    ImGui::End();
  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_VBOPositions);
  abcg::glDeleteBuffers(1, &m_VBOColors);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Window::setupModel() {
  abcg::glDeleteBuffers(1, &m_VBOPositions);
  abcg::glDeleteBuffers(1, &m_VBOColors);
  abcg::glDeleteVertexArrays(1, &m_VAO);

  // Create array of random vertex positions
  std::uniform_real_distribution rd(-1.5f, 1.5f);
  std::array<glm::vec2, 3> const positions{
      {{rd(m_randomEngine), rd(m_randomEngine)},
       {rd(m_randomEngine), rd(m_randomEngine)},
       {rd(m_randomEngine), rd(m_randomEngine)}}};

  std::array<glm::vec4, 3> colors;
       
  if (m_randomColors) {
    std::uniform_real_distribution rd_c(0.0f, 1.0f);

    std::array<glm::vec4, 3> random_colors{
        {
          {rd_c(m_randomEngine), rd_c(m_randomEngine), rd_c(m_randomEngine), m_colors.at(0)[3]},
          {rd_c(m_randomEngine), rd_c(m_randomEngine), rd_c(m_randomEngine), m_colors.at(1)[3]},
          {rd_c(m_randomEngine), rd_c(m_randomEngine), rd_c(m_randomEngine), m_colors.at(2)[3]}
        }
      };
    m_colors = random_colors;
  }


  colors = m_colors;

  if (m_solidColor) {
    colors.at(1) = colors.at(0);
    colors.at(2) = colors.at(0);
  }

  // Generate VBO of positions
  abcg::glGenBuffers(1, &m_VBOPositions);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOPositions);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate VBO of colors
  abcg::glGenBuffers(1, &m_VBOColors);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), colors.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};
  auto const colorAttribute{abcg::glGetAttribLocation(m_program, "inColor")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOPositions);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glEnableVertexAttribArray(colorAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
  abcg::glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}