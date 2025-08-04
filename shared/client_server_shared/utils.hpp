#pragma once
#include <chrono>

namespace utils {
inline float unix_time_seconds() {
    return std::chrono::duration<float>(
               std::chrono::high_resolution_clock::now().time_since_epoch())
        .count();
}
} // namespace utils