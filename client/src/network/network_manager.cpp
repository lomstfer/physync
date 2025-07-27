#include <cstdint>
#include <vector>
#define ENET_IMPLEMENTATION
#include "network_manager.hpp"
#include <iostream>
#include "enet.h"

NetworkManager::NetworkManager(std::string address, int port) {
  if (enet_initialize() != 0) {
    std::cout << "An error occurred while initializing ENet.\n";
    throw;
  }

  enet_client_host = {0};
  enet_client_host = enet_host_create(NULL, 1, 1, 0, 0);
  if (enet_client_host == NULL) {
    std::cout
        << "An error occurred while trying to create an ENet client host.\n";
    throw;
  }

  e_address = {0};
  enet_server_peer = {0};
  enet_address_set_host(&e_address, address.c_str());
  e_address.port = port;
}

bool NetworkManager::connect() {
  enet_server_peer = enet_host_connect(enet_client_host, &e_address, 2, 0);
  if (enet_server_peer == NULL) {
    std::cout << "No available peers for initiating an ENet connection.\n";
    throw;
  }

  ENetEvent event{};
  if (enet_host_service(enet_client_host, &event, 5000) > 0 &&
      event.type == ENET_EVENT_TYPE_CONNECT) {
    std::cout << "Connection succeeded.\n";
    return true;
  } else {
    enet_peer_reset(enet_server_peer);
    std::cout << "Connection failed.\n";
    return false;
  }
}

void NetworkManager::check_for_events(std::function<void(std::vector<std::uint8_t>)> data_callback) {
  ENetEvent event{};
  enet_host_service(enet_client_host, &event, 0);
  switch (event.type) {
    case ENET_EVENT_TYPE_RECEIVE: {
      std::vector<uint8_t> data_vec(event.packet->data, 
                                   event.packet->data + event.packet->dataLength);
      data_callback(data_vec);

      enet_packet_destroy(event.packet);
    } break;
    case ENET_EVENT_TYPE_DISCONNECT:
      puts("Disconnection.");
      break;
  }
}

NetworkManager::~NetworkManager() {
  ENetEvent event{};
  enet_peer_disconnect(enet_server_peer, 0);
  if (enet_host_service(enet_client_host, &event, 2000) > 0 &&
      event.type == ENET_EVENT_TYPE_DISCONNECT) {
    std::cout << "Disconnection succeeded.\n";
  }
}