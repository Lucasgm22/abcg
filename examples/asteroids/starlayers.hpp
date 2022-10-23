#ifndef STARLAYERS_HPP_
#define STARLAYERS_HPP_

#include <array>
#include <random>

#include "abcgOpenGL.hpp"

#include "gamedata.hpp"
#include "ship.hpp"

class StarLayers {
public:
  void create(GLuint program, int quantity);
  void paint();
  void destroy();
  void update(const Ship &ship, float deltaTime);

private:
  GLuint m_program{};
  GLint m_pointSizeLoc{};
  GLint m_translationLoc{};

  struct StarLayer {
    GLuint m_VAO{};
    GLuint m_VBO{};

    float m_pointSize{};
    int m_quantity{};
    glm::vec2 m_translation{};
  };

  std::array<StarLayer, 5> m_starLayers;

  std::default_random_engine m_randomEngine;
};

#endif