#include "window.hpp"

void Window::onEvent(SDL_Event const &event) {

  if (event.type == SDL_KEYDOWN && !m_cube.isMoving()) {
 
    if (event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_UP) {
      m_cube.moveUp(getDeltaTimef());
    }
    if (event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_DOWN) {
      m_cube.moveDown(getDeltaTimef());
    }
    if (event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_LEFT) {
      m_cube.moveLeft(getDeltaTimef());
    }
    if (event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT) {
      m_cube.moveRigth(getDeltaTimef());
    }
  }
}

void Window::onCreate() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0, 0, 0, 1);
  abcg::glEnable(GL_DEPTH_TEST);

  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "light.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "light.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  m_viewMatrixLoc = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLoc = abcg::glGetUniformLocation(m_program, "projMatrix");
  m_modelMatrixLoc = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");

  m_ground.create(m_program, m_modelMatrixLoc, m_colorLoc, m_scale);
  m_cube.loadObj(assetsPath + "box.obj");
  m_cube.setupVAO(m_program, m_modelMatrixLoc, m_colorLoc, m_scale);

  m_viewMatrix =glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f),
                            glm::vec3(0.0f, 0.0f, 0.0f), 
                            glm::vec3(0.0f, 1.0f, 0.0f));
}

void Window::onUpdate() {
  m_cube.update(getDeltaTimef());
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  // Set uniform variables that have the same value for every model
  abcg::glUniformMatrix4fv(m_viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);

  m_cube.paint();
  m_ground.paint();


  abcg::glUseProgram(0);
}

void Window::onPaintUI() {
  {
    auto const widgetSize{ImVec2(222, 40)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportSize.x - widgetSize.x - 5, 5));
    ImGui::SetNextWindowSize(widgetSize);
    ImGui::Begin("Widget window", nullptr, ImGuiWindowFlags_NoDecoration);

    // Projection combo box
    {
      static std::size_t currentIndex{};
      std::vector<std::string> comboItems{"Perspective", "Orthographic"};

      ImGui::PushItemWidth(120);
      if (ImGui::BeginCombo("Projection",
                            comboItems.at(currentIndex).c_str())) {
        for (auto const index : iter::range(comboItems.size())) {
          auto const isSelected{currentIndex == index};
          if (ImGui::Selectable(comboItems.at(index).c_str(), isSelected))
            currentIndex = index;
          if (isSelected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();

      if (currentIndex == 0) {
        auto const aspect{gsl::narrow<float>(m_viewportSize.x) /
                          gsl::narrow<float>(m_viewportSize.y)};
        m_projMatrix =
            glm::perspective(glm::radians(45.0f), aspect, 0.1f, 5.0f);
      } else {
        m_projMatrix = glm::ortho(-1.5f, 1.5f, -1.5f, 1.5f, 0.1f, 5.0f);
      }
    }

    ImGui::End();
  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
}

void Window::onDestroy() {
  m_ground.destroy();
  m_cube.destroy();
  abcg::glDeleteProgram(m_program);
}

float Window::getDeltaTimef() {
  return gsl::narrow_cast<float>(getDeltaTime());
}