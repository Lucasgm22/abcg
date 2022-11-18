#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"
#include "cube.hpp"
#include "ground.hpp"


class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onUpdate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;
  float getDeltaTimef();

private:

  glm::ivec2 m_viewportSize{};
  float m_scale{0.2f};

  GLint m_modelMatrixLoc{};
  glm::mat4 m_viewMatrix{1.0f};
  GLint m_viewMatrixLoc{};
  glm::mat4 m_projMatrix{1.0f};
  GLint m_projMatrixLoc{};
  GLint m_colorLoc{};

  Ground m_ground;
  Cube m_cube;
  GLuint m_program{};
};

#endif