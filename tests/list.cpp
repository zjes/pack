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
#include <catch2/catch_all.hpp>
#include <pack/pack.h>

TEST_CASE("new list")
{
    pack::List<pack::Int32> ilist = {{10, 12}};
    REQUIRE(ilist.size() == 2);
    CHECK(ilist.type() == pack::Attribute::NodeType::List);

    SECTION("access")
    {
        auto v  = ilist[1];
        auto vv = v.value();
        REQUIRE(vv);
        std::cerr << v.typeName() << std::endl;
        if (v == 12) {
            std::cerr << "==" << std::endl;
        }
        if (ilist[1] == 12) {
            std::cerr << "==" << std::endl;
        }
        REQUIRE(ilist[1] == 12);
        REQUIRE_THROWS(ilist[2] == 12);
        REQUIRE_THROWS(ilist[-1] == 12);
    }

    SECTION("indexOf")
    {
        CHECK(ilist.indexOf(12) == 1);
        CHECK(ilist.indexOf(13) == -1);
        CHECK(ilist.indexOf([](const auto& val) {
            return val == 12;
        }) == 1);
        CHECK(ilist.indexOf([](const auto& val) {
            return val == 13;
        }) == -1);
    }

    SECTION("findItem")
    {
        CHECK(ilist.findItem([](const auto& val) {
            return val == 12;
        }) == 12);
        CHECK(ilist.findItem([](const auto& val) {
            return val == 13;
        }) == std::nullopt);
        CHECK(ilist.findItem(12) == 12);
        CHECK(ilist.findItem(13) == std::nullopt);
    }

    SECTION("find")
    {
        auto found = ilist.find([](const auto& val) {
            return val == 12;
        });
        REQUIRE(found != ilist.end());
        CHECK(*found == 12);
        found = ilist.find([](const auto& val) {
            return val == 13;
        });
        CHECK(found == ilist.end());
        found = ilist.find(12);
        REQUIRE(found != ilist.end());
        CHECK(*found == 12);
        found = ilist.find(13);
        CHECK(found == ilist.end());
    }

    SECTION("sort")
    {
        ilist.sort();
        CHECK(ilist[0] == 10);
        CHECK(ilist[1] == 12);
        ilist.sort([](const auto& a, const auto& b) {
            return a > b;
        });
        CHECK(ilist[0] == 12);
        CHECK(ilist[1] == 10);
    }

    SECTION("sorted")
    {
        auto copy = ilist.sorted();
        CHECK(copy[0] == 10);
        CHECK(copy[1] == 12);
        copy = ilist.sorted([](const auto& a, const auto& b) {
            return a > b;
        });
        CHECK(copy[0] == 12);
        CHECK(copy[1] == 10);
    }

    SECTION("remove")
    {
        CHECK(ilist.remove(12));
        CHECK(ilist.size() == 1);
        CHECK(ilist.remove([](const auto& it) {
            return it == 10;
        }));
        CHECK(ilist.empty());
    }

    SECTION("append")
    {
        decltype(ilist) list;
        list.append(42);
        REQUIRE(list.size() == 1);
        CHECK(list[0] == 42);

        list.append() = 43;
        REQUIRE(list.size() == 2);
        CHECK(list[0] == 42);
        CHECK(list[1] == 43);

        ilist.append(std::as_const(list));
        CHECK(ilist.size() == 4);

        ilist.append(std::move(list));
        CHECK(ilist.size() == 6);

        std::vector<pack::Int32> vec = {22, 33};
        ilist.append(std::as_const(vec));
        CHECK(ilist.size() == 8);

        ilist.append(std::move(vec));
        CHECK(ilist.size() == 10);

        std::vector<int> vec2 = {22, 33};
        ilist.append(std::as_const(vec2));
        CHECK(ilist.size() == 12);

        ilist.append(std::move(vec2));
        CHECK(ilist.size() == 14);

        ilist.append({15, 16});
        CHECK(ilist.size() == 16);
    }

    SECTION("compare")
    {
        auto list = ilist;
        CHECK(list.compare(ilist));

        decltype(ilist) list2;
        CHECK(!list2.compare(ilist));

        list.append(43);
        CHECK(!list.compare(ilist));
    }

    // SECTION("typeName")
    // {
    //     CHECK(ilist.typeName() == "List<Int32>"_s);
    //     CHECK(decltype(ilist)::_typeName() == "List<Int32>"_s);
    // }
}
