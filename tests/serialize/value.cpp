#include "tests/catch2.h"
#include <pack/pack.h>


TEST_CASE("Serialize/Value YAML")
{
    {
        pack::Int32 int32 = 42;
        auto        res   = pack::serialize(pack::Serializer::Yaml, int32);
        CHECK(res);
        CHECK(*res == "42");
    }
    {
        pack::Int64 int64 = 42;
        auto        res   = pack::serialize(pack::Serializer::Yaml, int64);
        CHECK(res);
        CHECK(*res == "42");
    }
    {
        pack::Int32Map val = {{"v1"_s, 10}, {"v2"_s, 12}, {"v3"_s, 13}};
        auto           res = pack::serialize(pack::Serializer::Yaml, val);
        CHECK(res);
        CHECK(*res == "v1: 10\nv2: 12\nv3: 13");
    }
    {
        pack::Int32Map val = {{"v1", 10}, {"v2", 12}, {"v3", 13}};
        auto           res = pack::serialize(pack::Serializer::Yaml, val);
        CHECK(res);
        CHECK(*res == "v1: 10\nv2: 12\nv3: 13");
    }
    {
        pack::Int32List val = {10, 12, 13};
        auto            res = pack::serialize(pack::Serializer::Yaml, val);
        CHECK(res);
        CHECK(*res == "- 10\n- 12\n- 13");
    }
    {
        enum class En
        {
            val1,
            val2
        };
        pack::Enum<En> val = En::val2;
        auto           res = pack::serialize(pack::Serializer::Yaml, val);
        CHECK(res);
        CHECK(*res == "val2");
    }
    {
        pack::Binary val;
        val.setString("parrot"_s);
        auto res = pack::serialize(pack::Serializer::Yaml, val);
        CHECK(res);
        CHECK(*res == "cGFycm90");
    }
}

TEST_CASE("Serialize/Value JSON")
{
    {
        pack::Int32 int32 = 42;
        auto        res   = pack::serialize(pack::Serializer::Json, int32);
        CHECK(res);
        CHECK(*res == "42");
    }
    {
        pack::Int64 int64 = 42;
        auto        res   = pack::serialize(pack::Serializer::Json, int64);
        CHECK(res);
        CHECK(*res == "42");
    }
    {
        pack::Int32Map val = {{"v1"_s, 10}, {"v2"_s, 12}, {"v3"_s, 13}};
        auto           res = pack::serialize(pack::Serializer::Json, val);
        CHECK(res);
        CHECK(*res == R"({"v1":10,"v2":12,"v3":13})");
    }
    {
        pack::Int32Map val = {{"v1", 10}, {"v2", 12}, {"v3", 13}};
        auto           res = pack::serialize(pack::Serializer::Json, val);
        CHECK(res);
        CHECK(*res == R"({"v1":10,"v2":12,"v3":13})");
    }
    {
        pack::Int32List val = {10, 12, 13};
        auto            res = pack::serialize(pack::Serializer::Json, val);
        CHECK(res);
        CHECK(*res == "[10,12,13]");
    }
    {
        enum class En
        {
            val1,
            val2
        };
        pack::Enum<En> val = En::val2;
        auto           res = pack::serialize(pack::Serializer::Json, val);
        CHECK(res);
        CHECK(*res == R"("val2")");
    }
    {
        pack::Binary val;
        val.setString("parrot"_s);
        auto res = pack::serialize(pack::Serializer::Json, val);
        CHECK(res);
        CHECK(*res == "[112,97,114,114,111,116]");
    }
}

TEST_CASE("Deserialize/Value YAML")
{
    {
        pack::Int32 int32;
        auto        res = pack::deserialize(pack::Serializer::Yaml, "42", int32);
        CHECK(res);
        CHECK(int32 == 42);
    }
    {
        pack::Int64 int64;
        auto        res = pack::deserialize(pack::Serializer::Yaml, "42", int64);
        CHECK(res);
        CHECK(int64 == 42);
    }
    {
        pack::Int32Map val;
        auto           res = pack::deserialize(pack::Serializer::Yaml, "v1: 10\nv2: 12\nv3: 13", val);
        CHECK(res);
        REQUIRE(val.size() == 3);
        CHECK((val.contains("v1") && val["v1"] == 10));
        CHECK((val.contains("v2") && val["v2"] == 12));
        CHECK((val.contains("v3") && val["v3"] == 13));
    }
    {
        pack::Int32List val;
        auto            res = pack::deserialize(pack::Serializer::Yaml, "- 10\n- 12\n- 13", val);
        CHECK(res);
        REQUIRE(val.size() == 3);
        CHECK(val[0] == 10);
        CHECK(val[1] == 12);
        CHECK(val[2] == 13);
    }
    {
        enum class En
        {
            val1,
            val2
        };
        pack::Enum<En> val;
        auto           res = pack::deserialize(pack::Serializer::Yaml, "val2", val);
        CHECK(res);
        CHECK(val == En::val2);
    }
    {
        pack::Binary val;
        auto         res = pack::deserialize(pack::Serializer::Yaml, "cGFycm90", val);
        CHECK(res);
        CHECK(val.asString() == "parrot");
    }
}

TEST_CASE("Deserialize/Value JSON")
{
    {
        pack::Int32 int32;
        auto        res = pack::deserialize(pack::Serializer::Json, "42", int32);
        CHECK(res);
        CHECK(int32 == 42);
    }
    {
        pack::Int64 int64;
        auto        res = pack::deserialize(pack::Serializer::Json, "42", int64);
        CHECK(res);
        CHECK(int64 == 42);
    }
    {
        pack::Int32Map val;
        auto           res = pack::deserialize(pack::Serializer::Json, R"({"v1":10,"v2":12,"v3":13})", val);
        CHECK(res);
        REQUIRE(val.size() == 3);
        CHECK((val.contains("v1") && val["v1"] == 10));
        CHECK((val.contains("v2") && val["v2"] == 12));
        CHECK((val.contains("v3") && val["v3"] == 13));
    }
    {
        pack::Int32List val;
        auto            res = pack::deserialize(pack::Serializer::Json, "[10,12,13]", val);
        CHECK(res);
        REQUIRE(val.size() == 3);
        CHECK(val[0] == 10);
        CHECK(val[1] == 12);
        CHECK(val[2] == 13);
    }
    {
        enum class En
        {
            val1,
            val2
        };
        pack::Enum<En> val;
        auto           res = pack::deserialize(pack::Serializer::Json, R"("val2")", val);
        CHECK(res);
        CHECK(val == En::val2);
    }
    {
        pack::Binary val;
        auto         res = pack::deserialize(pack::Serializer::Json, "[112,97,114,114,111,116]", val);
        CHECK(res);
        CHECK(val.asString() == "parrot");
    }
}
