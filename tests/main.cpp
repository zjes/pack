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
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <pack/pack.h>

TEST_CASE("Convert")
{
    CHECK(42 == pack::convert<int>("42"_s));
    CHECK(42 == pack::convert<int>(42.1));

    SECTION("std::string")
    {
        CHECK("42" == pack::convert<std::string>(42));
        CHECK("42" == pack::convert<std::string>(42l));
        CHECK("42" == pack::convert<std::string>(42ul));
        CHECK("32" == pack::convert<std::string>(' '));
        CHECK("42.1" == pack::convert<std::string>(42.1));
        CHECK("42.1" == pack::convert<std::string>(42.1f));
        CHECK("true" == pack::convert<std::string>(true));
        CHECK("false" == pack::convert<std::string>(false));
#ifdef WITH_QTSTRING
        CHECK("parrot" == pack::convert<std::string>(QStringLiteral("parrot")));
#endif
    }

    SECTION("bool")
    {
#ifdef WITH_QTSTRING
        CHECK(true == pack::convert<bool>(QStringLiteral("true")));
        CHECK(false == pack::convert<bool>(QStringLiteral("false")));
        CHECK(true == pack::convert<bool>(QStringLiteral("TRUE")));
        CHECK(true == pack::convert<bool>(QStringLiteral("True")));
        CHECK(true == pack::convert<bool>(QStringLiteral("On")));
        CHECK(true == pack::convert<bool>(QStringLiteral("on")));
        CHECK(true == pack::convert<bool>(QStringLiteral("1")));
        CHECK(false == pack::convert<bool>(QStringLiteral("some other")));
#endif
        CHECK(true == pack::convert<bool>(std::string("true")));
        CHECK(false == pack::convert<bool>(std::string("false")));
        CHECK(true == pack::convert<bool>(std::string("TRUE")));
        CHECK(true == pack::convert<bool>(std::string("True")));
        CHECK(true == pack::convert<bool>(std::string("On")));
        CHECK(true == pack::convert<bool>(std::string("on")));
        CHECK(true == pack::convert<bool>(std::string("1")));
        CHECK(false == pack::convert<bool>(std::string("some other")));
        CHECK(true == pack::convert<bool>("true"));
        CHECK(true == pack::convert<bool>("TRUE"));
        CHECK(true == pack::convert<bool>("True"));
        CHECK(true == pack::convert<bool>("On"));
        CHECK(true == pack::convert<bool>("on"));
        CHECK(true == pack::convert<bool>("1"));
        CHECK(false == pack::convert<bool>("false"));
        CHECK(true == pack::convert<bool>(2));
        CHECK(true == pack::convert<bool>(1.1));
        CHECK(true == pack::convert<bool>(1.1f));
        CHECK(false == pack::convert<bool>(0.));
        CHECK(false == pack::convert<bool>(0.f));
        CHECK(false == pack::convert<bool>(0));
        CHECK(false == pack::convert<bool>(false));
        CHECK(true == pack::convert<bool>(true));
    }
}
