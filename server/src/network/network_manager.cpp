#define ENET_IMPLEMENTATION
#include "network_manager.hpp"
#include <iostream>
#include "enet.h"

NetworkManager::NetworkManager(int port) {
  if (enet_initialize() != 0) {
    std::cout << "An error occurred while initializing ENet.\n";
    throw;
  }

  ENetAddress address = {0};

  address.host = ENET_HOST_ANY;
  address.port = port;

  enet_server_host = enet_host_create(&address, 200, 1, 0, 0);
  if (enet_server_host == NULL) {
    std::cout
        << "An error occurred while trying to create an ENet server host.\n";
    throw;
  }
}

void NetworkManager::check_for_events(std::function<void()> callback) {
  ENetEvent event;

  while (enet_host_service(enet_server_host, &event, 0) > 0) {
    switch (event.type) {
      case ENET_EVENT_TYPE_CONNECT:
        printf("A new client connected %u.\n", event.peer->connectID);
        /* Store any relevant client information here. */
        // event.peer->data = "Client information";
        break;

      case ENET_EVENT_TYPE_RECEIVE:
        printf(
            "A packet of length %zu containing %s was received from %s on "
            "channel %u.\n",
            event.packet->dataLength, event.packet->data, event.peer->data,
            event.channelID);
        /* Clean up the packet now that we're done using it. */
        enet_packet_destroy(event.packet);
        break;

      case ENET_EVENT_TYPE_DISCONNECT:
        printf("%s disconnected.\n", event.peer->data);
        /* Reset the peer's client information. */
        event.peer->data = NULL;
        break;

      case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
        printf("%u disconnected due to timeout.\n", event.peer->connectID);
        /* Reset the peer's client information. */
        event.peer->data = NULL;
        break;

      case ENET_EVENT_TYPE_NONE:
        break;
    }
  }
}

void NetworkManager::broadcast(std::vector<std::uint8_t> data) {
  ENetPacket* packet = enet_packet_create(data.data(), data.size(),
                                          ENET_PACKET_FLAG_RELIABLE);
  for (int i = 0; i < enet_server_host->peerCount; i++) {
    auto p = &enet_server_host->peers[i];
    enet_peer_send(p, 0, packet);
  }
}

NetworkManager::~NetworkManager() {
  enet_host_destroy(enet_server_host);
  enet_deinitialize();
}