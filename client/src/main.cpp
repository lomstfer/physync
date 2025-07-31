// clang-format off
#include <cstdlib>
#include <glad/glad.h>
// clang-format on
#include <iostream>

#include "client_server_shared/network_messages.hpp"
#include "network/network_manager.hpp"
#include "renderer/renderer.hpp"

GLFWwindow* init_glfw_window() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  GLFWwindow* window = glfwCreateWindow(1280, 720, "physync", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(1);
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    exit(1);
  }

  return window;
}

int main() {
  WorldState world_state = WorldState();

  Renderer renderer = Renderer(init_glfw_window());

  NetworkManager network_manager = NetworkManager("127.0.0.1", 5555);
  if (network_manager.connect() == false) {
    exit(1);
  }

  while (renderer.consume_state(world_state)) {
    network_manager.check_for_events(
        [&world_state](const std::vector<std::uint8_t>& data) {
          const std::vector<uint8_t> bytes_without_id(data.begin() + 1,
                                                      data.end());
          switch (data[0]) {
            case Msg::ServerToClient::MsgId::NewWorldState: {
              auto new_world_data =
                  Msg::get_data_struct<Msg::ServerToClient::NewWorldStateData>(
                      bytes_without_id);
              world_state.cubes.clear();
              for (const Msg::CubeData& cube_data : new_world_data.cubes) {
                world_state.cubes.push_back(
                    Cube(cube_data.pos_x, cube_data.pos_y, cube_data.pos_z,
                         cube_data.rot_x, cube_data.rot_y, cube_data.rot_z,
                         cube_data.rot_w));
              }
            }
          }
        });
  }
}