#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "event_manager.h"

using symplevents::EventManager;

TEST_CASE("EventManager accepts triggers", "[event]")
{
    EventManager& manager = EventManager::GlobalManager();

    REQUIRE_NOTHROW(manager.Subscribe("test_event", []()
        {

        }));

    REQUIRE_NOTHROW(manager.Subscribe<int, const char*>("test_event_args", [](int, const char*)
        {

        }));
}

TEST_CASE("Triggering events in EventManager calls events", "[event]")
{
    SECTION("Parameterless trigger") {
        EventManager manager;

        bool called = false;
        REQUIRE_NOTHROW(manager.Subscribe("test_event", [&called]()
            {
                called = true;
            }));

        manager.Send("test_event");

        REQUIRE(called == true);
    }

    SECTION("Trigger with arguments") {
        EventManager manager;

        bool called = false;

        int i_value = 100;
        const char* c_value = "events are fun!";

        REQUIRE_NOTHROW(manager.Subscribe<int, const char*>("test_event", [&called, i_value, c_value](int i, const char* c)
            {
                REQUIRE(i == i_value);
                REQUIRE(c == c_value);

                called = true;
            }));

        REQUIRE_NOTHROW(manager.Send<int, const char*>("test_event", i_value, c_value));

        REQUIRE(called == true);
    }

    SECTION("Trigger arguments handled correctly")
    {
        EventManager manager;

        REQUIRE_NOTHROW(manager.Subscribe<int>("test_event", [](int)
            {

            }));

        REQUIRE_THROWS(manager.Send("test_event"));
        REQUIRE_THROWS(manager.Send<float, float>("test_event", 0.0f, 0.0f));
    }
}