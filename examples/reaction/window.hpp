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
  abcg::Timer m_timer;

  enum class GameState { Play, Stop, FirstRound };
  GameState m_gameState;


  int m_hits{};
  int m_miss{};

  double m_reaction{};
  double m_totalTime{};
  double m_minTime{};
  double m_maxTime{};
  int m_numberOfTargets{1};
  int m_currentNumberOfTargets{0};
  bool m_hasPlayed{false};

  
  std::array<char, m_boardSize> m_board{}; // ' ', 'X', 'H', 'W'
  std::default_random_engine m_randomEngine;

  ImFont *m_font{};

  void putTargets();
};

#endif