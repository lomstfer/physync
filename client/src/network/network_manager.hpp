#include "enet.h"
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

class NetworkManager {
  ENetHost *enet_client_host;
  ENetPeer *enet_server_peer;
  ENetAddress e_address;

public:
  NetworkManager(std::string adress, int port);
  ~NetworkManager();
  bool connect();
  void check_for_events(
      std::function<void(std::vector<std::uint8_t>)> data_callback);
  void send_data_reliable(const std::vector<uint8_t> &data);
};
