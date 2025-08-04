// clang-format off
#include <glad/glad.h>
// clang-format on
#include "cube_renderer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "mesh_renderer.hpp"
#include "shader.hpp"


// clang-format off
const std::vector<float> cube_vertices = {
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,

    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,

    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,

    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f};
// clang-format on

CubeRenderer::CubeRenderer() : MeshRenderer(cube_vertices, Shader(Shader::cube_vertex_src, Shader::cube_fragment_src)) {}

void CubeRenderer::render(const std::vector<Cube>& cubes,
                          glm::mat4 view,
                          glm::mat4 projection) {
  glBindVertexArray(_VAO);

  _shader.setMat4("view", view);
  _shader.setMat4("projection", projection);
  _shader.use();

  for (const auto& c : cubes) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(c.pos.x, c.pos.y, c.pos.z));
    model *= glm::mat4_cast(c.rot);

    _shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, _vertex_count);
  }

  glBindVertexArray(0);
}