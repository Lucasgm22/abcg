#ifndef BULLETS_HPP_
#define BULLETS_HPP_

#include <list>

#include "abcgOpenGL.hpp"

#include "gamedata.hpp"
#include "ship.hpp"

class OpenGLWindow;

class Bullets {
public:
  void create(GLuint program);
  void paint();
  void destroy();
  void update(Ship &ship, const GameData &gameData, float deltaTime);

  struct Bullet {
    bool m_dead{};
    glm::vec2 m_translation{};
    glm::vec2 m_velocity{};
  };

  std::list<Bullet> m_bullets;

  float m_scale{0.015f};

private:
  GLuint m_program{};
  GLint m_colorLoc{};
  GLint m_rotationLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};

  GLuint m_VAO{};
  GLuint m_VBO{};
};

#endif