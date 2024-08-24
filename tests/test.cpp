#include <catch2/catch_test_macros.hpp>

TEST_CASE("unit test1", "[test1]")
{
    SECTION("s1")
    {
        bool b = true;
        REQUIRE(b == true);
    }
    SECTION("s2")
    {
        bool b = false;
        REQUIRE(b== false);

    }
}

TEST_CASE("unit test2", "[test2]")
{
    SECTION("s1")
    {
        bool b = true;
        REQUIRE(b == true);
    }
    SECTION("s2")
    {
        bool b = false;
        REQUIRE(b== false);

    }
}
