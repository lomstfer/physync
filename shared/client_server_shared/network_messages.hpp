#pragma once
#pragma once
#include "external/cereal/cereal.hpp"
#include "external/cereal/archives/binary.hpp"
#include "external/cereal/types/vector.hpp"
#include <vector>
#include <sstream>
#include <cstdint>


namespace Msg {
struct CubeData {
    float pos_x;
    float pos_y;
    float pos_z;

    // quaternion
    float rot_x;
    float rot_y;
    float rot_z;
    float rot_w;

    template <class Archive> void serialize(Archive &archive) {
        archive(pos_x, pos_y, pos_z, rot_x, rot_y, rot_z, rot_w);
    }
};

namespace ServerToClient {
enum MsgId { NewWorldState, TimeResponse };

struct NewWorldStateData {
    std::vector<CubeData> cubes;
    double timestamp;
    template <class Archive> void serialize(Archive &archive) {
        archive(cubes, timestamp);
    }
};

struct TimeResponseData {
    double request_time_sent;
    double response_time_sent;
    template <class Archive> void serialize(Archive &archive) {
        archive(request_time_sent, response_time_sent);
    }
};
} // namespace ServerToClient

namespace ClientToServer {
enum MsgId { TimeRequest };

struct TimeRequestData {
    double request_time_sent;
    template <class Archive> void serialize(Archive &archive) {
        archive(request_time_sent);
    }
};
} // namespace ClientToServer

template <typename MsgType, typename DataType>
std::vector<uint8_t> get_bytes_to_send(MsgType msg_id, DataType data) {
    std::ostringstream ss;
    cereal::BinaryOutputArchive archive(ss);

    archive(static_cast<uint8_t>(msg_id), data);

    std::string serialized = ss.str();
    return std::vector<uint8_t>(serialized.begin(), serialized.end());
}

template <typename DataType>
DataType get_data_struct(const std::vector<uint8_t> &bytes_without_id) {
    std::string data_str(bytes_without_id.begin(), bytes_without_id.end());
    std::istringstream ss(data_str);
    cereal::BinaryInputArchive archive(ss);

    DataType data;
    archive(data);
    return data;
}

} // namespace Msg
