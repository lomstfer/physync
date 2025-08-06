#include "time_syncer.hpp"
#include "network_messages.hpp"
#include "utils.hpp"

std::vector<uint8_t> TimeSyncer::get_new_request() {
    auto data = Msg::ClientToServer::TimeRequestData{utils::unix_time_seconds()};
    return Msg::get_bytes_to_send(Msg::ClientToServer::TimeRequest, data);
}

void TimeSyncer::handle_response(Msg::ServerToClient::TimeResponseData data) {
    double now = utils::unix_time_seconds();
    double round_trip = now - data.request_time_sent;
    double one_way_latency = round_trip / 2.f;
    
    if (minimum_latency_calculated == -1 || one_way_latency < minimum_latency_calculated) {
        minimum_latency_calculated = one_way_latency;
        double predicted_server_time = data.response_time_sent + one_way_latency;
        time_diff_to_server = now - predicted_server_time;
    }
    
    respones_handled += 1;
    if (respones_handled >= minimum_responses) {
        finished_sync = true;
    }
}

double TimeSyncer::get_server_time() {
    double now = utils::unix_time_seconds();

    return now - time_diff_to_server;
}

bool TimeSyncer::is_synchronized() {
    return finished_sync;
}