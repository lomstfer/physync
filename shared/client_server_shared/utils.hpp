#pragma once
#include <chrono>

namespace utils {
inline double unix_time_seconds() {
    return std::chrono::duration<double>(
               std::chrono::high_resolution_clock::now().time_since_epoch())
        .count();
}
} // namespace utils