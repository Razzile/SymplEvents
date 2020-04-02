#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "delegate.h"

using symplevents::Delegate;

TEST_CASE("Delegate listeners can be registered", "[delegate]")
{
    Delegate delegate;

    REQUIRE(delegate.listeners() == 0);

    REQUIRE_NOTHROW(delegate.AddListener([]()
        {

        }));

    REQUIRE(delegate.listeners() == 1);
}

TEST_CASE("Delegate listeners can be cleared", "[delegate]")
{
    Delegate delegate;

    REQUIRE(delegate.listeners() == 0);

    REQUIRE_NOTHROW(delegate.AddListener([]()
        {

        }));

    REQUIRE(delegate.listeners() == 1);

    REQUIRE_NOTHROW(delegate.RemoveAllListeners());

    REQUIRE(delegate.listeners() == 0);
}

TEST_CASE("Delegate listeners can be called", "[delegate]")
{
    SECTION("A parameterless delegate is created and called") {
        Delegate delegate;

        bool called = false;

        REQUIRE_NOTHROW(delegate.AddListener([&called]()
            {
                called = true;
            }));

        delegate();

        REQUIRE(called == true);
    }

    SECTION("A variadic delegate is created and called")
    {
        Delegate<int, const char*> delegate;

        bool called = false;

        int i_value = 100;
        const char* c_value = "delegates are fun!";

        REQUIRE_NOTHROW(delegate.AddListener([&called, i_value, c_value](int i, const char* c)
            {
                REQUIRE(i == i_value);
                REQUIRE(c == c_value);

                called = true;
            }));

        delegate(i_value, c_value);

        REQUIRE(called == true);
    }
}
