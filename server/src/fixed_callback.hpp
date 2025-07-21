#pragma once

#include <functional>
#include <chrono>

class FixedCallback
{
    float time_step;
    float accumulator = 0;
    std::chrono::steady_clock::time_point time_last;

public:
    FixedCallback(float time_step) : time_step(time_step)
    {
        time_last = std::chrono::high_resolution_clock::now();
    };
    void update(std::function<void()> callback)
    {
        auto time_now = std::chrono::high_resolution_clock::now();
        accumulator += std::chrono::duration_cast<std::chrono::duration<float>>(time_now - time_last).count();
        time_last = time_now;

        while (accumulator >= time_step)
        {
            accumulator -= time_step;
            std::cout << accumulator << " ";
            callback();
        }
    };
};