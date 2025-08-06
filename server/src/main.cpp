#include <cstdint>
#include <iostream>
#include <vector>
// clang-format off
#include "fixed_loop.hpp"
#include "fixed_callback.hpp"
#include "utils.hpp"
#include "world_manager.hpp"
#include "network/network_manager.hpp"
// clang-format on
#include "client_server_shared/network_messages.hpp"

int main() {
  std::cout << "physync server" << std::endl;

  float unix_time_started = utils::unix_time_seconds();

  WorldManager world_manager;
  NetworkManager network_manager(5555);
  const float time_step = 1.0f / 30.0f;
  const float broadcast_delay = 1.0f / 30.0f;

  world_manager.add_cube(3, 10, 3);
  world_manager.add_cube(5, 3, 3);
  world_manager.add_cube(0, 5, 0);

  FixedLoop main_loop(time_step);
  FixedCallback broadcast_callback(broadcast_delay);
  main_loop.loop([&] {
    const double server_time = utils::unix_time_seconds() - unix_time_started;

    network_manager.check_for_events(
        [server_time](std::vector<std::uint8_t> data,
                      std::function<void(std::vector<std::uint8_t>)>
                          send_to_client_callback) {
          const std::vector<uint8_t> bytes_without_id(data.begin() + 1,
                                                      data.end());
          switch (data[0]) {
          case Msg::ClientToServer::TimeRequest: {
            auto time_request_data =
                Msg::get_data_struct<Msg::ClientToServer::TimeRequestData>(
                    bytes_without_id);
            auto resonse_data = Msg::ServerToClient::TimeResponseData{
                time_request_data.request_time_sent, server_time};
            auto reponse_bytes = Msg::get_bytes_to_send(
                Msg::ServerToClient::TimeResponse, resonse_data);
            send_to_client_callback(reponse_bytes);
          }
          }
        });
    world_manager.simulate(time_step);
    if (rand() % 10 == 0) {
      world_manager.add_cube(rand() % 50 - 25, rand() % 10 + 10,
                             rand() % 50 - 25);
    }

    broadcast_callback.update([&network_manager, &world_manager, server_time] {
      auto data_obj = Msg::ServerToClient::NewWorldStateData{
          world_manager.get_cube_data(), server_time};

      std::vector<uint8_t> bytes =
          Msg::get_bytes_to_send(Msg::ServerToClient::NewWorldState, data_obj);
      network_manager.broadcast(bytes);
    });
  });
}
