#pragma once

#include "../world_state/cube.hpp"
#include <vector>
#include "shader.hpp"
class CubeRenderer
{
    unsigned int VBO;
    unsigned int VAO;
    Shader _shader = Shader(Shader::cube_vertex_src, Shader::cube_fragment_src);

public:
    CubeRenderer();
    void render(const std::vector<Cube> &cubes, glm::mat4 view, glm::mat4 projection);
};