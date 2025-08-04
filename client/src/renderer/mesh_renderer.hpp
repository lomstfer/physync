#pragma once

#include <vector>
#include "shader.hpp"

class MeshRenderer {
protected:
  unsigned int _VBO;
  unsigned int _VAO;
  Shader _shader;
  int _vertex_count;

 public:
  MeshRenderer(const std::vector<float>& vertices, Shader shader);
};