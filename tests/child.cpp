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
#include <iostream>
#include <pack/pack.h>

namespace child {

struct Parent : public pack::Node
{
    pack::String value = FIELD("value");
    pack::String field = FIELD("field");

    using pack::Node::Node;
    META(Parent, value, field);
};

struct Child : public Parent
{
    pack::String child = FIELD("child");

    using Parent::Parent;
    META_BASE(Child, Parent, child);
};

} // namespace child

TEST_CASE("Child class")
{
    child::Child origin;
    origin.field = "field"_s;
    origin.value = "value"_s;
    origin.child = "child"_s;

    auto check = [](const child::Child& item) {
        REQUIRE("value"_s == item.value);
        REQUIRE("field"_s == item.field);
        REQUIRE("child"_s == item.child);
    };

    check(origin);

    SECTION("Serialization yaml")
    {
        auto cnt = pack::yaml::serialize(origin);
        REQUIRE(cnt);
        CHECK(!pack::isEmpty(*cnt));

        child::Child restored;
        REQUIRE(pack::yaml::deserialize(*cnt, restored));

        check(restored);
    }

    SECTION("Serialization json")
    {
        auto cnt = pack::json::serialize(origin);
        REQUIRE(cnt);
        CHECK(!pack::isEmpty(*cnt));

        child::Child restored;
        REQUIRE(pack::json::deserialize(*cnt, restored));

        check(restored);
    }
}

TEST_CASE("Child copy/move")
{
    child::Child origin;
    origin.field = "field"_s;
    origin.value = "value"_s;
    origin.child = "child"_s;

    {
        child::Child other = origin;
        REQUIRE(other.field == origin.field);
        REQUIRE(other.value == origin.value);
        REQUIRE(other.child == origin.child);

        REQUIRE(other == origin);
        REQUIRE(other.compare(origin));
    }
    {
        child::Child other = std::move(origin);
        REQUIRE(other.field == "field"_s);
        REQUIRE(other.value == "value"_s);
        REQUIRE(other.child == "child"_s);
    }
}
