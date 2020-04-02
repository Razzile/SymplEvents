#include "event_manager.h"
#include <iostream>

using symplevents::EventManager;

const char* kAnimals[] = {
    "Fox",
    "Penguin",
    "Cat",
    "Shark",
    "Elephant",
    "Tiger",
    "Gecko",
    "Frog",
    "Human",
    "Goat"
};

int main()
{
    EventManager& manager = EventManager::GlobalManager();

    // subscribe to 'hello' event taking a string and an int with a callback
    manager.Subscribe<std::string, int>(
        "hello", [](const std::string& animal, int count) -> void
        {
            std::cout << "The quick brown " << animal << " jumped over the lazy dog " << count << " times" << std::endl;
        });

    // subscribe to the world event with a callback
    manager.Subscribe("world", []() { std::cout << "blank msg" << std::endl; });

    std::vector<std::thread> threads;
    threads.reserve(10);
    for (int i = 0; i < 10; i++)
    {
        threads.emplace_back(
            [](EventManager& manager, int thread)
            {
                // trigger the 'hello' event with the animal at index i and the index i
                manager.Send<std::string, int>("hello", kAnimals[thread], thread);
            },
            std::ref(manager), i);
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    manager.Send("world");
}
