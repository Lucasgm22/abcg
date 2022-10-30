#include "window.hpp"

void Window::onEvent(SDL_Event const &event) {

  //Key Board events
  if (event.type == SDL_KEYDOWN)
    if (event.key.keysym.sym == SDLK_SPACE)
      m_board.randomColors();

  // Mouse events
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_board.randomColor1();
    if (event.button.button == SDL_BUTTON_RIGHT)
      m_board.randomColor2();
  }
  if (event.type == SDL_MOUSEMOTION) {
    glm::ivec2 mousePosition;
    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
    float x = (mousePosition.x);
    float y = (m_viewportSize.y - mousePosition.y);
  
    glm::vec2 mouse{x,y};

    m_board.m_mouse = mouse;
                        
  }
}

void Window::onCreate() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  // Create program to render the other objects
  m_objectsProgram =
      abcg::createOpenGLProgram({{.source = assetsPath + "objects.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "objects.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  abcg::glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  restart();
}

void Window::restart() {
  m_board.create(m_objectsProgram);
}


void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  m_board.paint();
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_board.resize(m_viewportSize);

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_objectsProgram);

  m_board.destroy();
}