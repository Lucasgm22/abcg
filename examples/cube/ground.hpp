#ifndef GROUND_HPP_
#define GROUND_HPP_

#include "abcgOpenGL.hpp"
#include "vertex.hpp"

class Ground {
public:
  void create(GLuint program, GLint modelMatrixLoc, GLint colorLoc, GLint normalMatrixLoc, glm::mat4 viewMatrix, float scale, int N);
  void paint();
  void destroy();

private:
  std::vector<Vertex> m_vertices;
  float m_scale;
  int m_N{5}; //Configure number of tiles on the grid of 2N+1 x 2N+1 tiles
  GLuint m_VAO{};
  GLuint m_VBO{};

  GLint m_modelMatrixLoc{};
  GLint m_normalMatrixLoc;
  GLint m_colorLoc{};

  glm::mat4 m_viewMatrix;


  //Material Light properties
  GLint m_KaLoc;
  GLint m_KdLoc;
  GLint m_KsLoc;

  float m_Ka{0.7};
  float m_Kd{0.3};
  float m_Ks{0.1};


};

#endif