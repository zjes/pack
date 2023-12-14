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
#ifdef WITH_QT
#include <QHash>
#include <QMap>
#endif

TEST_CASE("map")
{
    {
        pack::Int32Map imap;
        CHECK(imap.empty());
    }

    {
        pack::Int32Map::MapType vec = {{"one"_s, 1}, {"two"_s, 2}};
        pack::Int32Map          imap(vec);
        CHECK(!imap.empty());
        CHECK(imap.size() == 2);
        CHECK(imap.keys() == std::vector<pack::UString>{"one"_s, "two"_s});
        CHECK(imap["one"_s] == 1);
        CHECK(imap["two"_s] == 2);
        CHECK_THROWS(imap["three"_s] == 3);
        CHECK(imap.contains("one"_s));
        CHECK(!imap.contains("three"_s));
        CHECK(imap.remove("one"_s));
        CHECK(!imap.contains("one"_s));
    }
#ifdef WITH_QT
    {
        QHash<QString, QString> hash = {{QStringLiteral("one"), QStringLiteral("val1")}, {QStringLiteral("second"), QStringLiteral("val2")}};
        pack::StringMap         smap(hash);

        CHECK(!smap.empty());
        CHECK(smap.size() == 2);
        CHECK(smap["one"_s] == "val1"_s);
        CHECK(smap["second"_s] == "val2"_s);
    }

    {
        QMap<QString, QString> map = {{QStringLiteral("one"), QStringLiteral("val1")}, {QStringLiteral("second"), QStringLiteral("val2")}};
        pack::StringMap        smap(map);

        CHECK(!smap.empty());
        CHECK(smap.size() == 2);
        CHECK(smap["one"_s] == "val1"_s);
        CHECK(smap["second"_s] == "val2"_s);
    }
#endif
    {
        std::map<pack::UString, pack::UString> map = {{"one"_s, "val1"_s}, {"second"_s, "val2"_s}};
        pack::StringMap                          smap(map);

        CHECK(!smap.empty());
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
