#include <catch2/catch.hpp>
#include <pack/pack.h>
#include <vector>

TEST_CASE("Binary")
{
    pack::Binary bin;
    bin.setString("parrot is dead"_s);
    CHECK(bin.asString() == "parrot is dead"_s);
    CHECK(bin.value()[0] == std::byte('p'));

    SECTION("json")
    {
        auto cnt = *pack::json::serialize(bin);
        CHECK(!pack::isEmpty(cnt));
        pack::Binary rebin;
        CHECK(pack::json::deserialize(cnt, rebin));
        CHECK(rebin.value()[0] == std::byte('p'));
        CHECK(rebin == bin);
    }

    SECTION("yaml")
    {
        auto cnt = *pack::yaml::serialize(bin);
        CHECK(!pack::isEmpty(cnt));
        pack::Binary rebin;
        CHECK(pack::yaml::deserialize(cnt, rebin));
        CHECK(rebin.value()[0] == std::byte('p'));
        CHECK(rebin == bin);
        CHECK(rebin.asString() == "parrot is dead"_s);
    }
}

namespace binary {

struct Test: public pack::Node
{
    pack::Binary bin = FIELD("binary");

    using pack::Node::Node;
    META(Test, bin);
};

}

TEST_CASE("Binary field")
{
    binary::Test test;
    test.bin.setString("dead parrot"_s);

    SECTION("json")
    {
        auto cnt = *pack::json::serialize(test);
        CHECK(!pack::isEmpty(cnt));
        binary::Test rebin;
        CHECK(pack::json::deserialize(cnt, rebin));
        CHECK(rebin.bin.value()[0] == std::byte('d'));
        CHECK(rebin == test);
    }

    SECTION("yaml")
    {
        auto cnt = *pack::yaml::serialize(test);
        CHECK(!pack::isEmpty(cnt));
        binary::Test rebin;
        CHECK(pack::yaml::deserialize(cnt, rebin));
        CHECK(rebin.bin.value()[0] == std::byte('d'));
        CHECK(rebin == test);
        CHECK(rebin.bin.asString() == "dead parrot"_s);
    }
}
