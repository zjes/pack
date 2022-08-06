/*  ========================================================================================================================================
    Copyright (C) 2020 Eaton
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    ========================================================================================================================================
*/
#include <catch2/catch.hpp>
#include <pack/pack.h>
#ifdef WITH_QTSTRING
#include <QHash>
#include <QMap>
#endif

struct TestMap : public pack::Node
{
    using pack::Node::Node;

    pack::StringMap strs = FIELD("strs");
    pack::Int32Map  ints = FIELD("ints");

    META(TestMap, strs, ints);
};

TEST_CASE("Simple map serialization/deserialization")
{
    TestMap origin;
    origin.strs.append("key1"_s, "some name1"_s);
    origin.strs.append("key2"_s, "some name2"_s);

    origin.ints.append("key1"_s, 12);
    origin.ints.append("key2"_s, 13);

    auto check = [](TestMap& item) {
        try {
            REQUIRE(2 == item.strs.size());
            REQUIRE(2 == item.ints.size());
            CHECK(item.strs.contains("key1"_s));
            CHECK(item.strs.contains("key2"_s));
            CHECK(item.ints.contains("key1"_s));
            CHECK(item.ints.contains("key2"_s));
            CHECK("some name1"_s == item.strs["key1"_s]);
            CHECK("some name2"_s == item.strs["key2"_s]);
            CHECK(12 == item.ints["key1"_s]);
            CHECK(13 == item.ints["key2"_s]);
        } catch (const std::exception& err) {
            FAIL(err.what());
        }
    };

    check(origin);

    SECTION("Serialization yaml")
    {
        auto cnt = *pack::yaml::serialize(origin);
        REQUIRE(!pack::isEmpty(cnt));

        TestMap restored;
        pack::yaml::deserialize(cnt, restored);

        check(restored);
    }

    SECTION("Serialization json")
    {
        auto cnt = *pack::json::serialize(origin);
        REQUIRE(!pack::isEmpty(cnt));

        TestMap restored;
        pack::json::deserialize(cnt, restored);

        check(restored);
    }
}

struct MapObj : public pack::Node
{
    pack::String value = FIELD("value");

    using pack::Node::Node;
    META(MapObj, value);
};

TEST_CASE("Object map serialization/deserialization")
{
    pack::Map<MapObj> origin;
    auto&             it = origin.append("key1"_s);
    it.value             = "Some string"_s;

    auto sav               = origin["key1"_s];
    origin["key1"_s].value = "Some string modified"_s;
    CHECK(origin["key1"_s].value == "Some string modified"_s);
    origin["key1"_s] = sav;
    CHECK(origin["key1"_s].value == "Some string"_s);

    MapObj ins;
    ins.value = "Some other value"_s;
    origin.append("key2"_s, ins);

    auto check = [](pack::Map<MapObj>& item) {
        try {
            REQUIRE(2 == item.size());
            CHECK(item.contains("key1"_s));
            CHECK(item.contains("key2"_s));
            CHECK("Some string"_s == item["key1"_s].value);
            CHECK("Some other value"_s == item["key2"_s].value);
        } catch (const std::exception& err) {
            FAIL(err.what());
        }
    };

    check(origin);

    {
        auto cnt = pack::yaml::serialize(origin);
        REQUIRE(cnt);
        pack::Map<MapObj> checked;
        REQUIRE(pack::yaml::deserialize(*cnt, checked));
        check(checked);
    }

    {
        auto cnt = pack::json::serialize(origin);
        REQUIRE(cnt);
        pack::Map<MapObj> checked;
        REQUIRE(pack::json::deserialize(*cnt, checked));
        check(checked);
    }
}

TEST_CASE("Value map serialization/deserialization")
{
    pack::StringMap origin;
    origin.append("key1"_s, "Some string"_s);

    auto sav         = origin["key1"_s];
    origin["key1"_s] = "Some string modified"_s;
    CHECK(origin["key1"_s] == "Some string modified"_s);
    origin["key1"_s] = sav;
    CHECK(origin["key1"_s] == "Some string"_s);

    origin.append("key2"_s, "Some other value"_s);

    auto check = [](pack::StringMap& item) {
        try {
            REQUIRE(2 == item.size());
            CHECK(item.contains("key1"_s));
            CHECK(item.contains("key2"_s));
            CHECK("Some string"_s == item["key1"_s]);
            CHECK("Some other value"_s == item["key2"_s]);
        } catch (const std::exception& err) {
            FAIL(err.what());
        }
    };

    check(origin);

    {
        auto cnt = pack::yaml::serialize(origin);
        REQUIRE(cnt);
        pack::StringMap checked;
        REQUIRE(pack::yaml::deserialize(*cnt, checked));
        check(checked);
    }

    {
        auto cnt = pack::json::serialize(origin);
        REQUIRE(cnt);
        pack::StringMap checked;
        REQUIRE(pack::json::deserialize(*cnt, checked));
        check(checked);
    }
}

TEST_CASE("map")
{
    {
        pack::Int32Map imap;
        CHECK(!imap.hasValue());
    }

    {
        pack::Int32Map::MapType vec = {{"one"_s, 1}, {"two"_s, 2}};
        pack::Int32Map          imap(vec);
        CHECK(imap.hasValue());
        CHECK(imap.size() == 2);
        CHECK(imap.keys() == std::vector<pack::string_t>{"one"_s, "two"_s});
        CHECK(imap["one"_s] == 1);
        CHECK(imap["two"_s] == 2);
        CHECK_THROWS(imap["three"_s] == 3);
        CHECK(imap.contains("one"_s));
        CHECK(!imap.contains("three"_s));
        CHECK(imap.remove("one"_s));
        CHECK(!imap.contains("one"_s));
    }
#ifdef WITH_QTSTRING
    {
        QHash<QString, QString> hash = {{"one"_s, "val1"_s}, {"second"_s, "val2"_s}};
        pack::StringMap         smap(hash);

        CHECK(smap.hasValue());
        CHECK(smap.size() == 2);
        CHECK(smap["one"_s] == "val1"_s);
        CHECK(smap["second"_s] == "val2"_s);
    }

    {
        QMap<QString, QString> map = {{"one"_s, "val1"_s}, {"second"_s, "val2"_s}};
        pack::StringMap        smap(map);

        CHECK(smap.hasValue());
        CHECK(smap.size() == 2);
        CHECK(smap["one"_s] == "val1"_s);
        CHECK(smap["second"_s] == "val2"_s);
    }
#endif
    {
        std::map<pack::string_t, pack::string_t> map = {{"one"_s, "val1"_s}, {"second"_s, "val2"_s}};
        pack::StringMap                          smap(map);

        CHECK(smap.hasValue());
        CHECK(smap.size() == 2);
        CHECK(smap["one"_s] == "val1"_s);
        CHECK(smap["second"_s] == "val2"_s);
    }

    pack::StringMap map;
    map.append("key"_s, "val"_s);
    CHECK(map.size() == 1);
    CHECK(map["key"_s] == "val"_s);

    map.append("key1"_s) = "val1"_s;
    CHECK(map.size() == 2);
    CHECK(map["key1"_s] == "val1"_s);

    CHECK(map.typeName() == "Map<String>"_s);
}
