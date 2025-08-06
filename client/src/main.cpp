// clang-format off
#include <cassert>
#include <cstdlib>
#include <glad/glad.h>
// clang-format on
#include <algorithm>
#include <iomanip>
#include <iostream>

#include "client_server_shared/network_messages.hpp"
#include "network/network_manager.hpp"
#include "network/time_syncer.hpp"
#include "renderer/renderer.hpp"
#include "utils.hpp"
#include "world_state/cube.hpp"

GLFWwindow *init_glfw_window() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);
  GLFWwindow *window = glfwCreateWindow(1280, 720, "physync", NULL, NULL);

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
  std::vector<std::vector<CubeSnapshot>> cube_snapshots;

  Renderer renderer = Renderer(init_glfw_window());

  NetworkManager network_manager = NetworkManager("127.0.0.1", 5555);
  if (network_manager.connect() == false) {
    exit(1);
  }

  TimeSyncer time_syncer = TimeSyncer();
  network_manager.send_data_reliable(time_syncer.get_new_request());

  double lt = 0;

  while (renderer.consume_state(world_state)) {
    network_manager.check_for_events(
        [&](const std::vector<std::uint8_t> &data) {
          const std::vector<uint8_t> bytes_without_id(data.begin() + 1,
                                                      data.end());
          switch (data[0]) {
          case Msg::ServerToClient::MsgId::NewWorldState: {
            // std::cout << "server time: " << time_syncer.get_server_time() <<
            // std::endl;
            auto new_world_data =
                Msg::get_data_struct<Msg::ServerToClient::NewWorldStateData>(
                    bytes_without_id);
            for (int i = 0; i < new_world_data.cubes.size(); i++/* const Msg::CubeData &cube_data : new_world_data.cubes */) {
              Msg::CubeData cube_data = new_world_data.cubes[i];
              if (i < cube_snapshots.size()) {
                cube_snapshots[i].push_back(CubeSnapshot{
                    Cube(cube_data.pos_x, cube_data.pos_y, cube_data.pos_z,
                         cube_data.rot_x, cube_data.rot_y, cube_data.rot_z,
                         cube_data.rot_w),
                    new_world_data.timestamp});
              } else {
                cube_snapshots.push_back({CubeSnapshot{
                    Cube(cube_data.pos_x, cube_data.pos_y, cube_data.pos_z,
                         cube_data.rot_x, cube_data.rot_y, cube_data.rot_z,
                         cube_data.rot_w),
                    new_world_data.timestamp}});
              }
            }
            for (auto &snapshots : cube_snapshots) {
              std::sort(snapshots.begin(), snapshots.end(),
                        [](const CubeSnapshot &a, const CubeSnapshot &b) {
                          return a.timestamp < b.timestamp;
                        });
            }
            break;
          }
          case Msg::ServerToClient::TimeResponse: {
            auto time_response_data =
                Msg::get_data_struct<Msg::ServerToClient::TimeResponseData>(
                    bytes_without_id);
            time_syncer.handle_response(time_response_data);
            if (!time_syncer.is_synchronized()) {
              network_manager.send_data_reliable(time_syncer.get_new_request());
            }
            break;
          }
          }
        });

    lt = time_syncer.get_server_time();
    world_state.cubes.clear();
    float render_time = time_syncer.get_server_time() - 1.f / 30.f * 2;
    for (std::vector<CubeSnapshot> &snapshots : cube_snapshots) {
      while (snapshots.size() >= 2 && snapshots[1].timestamp < render_time) {
        snapshots.erase(snapshots.begin());
      }
      if (snapshots.empty()) {
        continue;
      }
      if (snapshots.size() == 1) {
        world_state.cubes.push_back(snapshots[0].data);
        continue;
      }
      if (snapshots.size() >= 2) {
        Cube c = CubeSnapshot::lerp(snapshots[0], snapshots[1], render_time);
        world_state.cubes.push_back(c);
      }
    }
  }
}