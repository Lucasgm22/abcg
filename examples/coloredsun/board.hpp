#ifndef BOARD_HPP_
#define BOARD_HPP_

#include "abcgOpenGL.hpp"

#include <random>


class Board {
public:
  void create(GLuint program);
  void paint();
  void destroy();
  void resize(glm::vec2 size);
  void randomColor1();
  void randomColor2();
  void randomColors();

  glm::vec2 m_mouse{};
  glm::vec3 m_color1{1,0,0};
  glm::vec3 m_color2{0,0,0};
  glm::vec2 m_resolution{};


private:
  GLuint m_program{};

  GLint m_resolutionLoc{};
  GLint m_mouseLoc{};
  GLint m_color1Loc{};
  GLint m_color2Loc{};

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  std::default_random_engine m_randomEngine;


  glm::vec3 randomColor();
};
#endif