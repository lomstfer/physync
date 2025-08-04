// clang-format off
#include <glad/glad.h>
// clang-format on
#include "mesh_renderer.hpp"
#include "shader.hpp"
#include <vector>

MeshRenderer::MeshRenderer(const std::vector<float>& vertices, Shader shader) : _shader(shader) {
  _vertex_count = vertices.size() / 3;

  glGenVertexArrays(1, &_VAO);
  glBindVertexArray(_VAO);

  glGenBuffers(1, &_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(),
               vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}