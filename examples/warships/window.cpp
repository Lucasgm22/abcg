#include "window.hpp"

void Window::onCreate() {
  // Load font with bigger size for the X's and O's
  auto const filename{abcg::Application::getAssetsPath() +
                      "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 72.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError{"Cannot load font file"};
  }
  auto const seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);

  restartGame();
}

void Window::restartGame() {
  m_board.fill(' ');
  m_occupiedSpaces = 0;
  m_hits = 0;
  m_waters = 0;
  std::uniform_int_distribution<int> intDistribution(0, 100);
  for (auto i : iter::range(m_N)) {
    for (auto j : iter::range(m_N)) {
      if (intDistribution(m_randomEngine) < m_percentageOfShips) {
        auto const offset{i * m_N + j};
        m_board.at(offset) = 'X';
        m_occupiedSpaces = m_occupiedSpaces + 1;
      }
    }
  }
  m_gameState = GameState::Play;
  m_gameLastPlayCondition = GameLastPlayCondition::START;
}

void Window::onPaintUI() {
  // Get size of application's window
  auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};
  auto const appWindowHeight{gsl::narrow<float>(getWindowSettings().height)};

  // "Tic-Tac-Toe" window
  {
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, appWindowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    auto const flags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse};
    ImGui::Begin("Warships", nullptr, flags);


    // Static text showing current turn or win/draw messages
    {
      std::string text;
      switch (m_gameState) {
      case GameState::Play:
        switch (m_gameLastPlayCondition)
        {
        case GameLastPlayCondition::START:
          text = "Begin the War!";
          break;
        case GameLastPlayCondition::WATER:
          text = "You missed the enemy!";
          break;
        case GameLastPlayCondition::HIT:
          text = "You HIT a enemy ship!";
          break;
        case GameLastPlayCondition::DESTROY:
          text = "You destroyed a enemy ship!";
          break;        
        default:
          break;
        }
        break;
      case GameState::Win:
        text = "WIN! You killed all your enemies!";
        break;
      case GameState::PacificWin:
        text = "WIN! You scared them all with you arsenal!";
        break;
      }
      // Center text
      ImGui::SetCursorPosX(
          (appWindowWidth - ImGui::CalcTextSize(text.c_str()).x)/ 3);
      ImGui::Text("%s Hits: %d | Left: %d", text.c_str(), m_hits, m_occupiedSpaces - m_hits);
      ImGui::Spacing();
    }

    ImGui::Spacing();

    // Game board
    {
      auto const gridHeight{appWindowHeight - 22 - 60 - (m_N * 10) - 60};
      auto const buttonHeight{gridHeight / m_N};

      // Use custom font
      ImGui::PushFont(m_font);
      if (ImGui::BeginTable("Game board", m_N)) {
        for (auto i : iter::range(m_N)) {
          ImGui::TableNextRow();
          for (auto j : iter::range(m_N)) {
            ImGui::TableSetColumnIndex(j);
            auto const offset{i * m_N + j};

            // Get current character
            auto ch{m_board.at(offset)};


            // Button text is ch followed by an ID in the format ##ij
            auto buttonText{fmt::format("{}##{}{}", ch, i, j)};
            if (m_gameMode == GameMode::Normal) {
              buttonText = fmt::format("{}##{}{}", (ch != 'X' && ch != 'W') ? ch : ' ' , i, j);
            }
            if (ImGui::Button(buttonText.c_str(), ImVec2(-1, buttonHeight))) {
              if (m_gameState == GameState::Play) {
                switch (ch) {
                case ' ':
                  m_board.at(offset) = 'W';
                  m_waters = m_waters + 1;
                  checkEndCondition();
                  m_gameLastPlayCondition = GameLastPlayCondition::WATER;
                  break;
                case 'X':
                  m_board.at(offset) = 'H';
                  m_hits = m_hits + 1;
                  checkEndCondition();
                  m_gameLastPlayCondition = GameLastPlayCondition::DESTROY;
                  checkDestroyed(i);
                  break;
                default:
                  m_gameLastPlayCondition = GameLastPlayCondition::WATER;
                  break;
                }            
              }
            }
          }
          ImGui::Spacing();
        }
        ImGui::EndTable();
      }
      ImGui::PopFont();
    }

    ImGui::Spacing();

    // "Restart game" button
    {
      auto restartText = (m_gameState == GameState::Play) ? "Restart" : "Win again!";
      if (ImGui::Button(fmt::format("{} - Easy", restartText).c_str(), ImVec2(-1, 25))) {
        m_gameMode = GameMode::Easy;
        restartGame();
      }
      if (ImGui::Button(fmt::format("{} - Normal", restartText).c_str(), ImVec2(-1, 25))) {
        m_gameMode = GameMode::Normal;
        restartGame();
      }      
    }

    ImGui::End();
  }
}

void Window::checkDestroyed(int offset) {
  for (auto i : {offset + 1, offset - 1, offset + m_N, offset - m_N}) {
    fmt::print(stderr, "range: {}, {}, {}, {}\n", offset + 1, offset - 1, offset + m_N, offset - m_N);
    if (0 <= i && i < m_boardSize && i % m_N != 0 && i % m_N != m_N - 1) {
      switch (m_board.at(i))
      {
      case 'X':
        m_gameLastPlayCondition = GameLastPlayCondition::HIT;
        return;
        break;
      case 'H':
        checkDestroyed(i);
        break;
      }
    }              
  }
}

void Window::checkEndCondition() {
  //Hitted all boats
  fmt::print(stderr, "hist: {}, letf: {}, waters: {}, all_water: {}\n", m_hits, m_occupiedSpaces- m_hits, m_waters, m_boardSize - m_occupiedSpaces);
  if (m_hits == m_occupiedSpaces) {
    m_gameState = GameState::Win;
  //Hitted all and only water  
  } else if (m_waters + m_hits == m_boardSize - m_occupiedSpaces) {
    m_gameState = GameState::PacificWin;
  }
}