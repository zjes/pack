/* =========================================================================================================================================
    ____ __ _ ____ __  __
   |    |  ` |    |  /  /
   | |  | |  | |__|    /
   | ___| |  | |  |    \
   |_|  |__,_|____|__\__\ DSO library

Copyright (C) 2020-2022 zJes

This program is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
========================================================================================================================================= */
#include <catch2/catch.hpp>
#include <expected>
#include <pack/expected.h>

struct St
{
    St()                     = default;
    St(const St&)            = delete;
    St& operator=(const St&) = delete;
    St(St&&)                 = default;

    bool func()
    {
        return true;
    }
};

TEST_CASE("expected")
{
    SECTION("expected")
    {
        auto it = pack::expected<int>(32);
        CHECK(it);
        CHECK(32 == *it);
    }

    SECTION("Unexpected")
    {
        pack::expected<int> it = pack::unexpected("wrong"_s);
        CHECK(!it);
        CHECK("wrong" == it.error());
    }

    SECTION("Return values")
    {
        auto func = []() -> pack::expected<St> {
            return St();
        };

        auto func2 = []() -> pack::expected<St> {
            return pack::unexpected("wrong"_s);
        };

        pack::expected<St> st = func();
        CHECK(st);
        CHECK(st->func());
        CHECK((*st).func());

        pack::expected<St> ust = func2();
        CHECK(!ust);
        CHECK("wrong" == ust.error());
    }

    SECTION("Return streamed unexpected")
    {
        auto func = []() -> pack::expected<St> {
            auto str = pack::format("wrong {}"_s, 42);
            return pack::unexpected(str);
        };

        pack::expected<St> st = func();
        CHECK(!st);
        CHECK("wrong 42" == st.error());
    }

    SECTION("Void expected")
    {
        auto it = pack::expected<void>();
        CHECK(it);

        auto func = []() -> pack::expected<void> {
            return {};
        };
        auto func2 = []() -> pack::expected<void> {
            return pack::unexpected("some error"_s);
        };
        CHECK(func());
        auto res = func2();
        CHECK(!res);
        CHECK("some error" == res.error());
    }
}
