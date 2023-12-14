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

#include <fmt/format.h>
#include <pack/types.h>
#include <string>
#ifdef WITH_QT
#include <QString>
#endif

namespace pack {

// =========================================================================================================================================

template <typename T>
struct Types
{
    using Type = std::decay_t<T>;
};

template <size_t N>
struct Types<char[N]>
{
    using Type = UString;
};

template <>
struct Types<const char*>
{
    using Type = UString;
};

// =========================================================================================================================================

template <typename T>
using UseType = typename Types<T>::Type;

// =========================================================================================================================================

template <typename CppType, typename T>
concept isSame = std::same_as<UseType<T>, CppType>;

// =========================================================================================================================================

template <typename T, typename CppType>
concept isConvertable = std::convertible_to<UseType<T>, CppType>;

// =========================================================================================================================================

template <class...>
constexpr std::false_type always_false{};

// =========================================================================================================================================

template <typename T>
concept enumerable = std::is_enum<T>::value;

template <typename T>
concept enumerableToStream = enumerable<T> && requires(std::ostream os, T value) {
    {
        os << value
    };
};

template <typename T>
concept enumerableFromStream = enumerable<T> && requires(std::istream os, T value) {
    {
        os >> value
    };
};

// =========================================================================================================================================

} // namespace pack
