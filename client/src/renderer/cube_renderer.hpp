#pragma once

#include "../world_state/cube.hpp"
#include <vector>
#include "mesh_renderer.hpp"
class CubeRenderer : public MeshRenderer
{
public:
    CubeRenderer();
    void render(const std::vector<Cube> &cubes, glm::mat4 view, glm::mat4 projection);
};