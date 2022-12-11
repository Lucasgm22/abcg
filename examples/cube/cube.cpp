#include "cube.hpp"

#include <unordered_map>
#include <glm/gtx/fast_trigonometry.hpp>

// Explicit specialization of std::hash for Vertex
template <> struct std::hash<Vertex> {
  size_t operator()(Vertex const &vertex) const noexcept {
    auto const h1{std::hash<glm::vec3>()(vertex.position)};
    auto const h2{std::hash<glm::vec3>()(vertex.normal)};
    return abcg::hashCombine(h1, h2);
  }
};

void Cube::createBuffers() {
  // Delete previous buffers
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);

  // VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER,
                     sizeof(m_vertices.at(0)) * m_vertices.size(),
                     m_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(m_indices.at(0)) * m_indices.size(),
                     m_indices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Cube::loadObj(std::string_view path) {
  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(path.data())) {
    if (!reader.Error().empty()) {
      throw abcg::RuntimeError(
          fmt::format("Failed to load model {} ({})", path, reader.Error()));
    }
    throw abcg::RuntimeError(fmt::format("Failed to load model {}", path));
  }

  if (!reader.Warning().empty()) {
    fmt::print("Warning: {}\n", reader.Warning());
  }

  auto const &attrib{reader.GetAttrib()};
  auto const &shapes{reader.GetShapes()};

  m_vertices.clear();
  m_indices.clear();

  m_hasNormals = false;

  // A key:value map with key=Vertex and value=index
  std::unordered_map<Vertex, GLuint> hash{};

  // Loop over shapes
  for (auto const &shape : shapes) {
    // Loop over indices
    for (auto const offset : iter::range(shape.mesh.indices.size())) {
      // Access to vertex
      auto const index{shape.mesh.indices.at(offset)};

      // Vertex position
      auto const startIndex{3 * index.vertex_index};
      glm::vec3 position{attrib.vertices.at(startIndex + 0),
                         attrib.vertices.at(startIndex + 1),
                         attrib.vertices.at(startIndex + 2)};

      // Vertex normal
      glm::vec3 normal{};
      if (index.normal_index >= 0) {
        m_hasNormals = true;
        auto const normalStartIndex{3 * index.normal_index};
        normal = {attrib.normals.at(normalStartIndex + 0),
                  attrib.normals.at(normalStartIndex + 1),
                  attrib.normals.at(normalStartIndex + 2)};
      }

      Vertex const vertex{.position = position, .normal = normal};

      // If hash doesn't contain this vertex
      if (!hash.contains(vertex)) {
        // Add this index (size of m_vertices)
        hash[vertex] = m_vertices.size();
        // Add this vertex
        m_vertices.push_back(vertex);
      }

      m_indices.push_back(hash[vertex]);
    }
  }

  if (!m_hasNormals) {
    computeNormals();
  }

  createBuffers();
}

void Cube::computeNormals() {
  // Clear previous vertex normals
  for (auto &vertex : m_vertices) {
    vertex.normal = glm::vec3(0.0f);
  }

  // Compute face normals
  for (auto const offset : iter::range(0UL, m_indices.size(), 3UL)) {
    // Get face vertices
    auto &a{m_vertices.at(m_indices.at(offset + 0))};
    auto &b{m_vertices.at(m_indices.at(offset + 1))};
    auto &c{m_vertices.at(m_indices.at(offset + 2))};

    // Compute normal
    auto const edge1{b.position - a.position};
    auto const edge2{c.position - b.position};
    auto const normal{glm::cross(edge1, edge2)};

    // Accumulate on vertices
    a.normal += normal;
    b.normal += normal;
    c.normal += normal;
  }

  // Normalize
  for (auto &vertex : m_vertices) {
    vertex.normal = glm::normalize(vertex.normal);
  }

  m_hasNormals = true;
}

void Cube::paint() {

  // Set uniform variables for the cube
  m_positionMatrix = glm::translate(glm::mat4{1.0f}, m_position);
  m_modelMatrix = m_positionMatrix * m_animationMatrix;
  m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(m_scale, m_scale, m_scale));
  abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix[0][0]);
  abcg::glUniform4f(m_colorLoc, 1.0f, 0.0f, 0.0f, 1.0f); // RED

  abcg::glUniform1f(m_KaLoc, m_Ka); 
  abcg::glUniform1f(m_KdLoc, m_Kd);
  abcg::glUniform1f(m_KsLoc, m_Ks);  

  auto const modelViewMatrix{glm::mat3(m_viewMatrix * m_modelMatrix)};
  auto const normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  abcg::glUniformMatrix3fv(m_normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  //SET uniform variables here
  abcg::glBindVertexArray(m_VAO);

  abcg::glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);
}

void Cube::create(GLuint program, GLint modelMatrixLoc, GLint colorLoc, GLint normalMatrixLoc, glm::mat4 viewMatrix, float scale, int N) {
  // Release previous VAO
  abcg::glDeleteVertexArrays(1, &m_VAO);

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);
  abcg::glBindVertexArray(m_VAO);

  // Bind EBO and VBO
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

  // Bind vertex attributes
  auto const positionAttribute{
      abcg::glGetAttribLocation(program, "inPosition")};
  if (positionAttribute >= 0) {
    abcg::glEnableVertexAttribArray(positionAttribute);
    abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex), nullptr);
  }

  auto const normalAttribute{abcg::glGetAttribLocation(program, "inNormal")};
  if (normalAttribute >= 0) {
    abcg::glEnableVertexAttribArray(normalAttribute);
    auto const offset{offsetof(Vertex, normal)};
    abcg::glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex),
                                reinterpret_cast<void *>(offset));
  }

  // End of binding
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  abcg::glBindVertexArray(0);

  m_KaLoc = abcg::glGetUniformLocation(program, "Ka");
  m_KdLoc = abcg::glGetUniformLocation(program, "Kd");
  m_KsLoc = abcg::glGetUniformLocation(program, "Ks");


  m_modelMatrixLoc = modelMatrixLoc;
  m_normalMatrixLoc = normalMatrixLoc;
  m_viewMatrix = viewMatrix;
  m_colorLoc = colorLoc;
  m_scale = scale;
  m_maxPos = m_scale * N;
}

void Cube::update(float deltaTime) {
  move(deltaTime);
}


void Cube::destroy() const {
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}


void Cube::move(float deltaTime) {
  if (!m_isMoving) return;
  if (m_angle >= 0.0f && m_angle < 90.0f) { //Angle in [0, 90) continue moving
    increaseAngle(deltaTime * m_angleVelocity);
    m_animationMatrix = glm::rotate(glm::mat4{1.0f}, glm::radians(gsl::narrow_cast<int>(m_orientation) * 90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //ROTATE AROUND A DIRECTION
    m_animationMatrix = glm::translate(m_animationMatrix, glm::vec3(0, -m_scale/2, m_scale/2)); //PUT ON ORIGIN
    m_animationMatrix = glm::rotate(m_animationMatrix, glm::radians(m_angle), glm::vec3(1.0f, 0.0f, 0.0f)); //ROTATE AROUND X axis
    m_animationMatrix = glm::translate(m_animationMatrix, glm::vec3(0, m_scale/2, -m_scale/2));//TRANSLATTE TO MATCH X axis
  } else if (m_angle >= 90.0f) { //Angle in [90, +infinity) finish moviment
    resetAnimation(); 
    translate();
  }
}

void Cube::resetAnimation() {
  m_animationMatrix = glm::mat4{1.0f};
  m_angle = 0.0f;
  m_isMoving = false;
}

void Cube::translate() {
  switch (m_orientation) {
    case Orientation::DOWN:
      m_position.z += m_scale;
      break;
    case Orientation::UP:
      m_position.z -= m_scale;
      break;
    case Orientation::LEFT:
      m_position.x -= m_scale;
      break;
    case Orientation::RIGHT:
      m_position.x += m_scale;
      break;
  }
}

void Cube::moveDown() {
  if (m_isMoving || m_position.z + m_scale > m_maxPos) return;
  m_isMoving = true;
  m_orientation = Orientation::DOWN;
}

void Cube::moveUp() {
  if (m_isMoving || m_position.z - m_scale < -m_maxPos) return;
  m_isMoving = true;
  m_orientation = Orientation::UP;
}

void Cube::moveLeft() {
  if (m_isMoving || m_position.x - m_scale < -m_maxPos) return;
  m_isMoving = true;
  m_orientation = Orientation::LEFT;
}

void Cube::moveRigth() {
  if (m_isMoving || m_position.x + m_scale > m_maxPos) return; 
  m_isMoving = true;
  m_orientation = Orientation::RIGHT;
}

void Cube::increaseAngle(float inc) {
  m_angle += inc;
  m_angle = m_angle <= 90.0f ? m_angle : 90.0f;
}
