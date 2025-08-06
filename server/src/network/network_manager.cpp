#define ENET_IMPLEMENTATION
#include "network_manager.hpp"
#include "enet.h"
#include <iostream>

NetworkManager::NetworkManager(int port) {
  if (enet_initialize() != 0) {
    std::cout << "An error occurred while initializing ENet.\n";
    throw;
  }

  ENetAddress address = {0};

  address.host = ENET_HOST_ANY;
  address.port = port;

  _enet_server_host = enet_host_create(&address, 200, 1, 0, 0);
  if (_enet_server_host == NULL) {
    std::cout
        << "An error occurred while trying to create an ENet server host.\n";
    throw;
  }
}

void NetworkManager::check_for_events(
    std::function<void(
        std::vector<std::uint8_t>,
        std::function<void(std::vector<std::uint8_t>)> send_to_client_callback)>
        data_callback) {
  ENetEvent event;

  while (enet_host_service(_enet_server_host, &event, 0) > 0) {
    switch (event.type) {
    case ENET_EVENT_TYPE_CONNECT: {
      printf("A new client connected %u.\n", event.peer->connectID);
      break;
    }

    case ENET_EVENT_TYPE_RECEIVE: {
      std::vector<uint8_t> data_vec(
          event.packet->data, event.packet->data + event.packet->dataLength);
          
      data_callback(data_vec, [&event](std::vector<std::uint8_t> data) {
        ENetPacket *packet = enet_packet_create(data.data(), data.size(),
                                                ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(event.peer, 0, packet);
      });

      enet_packet_destroy(event.packet);
      break;
    }

    case ENET_EVENT_TYPE_DISCONNECT: {
      printf("%u disconnected.\n", event.peer->connectID);
      event.peer->data = NULL;
      break;
    }

    case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT: {
      printf("%u disconnected due to timeout.\n", event.peer->connectID);
      break;
    }

    case ENET_EVENT_TYPE_NONE: {
      break;
    }
    }
  }
}

void NetworkManager::broadcast(std::vector<std::uint8_t> data) {
  ENetPacket *packet =
      enet_packet_create(data.data(), data.size(), ENET_PACKET_FLAG_RELIABLE);
  for (int i = 0; i < _enet_server_host->peerCount; i++) {
    auto p = &_enet_server_host->peers[i];
    enet_peer_send(p, 0, packet);
  }
}

NetworkManager::~NetworkManager() {
  enet_host_destroy(_enet_server_host);
  enet_deinitialize();
}