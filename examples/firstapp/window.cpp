#include "window.hpp"

void Window::onCreate() {
  auto const &windowSettings{getWindowSettings()};
  fmt::print("Initial window size: {}x{}\n", windowSettings.width,
             windowSettings.height);
}

void Window::onPaint() {
  // Set the clear color
  abcg::glClearColor(m_clearColor.at(0), m_clearColor.at(1), m_clearColor.at(2),
                     m_clearColor.at(3));
  // Clear the color buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onPaintUI() {
  // Parent class will show fullscreen button and FPS meter
  abcg::OpenGLWindow::onPaintUI();

  // Our own ImGui widgets go below
  {
    // Window begin
    ImGui::Begin("Hello, First App!");

    // Static text
    auto const &windowSettings{getWindowSettings()};
    ImGui::Text("Current window size: %dx%d (in windowed mode)",
                windowSettings.width, windowSettings.height);

    // Slider from 0.0f to 1.0f
    static float f{};
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

    // ColorEdit to change the clear color
    ImGui::ColorEdit3("clear color", m_clearColor.data());

    // More static text
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    // Window end
    ImGui::End();
  }

  // 100x50 button
  if (ImGui::Button("Press me!", ImVec2(100, 50))) {
    fmt::print("1st button pressed.\n");
  }

  // Nx50 button, where N is the remaining width available
  ImGui::Button("Press me!", ImVec2(-1, 50));
  // See also IsItemHovered, IsItemActive, etc
  if (ImGui::IsItemClicked()) {
    fmt::print("2nd Button pressed.\n");
  } 


  static bool enabled{true};
  ImGui::Checkbox("Some option", &enabled);
  fmt::print("The checkbox is {}\n", enabled ? "enabled" : "disabled");

  static std::size_t currentIndex{};
  std::vector comboItems{"AAA", "BBB", "CCC"};

  if (ImGui::BeginCombo("Combo box", comboItems.at(currentIndex))) {
    for (auto index{0U}; index < comboItems.size(); ++index) {
      bool const isSelected{currentIndex == index};
      if (ImGui::Selectable(comboItems.at(index), isSelected))
        currentIndex = index;

      // Set the initial focus when opening the combo (scrolling + keyboard
      // navigation focus)
      if (isSelected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }

  fmt::print("Selected combo box item: {}\n", comboItems.at(currentIndex));

  ImGui::SetNextWindowSize(ImVec2(300, 100));
  auto flags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize};
  ImGui::Begin("Window with menu", nullptr, flags);
  {
    bool save{};
    static bool showCompliment{};  // Hold state

    // Menu Bar
    if (ImGui::BeginMenuBar()) {
      // File menu
      if (ImGui::BeginMenu("File")) {
        ImGui::MenuItem("Save", nullptr, &save);
        ImGui::EndMenu();
      }
      // View menu
      if (ImGui::BeginMenu("View")) {
        ImGui::MenuItem("Show Compliment", nullptr, &showCompliment);
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    if (save) {
      // Save file...
    }

    if (showCompliment) {
      ImGui::Text("You're a beautiful person.");
    }
  }
  ImGui::End();

  static std::array pos2d{0.0f, 0.0f};
  ImGui::SliderFloat2("2D position", pos2d.data(), 0.0, 50.0);

  static std::array pos3d{0.0f, 0.0f, 0.0f};
  ImGui::SliderFloat3("3D position", pos3d.data(), -1.0, 1.0);

  static int i;
  ImGui::SliderInt("i", &i, 0, 36);
  
}