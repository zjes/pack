#include <catch2/catch.hpp>
#include <pack/pack.h>

#include "examples/example.h"

TEST_CASE("Protobuf types")
{
    test::Person person;
    person.name = "dead"_s;
    person.id = 42;
    person.email = "parrot has email?"_s;
    person.binary.setString("dead parrot"_s);
    person.longId = 42;
    person.floatVal = 42.2f;
    person.doubleVal = 42.2;
    person.names = {"aaa"_s, "bbb"_s};
    person.ids = {1, 2, 3};

    auto check = [](const test::Person& tocheck)
    {
        CHECK(tocheck.name == "dead"_s);
        CHECK(tocheck.id == 42);
        CHECK(tocheck.email == "parrot has email?"_s);
        CHECK(tocheck.binary.asString() == "dead parrot"_s);
        CHECK(tocheck.longId == 42);
        CHECK(tocheck.floatVal == 42.2f);
        CHECK(tocheck.doubleVal == 42.2);
        CHECK(tocheck.names == pack::StringList({"aaa"_s, "bbb"_s}));
        CHECK(tocheck.ids == pack::Int32List({1, 2, 3}));
    };

    check(person);

    SECTION("json")
    {
        auto cnt = pack::json::serialize(person);
        REQUIRE(cnt);
        CHECK(!pack::isEmpty(*cnt));

        test::Person restored;
        REQUIRE(pack::json::deserialize(*cnt, restored));

        check(restored);
    }

    SECTION("jaml")
    {
        auto cnt = pack::yaml::serialize(person);
        REQUIRE(cnt);
        CHECK(!pack::isEmpty(*cnt));

        test::Person restored;
        REQUIRE(pack::yaml::deserialize(*cnt, restored));

        check(restored);
    }

    SECTION("protobuf")
    {
        auto cnt = pack::protobuf::serialize(person);
        REQUIRE(cnt);
        CHECK(!pack::isEmpty(*cnt));

        test::Person restored;
        REQUIRE(pack::protobuf::deserialize(*cnt, restored));

        check(restored);
    }
}
