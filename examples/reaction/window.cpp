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
    m_gameState = GameState::Stop;
  }

  void Window::putTargets() {
    //Pics a index to put a target
    std::uniform_int_distribution<int> intDistribution(0, m_boardSize - 1);
    while (m_currentNumberOfTargets < m_numberOfTargets) {
      int randomIndex = intDistribution(m_randomEngine);
      if (m_board.at(randomIndex) != 'X') {
        m_board.at(randomIndex) = 'X';
        m_currentNumberOfTargets = m_currentNumberOfTargets + 1;
      }
    }
    //Restart timer for next reaction
    m_timer.restart();
  }


  void Window::onPaintUI() {
    // Get size of application's window
    auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};
    auto const appWindowHeight{gsl::narrow<float>(getWindowSettings().height)};
    // "Reaction" window
    {
      ImGui::SetNextWindowSize(ImVec2(appWindowWidth, appWindowHeight));
      ImGui::SetNextWindowPos(ImVec2(0, 0));

      auto const flags{ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse};
      ImGui::Begin("Reaction", nullptr, flags);


      // Static text showing play/stope messages
      {
        std::string text;
        switch (m_gameState) {
          case GameState::FirstRound:
            text = fmt::format("Hit the X's!");
            break;
          case GameState::Play:
            text = fmt::format("Last reaction: {}s!", m_reaction);
            break;
          case GameState::Stop:
            text = "Stoped!";
            break;
        }
        // Center text
        ImGui::SetCursorPosX(
            (appWindowWidth - ImGui::CalcTextSize(text.c_str()).x)/ 2);
        ImGui::Text(text.c_str());
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
      
              if (ImGui::Button(buttonText.c_str(), ImVec2(-1, buttonHeight))) {
                if (m_gameState != GameState::Stop) {
                  // Hitted the target
                  if (ch == 'X') {
                    m_board.at(offset) = ' ';
                    m_currentNumberOfTargets = m_currentNumberOfTargets - 1;
                    m_hits = m_hits + 1;
                    //Gets the time elapsed to react
                    m_reaction = m_timer.elapsed();
                    m_totalTime = m_reaction + m_totalTime;
                    if (m_gameState == GameState::FirstRound) {
                      m_minTime = m_reaction;
                      m_gameState = GameState::Play;
                    }
                    if (m_reaction > m_maxTime) m_maxTime = m_reaction;
                    if (m_reaction < m_minTime) m_minTime = m_reaction;
                    //Put a new target
                    putTargets();
                  } else {
                    m_miss = m_miss + 1;
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
            
      // "Stop game" button 
      if (m_gameState != GameState::Stop) {
        {
          if (ImGui::Button("Stop", ImVec2(-1, 25))) {
            m_gameState = GameState::Stop;
            m_hasPlayed = true;
          }
        }
      }

      ImGui::End();
    }

  // Stats and config window
  if (m_gameState == GameState::Stop) {
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, appWindowHeight / 2));
    ImGui::SetNextWindowPos(ImVec2(1, 0));
    ImGui::Begin("Stats and Config", nullptr);

    //Statis
    if (ImGui::CollapsingHeader("Stats from last play")) {
      {
        if (m_hasPlayed) {
          ImGui::Text(fmt::format("Min time: {}s", m_minTime).c_str());
          ImGui::Text(fmt::format("Max time: {}s", m_maxTime).c_str());
          ImGui::Text(fmt::format("Avarage: {}s", (m_hits != 0) ? (m_totalTime / m_hits) : 0).c_str());
          ImGui::Text(fmt::format("Hist: {}", m_hits).c_str());
          ImGui::Text(fmt::format("Misses: {}", m_miss).c_str());
        } else {
          ImGui::Text("Play to see your reaction Stats!");
        }
      }
    }
    // Config
    if (ImGui::CollapsingHeader("Config")) {
      {
        ImGui::Text("Config");
        ImGui::SliderInt("Max number of targets", &m_numberOfTargets, 1, 36, "%d", ImGuiSliderFlags_NoInput);
      }
    }
        
    // Play button
    if (ImGui::Button("Play", ImVec2(-1, 25))) {
      m_gameState = GameState::FirstRound;
      m_reaction = 0;
      m_maxTime = 0;
      m_minTime = 0;
      m_totalTime = 0;
      m_hits = 0;
      m_miss = 0;
      m_currentNumberOfTargets = 0;
      m_board.fill(' ');
      putTargets(); 
    }

    ImGui::End();
  }
}
