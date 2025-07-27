#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>
// clang-format off
#include "fixed_loop.hpp"
#include "world_manager.hpp"
#include "network/network_manager.hpp"
// clang-format on
#include "client_server_shared/network_messages.hpp"

int main() {
  std::cout << "physync server" << std::endl;

  WorldManager world_manager;
  NetworkManager network_manager = NetworkManager(5555);
  const float time_step = 1.0f / 30.0f;

  world_manager.add_cube(3, 10, 3);
  world_manager.add_cube(3, 100, 3);

  FixedLoop loop = FixedLoop(time_step);
  loop.loop([&] {
    network_manager.check_for_events([] {});
    world_manager.simulate(1.0f / 30.0f);
    if (rand()%1 == 0) {
      world_manager.add_cube(3, 10, 3);
    }

    auto data_obj =
        Msg::ServerToClient::NewWorldStateData{world_manager.get_cube_data()};

    std::vector<uint8_t> bytes =
        Msg::get_bytes_to_send(Msg::ServerToClient::NewWorldState, data_obj);

    network_manager.broadcast(bytes);
  });
}
