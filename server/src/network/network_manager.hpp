#include "enet.h"
#include <cstdint>
#include <functional>


class NetworkManager {
  ENetHost *_enet_server_host;

public:
  NetworkManager(int port);
  ~NetworkManager();
  void check_for_events(
      std::function<void(std::vector<std::uint8_t>,
                         std::function<void(std::vector<std::uint8_t>)>
                             send_to_client_callback)>
          data_callback);
  void broadcast(std::vector<std::uint8_t> data);
};
