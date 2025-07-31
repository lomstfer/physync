#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>
// clang-format off
#include "fixed_loop.hpp"
#include "fixed_callback.hpp"
#include "world_manager.hpp"
#include "network/network_manager.hpp"
// clang-format on
#include "client_server_shared/network_messages.hpp"

int main() {
  std::cout << "physync server" << std::endl;

  WorldManager world_manager;
  NetworkManager network_manager(5555);
  const float time_step = 1.0f / 30.0f;
  const float broadcast_delay = 1.0f / 10.0f;

  world_manager.add_cube(3, 10, 3);
  world_manager.add_cube(3, 100, 3);

  FixedLoop main_loop(time_step);
  FixedCallback broadcast_callback(broadcast_delay);
  main_loop.loop([&] {
    network_manager.check_for_events([] {});
    world_manager.simulate(time_step);
    if (rand()%2 == 0) {
      world_manager.add_cube(rand()%50-25, rand()%10+10, rand()%50-25);
    }

    
    broadcast_callback.update([&network_manager, &world_manager]{
      auto data_obj =
          Msg::ServerToClient::NewWorldStateData{world_manager.get_cube_data()};
  
      std::vector<uint8_t> bytes =
          Msg::get_bytes_to_send(Msg::ServerToClient::NewWorldState, data_obj);
      network_manager.broadcast(bytes);
    });
  });
}
