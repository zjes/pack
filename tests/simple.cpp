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

namespace simple {

class Person : public pack::Node
{
public:
    using pack::Node::Node;

public:
    pack::String name  = FIELD("name");
    pack::Int32  id    = FIELD("id");
    pack::String email = FIELD("email");

    META(Person, name, id, email);
};

} // namespace simple

TEST_CASE("Simple serialization/deserialization")
{
    simple::Person origin;
    origin.id    = 42;
    origin.name  = "Person"_s;
    origin.email = "person@email.org"_s;

    auto check = [](const simple::Person& item) {
        REQUIRE(42 == item.id);
        REQUIRE("Person"_s == item.name);
        REQUIRE("person@email.org"_s == item.email);
    };

    check(origin);

    SECTION("Serialization yaml")
    {
        auto cnt = pack::yaml::serialize(origin);
        REQUIRE(cnt);
        CHECK(!pack::isEmpty(*cnt));

        simple::Person restored;
        REQUIRE(pack::yaml::deserialize(*cnt, restored));

        check(restored);
    }

    SECTION("Serialization json")
    {
        auto cnt = pack::json::serialize(origin);
        REQUIRE(cnt);
        CHECK(!pack::isEmpty(*cnt));

        simple::Person restored;
        REQUIRE(pack::json::deserialize(*cnt, restored));

        check(restored);
    }
}

TEST_CASE("UTF-8 test")
{
    simple::Person origin;
    origin.id    = 42;
    origin.name  = "Персона"_s;
    origin.email = "成語"_s;

    auto check = [](const simple::Person& item) {
        REQUIRE(42 == item.id);
        REQUIRE("成語"_s == item.email);
        REQUIRE("Персона"_s == item.name);
    };

    check(origin);

    SECTION("Serialization yaml")
    {
        auto cnt = pack::yaml::serialize(origin);
        REQUIRE(cnt);
        CHECK(!pack::isEmpty(*cnt));

        simple::Person restored;
        REQUIRE(pack::yaml::deserialize(*cnt, restored));

        check(restored);
    }

    SECTION("Serialization json")
    {
        auto cnt = pack::json::serialize(origin);
        REQUIRE(cnt);
        CHECK(!pack::isEmpty(*cnt));

        simple::Person restored;
        REQUIRE(pack::json::deserialize(*cnt, restored));

        check(restored);
    }
}

struct Data : public pack::Node
{
    pack::Bool  value  = FIELD("value");
    pack::Int32 ivalue = FIELD("ivalue");

    using pack::Node::Node;
    META(Data, value, ivalue);
};

TEST_CASE("Null values")
{
    Data data;
    auto ret = pack::json::deserialize(R"({"value":null, "ivalue":null})"_s, data);
    REQUIRE(ret);
    CHECK(data.value == false);
    CHECK(data.ivalue == 0);
}

TEST_CASE("Node copy/move")
{
    simple::Person origin;
    origin.email = "person@email.org"_s;
    origin.id    = 42;
    origin.name  = "Person"_s;

    {
        simple::Person other = origin;
        CHECK(other.email == origin.email);
        CHECK(other.id == origin.id);
        CHECK(other.name == origin.name);

        CHECK(other == origin);
        CHECK(other.compare(origin));
    }
    {
        simple::Person other = std::move(origin);
        CHECK(other.email == "person@email.org"_s);
        CHECK(other.id == 42);
        CHECK(other.name == "Person"_s);

        CHECK(other == origin);
        CHECK(other.compare(origin));
    }
}
