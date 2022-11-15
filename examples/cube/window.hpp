#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"
#include "model.hpp"
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

private:

  enum class Input{ DOWN, RIGHT, UP, LEFT};
  Input m_input{Input::DOWN};
  
  abcg::Timer m_timer;


  glm::ivec2 m_viewportSize{};
  float m_scale{0.2f};
  Model m_model;


  GLint m_modelMatrixLoc{};
  glm::mat4 m_viewMatrix{1.0f};
  GLint m_viewMatrixLoc{};
  glm::mat4 m_projMatrix{1.0f};
  GLint m_projMatrixLoc{};
  GLint m_colorLoc{};

  Ground m_ground;
  GLuint m_program{};
};

#endif