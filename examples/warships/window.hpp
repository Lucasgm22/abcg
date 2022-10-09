#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "abcgOpenGL.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaintUI() override;

private:
  static int const m_N{6}; // Board size is m_N x m_N
  static int const m_boardSize{m_N * m_N};

  enum class GameState { Play, Win, PacificWin };
  GameState m_gameState;

  enum class GameMode { Easy, Normal };
  GameMode m_gameMode;

  enum class GameLastPlayCondition { START, WATER, HIT, DESTROY };
  GameLastPlayCondition m_gameLastPlayCondition;

  int m_percentageOfShips{25};
  int m_occupiedSpaces{};
  int m_hits{};
  int m_waters{};

  
  std::array<char, m_boardSize> m_board{}; // ' ', 'X', 'H', 'W'
  std::default_random_engine m_randomEngine;

  ImFont *m_font{};

  void checkEndCondition();
  void checkDestroyed(int offset);
  void restartGame();
};

#endif