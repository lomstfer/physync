#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../world_state/world_state.hpp"
#include "cube_renderer.hpp"


class Renderer {
  GLFWwindow* window;
  glm::vec3 camera_pos;
  glm::vec3 camera_front;
  glm::vec3 camera_up;
  CubeRenderer cube_renderer;

 public:
  Renderer(GLFWwindow* window);
  ~Renderer();
  bool consume_state(const WorldState& state);
};