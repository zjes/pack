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

#include "pack/types.h"
#include <algorithm>
#include <fmt/format.h>
#include <string>
#include <type_traits>
#ifdef WITH_QSTRING
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
    using Type = pack::string_t;
};

template <>
struct Types<const char*>
{
    using Type = pack::string_t;
};

// =========================================================================================================================================

template <typename T>
using UseType = typename Types<T>::Type;

// =========================================================================================================================================

template <typename CppType, typename T>
using isSame = std::is_same<UseType<std::decay_t<T>>, CppType>;

// =========================================================================================================================================

template <typename CppType, typename T>
using isConvertable = std::is_convertible<UseType<std::decay_t<T>>, CppType>;

// =========================================================================================================================================

template <class...>
constexpr std::false_type always_false{};

// =========================================================================================================================================

#define ENABLE_FLAGS(EnumType)                                                                                                             \
    constexpr EnumType operator~(EnumType rhs) noexcept                                                                                    \
    {                                                                                                                                      \
        return static_cast<EnumType>(~static_cast<std::underlying_type_t<EnumType>>(rhs));                                                 \
    }                                                                                                                                      \
    constexpr auto operator|(EnumType lhs, EnumType rhs) noexcept                                                                          \
    {                                                                                                                                      \
        return static_cast<EnumType>(                                                                                                      \
            static_cast<std::underlying_type_t<EnumType>>(lhs) | static_cast<std::underlying_type_t<EnumType>>(rhs));                      \
    }                                                                                                                                      \
    constexpr auto operator&(EnumType lhs, EnumType rhs) noexcept                                                                          \
    {                                                                                                                                      \
        return static_cast<EnumType>(                                                                                                      \
            static_cast<std::underlying_type_t<EnumType>>(lhs) & static_cast<std::underlying_type_t<EnumType>>(rhs));                      \
    }                                                                                                                                      \
    constexpr auto operator^(EnumType lhs, EnumType rhs) noexcept                                                                          \
    {                                                                                                                                      \
        return static_cast<EnumType>(                                                                                                      \
            static_cast<std::underlying_type_t<EnumType>>(lhs) ^ static_cast<std::underlying_type_t<EnumType>>(rhs));                      \
    }                                                                                                                                      \
    constexpr auto operator|=(EnumType& lhs, EnumType rhs) noexcept                                                                        \
    {                                                                                                                                      \
        return lhs = lhs | rhs;                                                                                                            \
    }                                                                                                                                      \
    constexpr auto operator&=(EnumType& lhs, EnumType rhs) noexcept                                                                        \
    {                                                                                                                                      \
        return lhs = lhs & rhs;                                                                                                            \
    }                                                                                                                                      \
    constexpr auto operator^=(EnumType& lhs, EnumType rhs) noexcept                                                                        \
    {                                                                                                                                      \
        return lhs = lhs ^ rhs;                                                                                                            \
    }

template <typename T>
bool isSet(T flag, T option)
{
    return std::underlying_type_t<T>(flag & option);
}

// =========================================================================================================================================

std::string toStdString(const pack::string_t& str);
#ifdef WITH_QTSTRING
std::string toStdString(const QByteArray& str);
#endif
pack::string_t fromStdString(const std::string& str);
std::string    tolower(const std::string& src);

// =========================================================================================================================================

bool isEmpty(const pack::string_t& str);
#ifdef WITH_QTSTRING
bool isEmpty(const QByteArray& str);
#endif

} // namespace pack
