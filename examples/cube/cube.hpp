#ifndef CUBE_HPP_
#define CUBE_HPP_

#include "abcgOpenGL.hpp"
#include "vertex.hpp"

class Cube {
public:
  void loadObj(std::string_view path);
  void paint();
  void update(float deltaTime);
  void create(GLuint program, GLint modelMatrixLoc, GLint colorLoc, GLint normalMatrixLoc, glm::mat4 viewMatrix, float scale, int N);
  void destroy() const;
  void moveLeft();
  void moveRigth();
  void moveUp();
  void moveDown();

private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};


  glm::mat4 m_animationMatrix{1.0f};
  glm::mat4 m_viewMatrix;
  glm::mat4 m_positionMatrix{1.0f};
  glm::mat4 m_modelMatrix{1.0f};
  GLint m_normalMatrixLoc;
  GLint m_modelMatrixLoc;

  //Material Light properties
  GLint m_KaLoc;
  GLint m_KdLoc;
  GLint m_KsLoc;

  float m_Ka{0.7};
  float m_Kd{0.3};
  float m_Ks{0.1};


  GLint m_colorLoc;

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  void createBuffers();

  enum class Orientation{ DOWN, RIGHT, UP, LEFT};

  glm::vec3 m_position{};
  float m_scale{1.0f};
  float m_angle{};
  Orientation m_orientation{Orientation::DOWN};
  bool m_isMoving{false};
  float m_maxPos{1.0f};
  float m_angleVelocity{360.0f}; //animation to move takes 1/4 of a second

  void move(float deltaTime);
  void translate();
  void resetAnimation();
  void increaseAngle(float inc);

  bool m_hasNormals{false};

  void computeNormals();

};

#endif