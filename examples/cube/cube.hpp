#ifndef CUBE_HPP_
#define CUBE_HPP_

#include "abcgOpenGL.hpp"

struct Vertex {
  glm::vec3 position{};

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Cube {
public:
  void loadObj(std::string_view path);
  void paint();
  void update(float deltaTime);
  void create(GLuint program, GLint modelMatrixLoc, GLint colorLoc, float scale, int N);
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
  glm::mat4 m_positionMatrix{1.0f};
  glm::mat4 m_modelMatrix{1.0f};
  GLint m_modelMatrixLoc;

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
  float m_angleVelocity{360.0f};

  void move(float deltaTime);
  void translate();
  void resetAnimation();
  void increaseAngle(float inc);

};

#endif