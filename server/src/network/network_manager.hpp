#include <cstdint>
#include <functional>
#include "enet.h"

class NetworkManager {
  ENetHost* enet_server_host;

 public:
  NetworkManager(int port);
  ~NetworkManager();
  void check_for_events(std::function<void()> callback);
  void broadcast(std::vector<std::uint8_t> data);
};
