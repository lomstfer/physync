#pragma once
#include "client_server_shared/network_messages.hpp"
#include <cstdint>
#include <vector>

class TimeSyncer {
  private:
    double time_diff_to_server;
    unsigned int minimum_responses = 10;
    unsigned int respones_handled = 0;
    double minimum_latency_calculated = -1;
    bool finished_sync;

  public:
    std::vector<uint8_t> get_new_request();
    void handle_response(Msg::ServerToClient::TimeResponseData data);
    double get_server_time();
    bool is_synchronized();
};