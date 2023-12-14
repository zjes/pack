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
#pragma once

#include <pack/ustring.h>
#include <sstream>

// =========================================================================================================================================

namespace pack {

/// Supported types
enum class Type
{
    Unknown,
    String,
    Bytes,
    Int32,
    Int64,
    UInt32,
    UInt64,
    Float,
    Double,
    Bool
};

// =========================================================================================================================================

template <Type>
struct ResolveType;

template <>
struct ResolveType<Type::String>
{
    using type = UString;
};

template <>
struct ResolveType<Type::Bytes>
{
    using type = std::vector<std::byte>;
};

template <>
struct ResolveType<Type::Int32>
{
    using type = int32_t;
};

template <>
struct ResolveType<Type::Int64>
{
    using type = int64_t;
};

template <>
struct ResolveType<Type::UInt32>
{
    using type = uint32_t;
};

template <>
struct ResolveType<Type::UInt64>
{
    using type = uint64_t;
};

template <>
struct ResolveType<Type::Float>
{
    using type = float;
};

template <>
struct ResolveType<Type::Double>
{
    using type = double;
};

template <>
struct ResolveType<Type::Bool>
{
    using type = bool;
};

UString valueTypeName(Type type);

} // namespace pack

std::ostream& operator<<(std::ostream& ss, pack::Type value);

inline pack::UString pack::valueTypeName(Type type)
{
    switch (type) {
        case Type::Unknown:
            return "Unknown"_s;
        case Type::String:
            return "String"_s;
        case Type::Bytes:
            return "Binary"_s;
        case Type::Bool:
            return "Bool"_s;
        case Type::Double:
            return "Double"_s;
        case Type::Float:
            return "Float"_s;
        case Type::Int32:
            return "Int32"_s;
        case Type::Int64:
            return "Int64"_s;
        case Type::UInt32:
            return "UInt32"_s;
        case Type::UInt64:
            return "UInt64"_s;
    }
    return "Unknown"_s;
}
