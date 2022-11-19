#ifndef GROUND_HPP_
#define GROUND_HPP_

#include "abcgOpenGL.hpp"

class Ground {
public:
  void create(GLuint program, GLint modelMatrixLoc, GLint colorLoc, float scale, int N);
  void paint();
  void destroy();

private:
  float m_scale;
  int m_N{5}; //Configure number of tiles on the grid of 2N+1 x 2N+1 tiles
  GLuint m_VAO{};
  GLuint m_VBO{};

  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};
};

#endif