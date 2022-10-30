#include "board.hpp"


void Board::create(GLuint program) {
  destroy();

  m_program = program;

  // Get location of uniforms in the program

  m_color1Loc = abcg::glGetUniformLocation(m_program, "u_color1");
  m_color2Loc = abcg::glGetUniformLocation(m_program, "u_color2");
  m_resolutionLoc = abcg::glGetUniformLocation(m_program, "u_resolution");
  m_mouseLoc = abcg::glGetUniformLocation(m_program, "u_mouse");



  // clang-format off
  // Create geometry data

  std::array positions{
      // Ship body
      glm::vec2{-1.0f, +1.0f}, glm::vec2{-1.0f, -1.0f},
      glm::vec2{+1.0f, -1.0f}, glm::vec2{+1.0f, +1.0f},
      };



  std::array const indices{0, 1, 2,
                           0, 2, 3,};
  // clang-format on                           

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  auto const seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);

  randomColor1();
  randomColor2();
}

void Board::paint() {
  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_VAO);


  abcg::glUniform2f(m_resolutionLoc, m_resolution.x, m_resolution.y);
  abcg::glUniform2f(m_mouseLoc, m_mouse.x, m_mouse.y);
  abcg::glUniform3f(m_color1Loc, m_color1.r, m_color1.g, m_color1.b);
  abcg::glUniform3f(m_color2Loc, m_color2.r, m_color2.g, m_color2.b);


  abcg::glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Board::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}


void Board::resize(glm::vec2 size) {
  m_resolution = size;
}

void Board::randomColors() {
  randomColor1();
  randomColor2();
}

void Board::randomColor1() {
  m_color1 = randomColor();
}

void Board::randomColor2() {
  m_color2 = randomColor();
}

glm::vec3 Board::randomColor() {
  std::uniform_real_distribution rd_c(0.0f, 1.0f);

  return glm::vec3(rd_c(m_randomEngine), rd_c(m_randomEngine), rd_c(m_randomEngine));
}