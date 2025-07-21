#pragma once

#include "../world_state/cube.hpp"
#include <vector>
#include "shader.hpp"
class CubeRenderer
{
    unsigned int VBO;
    unsigned int VAO;

public:
    CubeRenderer();
    void render(const Cube &cube, Shader &shader);
    void render(const std::vector<Cube> &cubes, Shader &shader);
};