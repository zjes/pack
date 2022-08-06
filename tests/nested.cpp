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

namespace nested {

class Item : public pack::Node
{
public:
    class SubItem : public pack::Node
    {
    public:
        pack::Bool   exists = FIELD("exists");
        pack::String name   = FIELD("name");

        using pack::Node::Node;
        META(SubItem, exists, name);
    };

public:
    pack::String name = FIELD("name");
    SubItem      sub  = FIELD("sub");

    using pack::Node::Node;
    META(Item, name, sub);
};

}

TEST_CASE("Nested serialization/deserialization")
{
    nested::Item origin;
    origin.name       = "Item"_s;
    origin.sub.exists = true;
    origin.sub.name   = "subname"_s;

    auto check = [](const nested::Item& item) {
        REQUIRE("Item"_s == item.name);
        REQUIRE(true == item.sub.exists);
        REQUIRE("subname"_s == item.sub.name);
    };

    check(origin);

    SECTION("Serialization yaml")
    {
        auto cnt = pack::yaml::serialize(origin);
        REQUIRE(cnt);
        CHECK(!pack::isEmpty(*cnt));

        nested::Item restored;
        REQUIRE(pack::yaml::deserialize(*cnt, restored));

        check(restored);
    }

    SECTION("Serialization json")
    {
        auto cnt = pack::json::serialize(origin);
        REQUIRE(cnt);
        CHECK(!pack::isEmpty(*cnt));

        nested::Item restored;
        REQUIRE(pack::json::deserialize(*cnt, restored));

        check(restored);
    }
}
