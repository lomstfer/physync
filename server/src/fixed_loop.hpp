#pragma once

#include <chrono>
#include <functional>
#include <thread>

class FixedLoop {
  float time_step;
  std::chrono::steady_clock::time_point update_time;

 public:
  FixedLoop(float time_step) : time_step(time_step) {};

  void loop(std::function<void()> callback) {
    auto update_time = std::chrono::high_resolution_clock::now();
    while (true) {
      auto current_time = std::chrono::high_resolution_clock::now();
      if (update_time < current_time) {
        callback();
        update_time += std::chrono::duration_cast<
            std::chrono::high_resolution_clock::duration>(
            std::chrono::duration<float>(time_step));
      } else {
        std::this_thread::sleep_for(update_time - current_time);
      }
    }
  }
};