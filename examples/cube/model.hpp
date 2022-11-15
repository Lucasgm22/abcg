#ifndef MODEL_HPP_
#define MODEL_HPP_

#include "abcgOpenGL.hpp"

struct Vertex {
  glm::vec3 position{};

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Model {
public:
  void loadObj(std::string_view path);
  void render();
  void setupVAO(GLuint program, GLint modelMatrixLoc, GLint colorLoc, float scale);
  void destroy() const;
  bool m_canMove{true};
  float m_angle{};
  void moveLeft();
  void translateLeft();
  void moveRigth();
  void translateRight();
  void moveUp();
  void translateUp();
  void moveDown();
  void translateDown();
  void increaseAngle(float inc);
  void resetAnimation();



  [[nodiscard]] int getNumTriangles() const {
    return gsl::narrow<int>(m_indices.size()) / 3;
  }

private:
  glm::vec3 m_position{};
  float m_scale{1.0f};


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
  void move(int orientation);

};

#endif