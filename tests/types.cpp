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

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <pack/types.h>


TEST_CASE("Types")
{
    CHECK(pack::valueTypeName(pack::Type::Bytes) == "Binary"_s);
    CHECK(pack::valueTypeName(pack::Type::Bool) == "Bool"_s);
    CHECK(pack::valueTypeName(pack::Type::Double) == "Double"_s);
    CHECK(pack::valueTypeName(pack::Type::Float) == "Float"_s);
    CHECK(pack::valueTypeName(pack::Type::Int32) == "Int32"_s);
    CHECK(pack::valueTypeName(pack::Type::Int64) == "Int64"_s);
    CHECK(pack::valueTypeName(pack::Type::String) == "String"_s);
    CHECK(pack::valueTypeName(pack::Type::UInt32) == "UInt32"_s);
    CHECK(pack::valueTypeName(pack::Type::UInt64) == "UInt64"_s);
    CHECK(pack::valueTypeName(pack::Type::Unknown) == "Unknown"_s);

    CHECK(typeid(pack::ResolveType<pack::Type::String>::type) == typeid(pack::UString));
    CHECK(typeid(pack::ResolveType<pack::Type::Bool>::type) == typeid(bool));
    CHECK(typeid(pack::ResolveType<pack::Type::Bytes>::type) == typeid(std::vector<std::byte>));
    CHECK(typeid(pack::ResolveType<pack::Type::Double>::type) == typeid(double));
    CHECK(typeid(pack::ResolveType<pack::Type::Float>::type) == typeid(float));
    CHECK(typeid(pack::ResolveType<pack::Type::Int32>::type) == typeid(int32_t));
    CHECK(typeid(pack::ResolveType<pack::Type::Int64>::type) == typeid(int64_t));
    CHECK(typeid(pack::ResolveType<pack::Type::UInt32>::type) == typeid(uint32_t));
    CHECK(typeid(pack::ResolveType<pack::Type::UInt64>::type) == typeid(uint64_t));
}
