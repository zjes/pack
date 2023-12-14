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

#include "pack/convert.h"
#include <catch2/catch.hpp>
#include <iostream>

using namespace Catch::literals;

enum class Test
{
    One = 1,
    Two = 2
};

enum class Parrot
{
    Blue  = 1,
    Green = 2
};

std::ostream& operator<<(std::ostream& ss, Test value)
{
    switch (value) {
        case Test::One:
            ss << "TOne";
            break;
        case Test::Two:
            ss << "TTwo";
            break;
    }
    return ss;
}

std::istream& operator>>(std::istream& ss, Test& value)
{
    std::string str;
    ss >> str;
    if (str == "TOne") {
        value = Test::One;
    } else if (str == "TTwo") {
        value = Test::Two;
    }
    return ss;
}

TEST_CASE("Convert")
{
    CHECK("11"_s == pack::convert<pack::UString>(11));
    CHECK("22.22"_s == pack::convert<pack::UString>(22.22));
    CHECK("22.22"_s == pack::convert<pack::UString>(22.22f));
    CHECK("true"_s == pack::convert<pack::UString>(true));
    CHECK("false"_s == pack::convert<pack::UString>(false));
    CHECK("11"_s == pack::convert<pack::UString>(11ll));
    CHECK("11"_s == pack::convert<pack::UString>(11ul));
    CHECK("str"_s == pack::convert<pack::UString>("str"));

    CHECK(0 == pack::convert<int8_t>("0"));
    CHECK(-1 == pack::convert<int8_t>("-1"));
    CHECK(44 == pack::convert<int8_t>(44.44f));
    CHECK(44 == pack::convert<int8_t>(44.44));
    CHECK(0 == pack::convert<int8_t>(false));
    CHECK(1 == pack::convert<int8_t>(true));

    CHECK(0 == pack::convert<int16_t>("0"));
    CHECK(-1 == pack::convert<int16_t>("-1"));
    CHECK(44 == pack::convert<int16_t>(44.44f));
    CHECK(44 == pack::convert<int16_t>(44.44));
    CHECK(0 == pack::convert<int16_t>(false));
    CHECK(1 == pack::convert<int16_t>(true));

    CHECK(0 == pack::convert<int32_t>("0"));
    CHECK(-1 == pack::convert<int32_t>("-1"));
    CHECK(44 == pack::convert<int32_t>(44.44f));
    CHECK(44 == pack::convert<int32_t>(44.44));
    CHECK(0 == pack::convert<int32_t>(false));
    CHECK(1 == pack::convert<int32_t>(true));

    CHECK(0 == pack::convert<int64_t>("0"));
    CHECK(-1 == pack::convert<int64_t>("-1"));
    CHECK(44 == pack::convert<int64_t>(44.44f));
    CHECK(44 == pack::convert<int64_t>(44.44));
    CHECK(0 == pack::convert<int64_t>(false));
    CHECK(1 == pack::convert<int64_t>(true));

    CHECK(0 == pack::convert<uint8_t>("0"));
    CHECK(std::numeric_limits<uint8_t>::max() == pack::convert<uint8_t>("-1"));
    CHECK(44 == pack::convert<uint8_t>(44.44f));
    CHECK(44 == pack::convert<uint8_t>(44.44));
    CHECK(0 == pack::convert<uint8_t>(false));
    CHECK(1 == pack::convert<uint8_t>(true));

    CHECK(0 == pack::convert<uint16_t>("0"));
    CHECK(std::numeric_limits<uint16_t>::max() == pack::convert<uint16_t>("-1"));
    CHECK(44 == pack::convert<uint16_t>(44.44f));
    CHECK(44 == pack::convert<uint16_t>(44.44));
    CHECK(0 == pack::convert<uint16_t>(false));
    CHECK(1 == pack::convert<uint16_t>(true));

    CHECK(0 == pack::convert<uint32_t>("0"));
    CHECK(std::numeric_limits<uint32_t>::max() == pack::convert<uint32_t>("-1"));
    CHECK(44 == pack::convert<uint32_t>(44.44f));
    CHECK(44 == pack::convert<uint32_t>(44.44));
    CHECK(0 == pack::convert<uint32_t>(false));
    CHECK(1 == pack::convert<uint32_t>(true));

    CHECK(0 == pack::convert<uint64_t>("0"));
    CHECK(std::numeric_limits<uint64_t>::max() == pack::convert<uint64_t>("-1"));
    CHECK(44 == pack::convert<uint64_t>(44.44f));
    CHECK(44 == pack::convert<uint64_t>(44.44));
    CHECK(0 == pack::convert<uint64_t>(false));
    CHECK(1 == pack::convert<uint64_t>(true));

    CHECK(true == pack::convert<bool>(true));
    CHECK(false == pack::convert<bool>(false));
    CHECK(false == pack::convert<bool>(0));
    CHECK(false == pack::convert<bool>(0.f));
    CHECK(false == pack::convert<bool>("false"));
    CHECK(false == pack::convert<bool>("0"));
    CHECK(true == pack::convert<bool>(1));
    CHECK(true == pack::convert<bool>(1.f));
    CHECK(true == pack::convert<bool>("true"));
    CHECK(true == pack::convert<bool>("1"));

    CHECK(0.f == pack::convert<float>("0"));
    CHECK(42.42f == pack::convert<float>("42.42"));
    CHECK(44.44f == pack::convert<float>(44.44f));
    CHECK(44.44f == pack::convert<float>(44.44));
    CHECK(0.f == pack::convert<float>(false));
    CHECK(1.f == pack::convert<float>(true));


    CHECK(1 == pack::convert<int>(Test::One));
    CHECK(2 == pack::convert<int>(Test::Two));

    CHECK("TOne" == pack::convert<std::string>(Test::One));
    CHECK("TTwo" == pack::convert<std::string>(Test::Two));
    CHECK(1 == pack::convert<int32_t>(Test::One));
    CHECK(2 == pack::convert<int32_t>(Test::Two));

    CHECK("Blue" == pack::convert<std::string>(Parrot::Blue));
    CHECK("Green" == pack::convert<std::string>(Parrot::Green));
    CHECK(1 == pack::convert<int32_t>(Parrot::Blue));
    CHECK(2 == pack::convert<int32_t>(Parrot::Green));

    CHECK(Test::One == pack::convert<Test>("TOne"));
    CHECK(Test::One == pack::convert<Test>(1));
    CHECK(Test::Two == pack::convert<Test>("TTwo"));
    CHECK(Test::Two == pack::convert<Test>(2));

    CHECK(Parrot::Blue == pack::convert<Parrot>("Blue"));
    CHECK(Parrot::Blue == pack::convert<Parrot>(1));
    CHECK(Parrot::Green == pack::convert<Parrot>("Green"));
    CHECK(Parrot::Green == pack::convert<Parrot>(2));
}
