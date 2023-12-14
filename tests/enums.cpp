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

namespace enums {

enum class Enum
{
    Dead,
    Can,
    Dance
};

enum class Enum2
{
    Blue,
    Parrot,
};

inline std::ostream& operator<<(std::ostream& ss, Enum2 value)
{
    ss << [&]() {
        switch (value) {
            case Enum2::Blue:
                return "BLUE";
            case Enum2::Parrot:
                return "PARROT";
        }
    }();
    return ss;
}

inline std::istream& operator>>(std::istream& ss, Enum2& value)
{
    std::string strval;
    ss >> strval;
    if (strval == "BLUE") {
        value = Enum2::Blue;
    } else if (strval == "PARROT") {
        value = Enum2::Parrot;
    } else {
        throw std::runtime_error("wrong value");
    }
    return ss;
}
} // namespace enums

TEST_CASE("Enum copy/move")
{
    {
        pack::Enum<enums::Enum> en(enums::Enum::Dead);
        auto                    en1 = en;
        REQUIRE(en1 == en);
        REQUIRE(en1.value() == en.value());
    }

    {
        pack::Enum<enums::Enum> en = FIELD("enum", pack::Default(enums::Enum::Can));
        en.setValue(enums::Enum::Dead);
        auto en1 = en;
        REQUIRE(en1 == en);
        REQUIRE(en1.value() == en.value());
        REQUIRE(en1.defValue() == en.defValue());
        REQUIRE(en.defValue() == enums::Enum::Can);
        REQUIRE(en1.defValue() == enums::Enum::Can);
        REQUIRE(en.value() == enums::Enum::Dead);
        REQUIRE(en1.value() == enums::Enum::Dead);
        REQUIRE(en1.key() == "enum"_s);
        REQUIRE(en.key() == "enum"_s);
    }
    {
        pack::Enum<enums::Enum> en = FIELD("enum", pack::Default(enums::Enum::Can));
        en.setValue(enums::Enum::Dead);
        auto en1 = std::move(en);

        REQUIRE(en1.defValue() == enums::Enum::Can);
        REQUIRE(en1.value() == enums::Enum::Dead);
        REQUIRE(en1.key() == "enum"_s);
    }
}

TEST_CASE("Enum methods")
{
    SECTION("from string")
    {
        {
            pack::Enum<enums::Enum> en;
            en.fromString("Dance"_s);
            REQUIRE(en == enums::Enum::Dance);
        }

        {
            pack::Enum<enums::Enum2> en;
            en.fromString("PARROT"_s);
            REQUIRE(en == enums::Enum2::Parrot);
        }

        {
            pack::Enum<enums::Enum> en{pack::Enum<enums::Enum>::Default(enums::Enum::Dance)};
            en.fromString("PARROT"_s);
            REQUIRE(en == enums::Enum::Dance); // Default value
        }

        {
            pack::Enum<enums::Enum2> en{pack::Default(enums::Enum2::Blue)};
            en.fromString("Dance"_s);
            REQUIRE(en == enums::Enum2::Blue); // Default value
        }
    }

    SECTION("to string")
    {
        {
            pack::Enum<enums::Enum> en = enums::Enum::Dance;
            REQUIRE(en.asString() == "Dance"_s);
        }

        {
            pack::Enum<enums::Enum2> en = enums::Enum2::Parrot;
            REQUIRE(en.asString() == "PARROT"_s);
        }
    }

    SECTION("from int")
    {
        {
            pack::Enum<enums::Enum> en;
            en.fromInt(int(enums::Enum::Dance));
            REQUIRE(en == enums::Enum::Dance);
        }

        {
            pack::Enum<enums::Enum2> en;
            en.fromInt(int(enums::Enum2::Parrot));
            REQUIRE(en == enums::Enum2::Parrot);
        }

        {
            pack::Enum<enums::Enum> en{pack::Enum<enums::Enum>::Default(enums::Enum::Dance)};
            en.fromInt(100);
            REQUIRE(en == enums::Enum::Dance); // Default value
        }

        {
            pack::Enum<enums::Enum2> en{pack::Default(enums::Enum2::Blue)};
            en.fromInt(100);
            REQUIRE(en == enums::Enum2::Blue); // Default value
        }
    }

    SECTION("to int")
    {
        {
            pack::Enum<enums::Enum> en = enums::Enum::Dance;
            REQUIRE(en.asInt() == 2);
        }

        {
            pack::Enum<enums::Enum2> en = enums::Enum2::Parrot;
            REQUIRE(en.asInt() == 1);
        }
    }

    SECTION("Values")
    {
        pack::Enum<enums::Enum> en;

        auto vals = en.values();
        REQUIRE(vals.size() == 3);
        REQUIRE(vals[0].first == "Dead"_s);
        REQUIRE(vals[0].second == 0);
        REQUIRE(vals[1].first == "Can"_s);
        REQUIRE(vals[1].second == 1);
        REQUIRE(vals[2].first == "Dance"_s);
        REQUIRE(vals[2].second == 2);
    }
}

TEST_CASE("Enum name")
{
    pack::Enum<enums::Enum> en;
    REQUIRE(en.typeName() == "Enum<enums::Enum>"_s);
    pack::Enum<enums::Enum2> en2;
    REQUIRE(en2.typeName() == "Enum<enums::Enum2>"_s);
}
