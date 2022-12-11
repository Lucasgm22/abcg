#include "window.hpp"

#include <filesystem>


void Window::onEvent(SDL_Event const &event) {

  if (event.type == SDL_KEYDOWN) {
 
    if (event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_UP) 
      m_cube.moveUp();
    
    if (event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_DOWN) 
      m_cube.moveDown();
    
    if (event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_LEFT) 
      m_cube.moveLeft();
    
    if (event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT) 
      m_cube.moveRigth();
    
  }
}

void Window::onCreate() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0, 0, 0, 1);
  abcg::glEnable(GL_DEPTH_TEST);

  m_viewMatrix =glm::lookAt(glm::vec3(1.9f, 1.9f, 1.9f),
                            glm::vec3(0.0f, 0.0f, 0.0f), 
                            glm::vec3(0.0f, 1.0f, 0.0f));

  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "light.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "light.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  m_viewMatrixLoc = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLoc = abcg::glGetUniformLocation(m_program, "projMatrix");
  m_modelMatrixLoc = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_normalMatrixLoc = abcg::glGetUniformLocation(m_program, "normalMatrix");
  m_lightPositionLoc = abcg::glGetUniformLocation(m_program, "lightPosition");
  m_IaLoc = abcg::glGetUniformLocation(m_program, "Ia");
  m_IdLoc = abcg::glGetUniformLocation(m_program, "Id");
  m_IsLoc = abcg::glGetUniformLocation(m_program, "Is");
  loadSkyTexture(assetsPath + "maps/sky/");


  m_ground.create(m_program, m_modelMatrixLoc, m_colorLoc, m_normalMatrixLoc, m_viewMatrix, m_scale, m_N);
  m_cube.loadObj(assetsPath + "box.obj");
  m_cube.create(m_program, m_modelMatrixLoc, m_colorLoc, m_normalMatrixLoc, m_viewMatrix, m_scale, m_N);
  createSkybox();

}

void Window::onUpdate() {
  m_cube.update(gsl::narrow_cast<float>(getDeltaTime()));
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  // Set uniform variables that have the same value for every model
  auto const aspect{gsl::narrow<float>(m_viewportSize.x) /
                        gsl::narrow<float>(m_viewportSize.y)};

  m_projMatrix = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 5.0f);


  abcg::glUniformMatrix4fv(m_viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform3f(m_lightPositionLoc, m_lightPos.x, m_lightPos.y, m_lightPos.z);
  abcg::glUniform1f(m_IaLoc, m_Ia);
  abcg::glUniform1f(m_IdLoc, m_Id);
  abcg::glUniform1f(m_IsLoc, m_Is);



  m_cube.paint();
  m_ground.paint();
  renderSkybox();


  abcg::glUseProgram(0);
}


void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
}

void Window::onDestroy() {
  m_ground.destroy();
  m_cube.destroy();
  destroySkybox();
  abcg::glDeleteProgram(m_program);
}


void Window::createSkybox() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  // Create skybox program
  auto const path{assetsPath + m_skyShaderName};
  m_skyProgram = abcg::createOpenGLProgram(
      {{.source = path + ".vert", .stage = abcg::ShaderStage::Vertex},
       {.source = path + ".frag", .stage = abcg::ShaderStage::Fragment}});

  // Generate VBO
  abcg::glGenBuffers(1, &m_skyVBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_skyVBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyPositions),
                     m_skyPositions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_skyProgram, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_skyVAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_skyVAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_skyVBO);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Window::renderSkybox() {
  abcg::glUseProgram(m_skyProgram);

  auto const viewMatrixLoc{
      abcg::glGetUniformLocation(m_skyProgram, "viewMatrix")};
  auto const projMatrixLoc{
      abcg::glGetUniformLocation(m_skyProgram, "projMatrix")};
  auto const skyTexLoc{abcg::glGetUniformLocation(m_skyProgram, "skyTex")};

  auto const viewMatrix{m_viewMatrix};
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform1i(skyTexLoc, 0);

  abcg::glBindVertexArray(m_skyVAO);

  abcg::glActiveTexture(GL_TEXTURE0);
  abcg::glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyTexture);

  abcg::glEnable(GL_CULL_FACE);
  abcg::glFrontFace(GL_CW);
  abcg::glDepthFunc(GL_LEQUAL);
  abcg::glDrawArrays(GL_TRIANGLES, 0, m_skyPositions.size());
  abcg::glDepthFunc(GL_LESS);
  abcg::glDisable(GL_CULL_FACE);
  abcg::glFrontFace(GL_CCW);

  abcg::glBindVertexArray(0);
  abcg::glUseProgram(0);
}

void Window::destroySkybox() const {
  abcg::glDeleteProgram(m_skyProgram);
  abcg::glDeleteBuffers(1, &m_skyVBO);
  abcg::glDeleteVertexArrays(1, &m_skyVAO);
}

void Window::loadSkyTexture(std::string const &path) {
  if (!std::filesystem::exists(path))
    return;

  abcg::glDeleteTextures(1, &m_skyTexture);
  m_skyTexture = abcg::loadOpenGLCubemap(
      {.paths = {path + "posx.jpg", path + "negx.jpg", path + "posy.jpg",
                 path + "negy.jpg", path + "posz.jpg", path + "negz.jpg"}});
}