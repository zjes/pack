/* =========================================================================================================================================
    ____ __ _ ____ __  __
   |    |  ` |    |  /  /
   | |  | |  | |__|    /
   | ___| |  | |  |    \
   |_|  |__,_|____|__\__\ DSO library

   Copyright (C) 2020 Eaton
   Copyright (C) 2020-2022 zJes

   This program is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
========================================================================================================================================= */
#pragma once
#include <sstream>
#include <vector>
#ifdef WITH_QTSTRING
#include <QString>
#else
#include <string>
#endif

#ifdef WITH_QTSTRING
namespace pack {
using string_t = QString;
}
inline std::ostream& operator<<(std::ostream& ss, const QString& value)
{
    ss << qPrintable(value);
    return ss;
}
inline QString operator"" _s(const char* str, std::size_t size)
{
    return QLatin1String(str, int(size));
}
#else
namespace pack {
using string_t = std::string;
}
inline std::string operator"" _s(const char* str, std::size_t size)
{
    return std::string(str, size);
}
#endif

namespace pack {

// =========================================================================================================================================

/// Supported types
enum class Type
{
    Unknown,
    String,
    Binary,
    Int32,
    Int64,
    UInt32,
    UInt64,
    Float,
    Double,
    Bool,
    UChar
};

string_t      valueTypeName(Type type);
std::ostream& operator<<(std::ostream& ss, Type value);

// =========================================================================================================================================

template <Type>
struct ResolveType;

template <>
struct ResolveType<Type::String>
{
    using type = string_t;
};

template <>
struct ResolveType<Type::Binary>
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

template <>
struct ResolveType<Type::UChar>
{
    using type = unsigned char;
};

inline string_t valueTypeName(Type type)
{
    switch (type) {
        case Type::Unknown:
            return "Unknown"_s;
        case Type::String:
            return "String"_s;
        case Type::Binary:
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
        case Type::UChar:
            return "UChar"_s;
        case Type::UInt32:
            return "UInt32"_s;
        case Type::UInt64:
            return "UInt64"_s;
    }
    return "Unknown"_s;
}

inline std::ostream& operator<<(std::ostream& ss, Type value)
{
    ss << valueTypeName(value);
    return ss;
}

} // namespace pack
