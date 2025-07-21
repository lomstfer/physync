#include <iostream>
#include "world_manager.hpp"
#include <thread>
#include "fixed_callback.hpp"

int main()
{
    std::cout << "physync server" << std::endl;

    WorldManager world_manager;

    const float time_step = 1.0f / 60.0f;
    FixedCallback world_update_callback(time_step);
    while (true)
    {
        world_update_callback.update([&world_manager, time_step]()
                                     { world_manager.simulate(time_step); });
        std::cout << "ida" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
}
