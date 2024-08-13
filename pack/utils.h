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

#include <pack/types.h>
#include <pack/ustring.h>
#include <tl/expected.hpp>
#ifdef WITH_QT
#include <QString>
#endif

namespace pack {

class UString;
template <typename T>
using expected = tl::expected<T, UString>;

class unexpected : public tl::unexpected<UString>
{
public:
    using tl::unexpected<UString>::unexpected;

    template <typename... Args>
    unexpected(UString&& str, Args&&... args)
        : tl::unexpected<UString>(std::vformat(str.toStdString(), std::make_format_args(args...)))
    {
    }

    template <typename... Args>
    unexpected(std::string&& str, Args&&... args)
        : tl::unexpected<UString>(std::vformat(str, std::make_format_args(args...)))
    {
    }

    template <typename... Args>
    unexpected(const char* str, Args&&... args)
        : tl::unexpected<UString>(std::vformat(str, std::make_format_args(args...)))
    {
    }

#ifdef WITH_QT
    template <typename... Args>
    unexpected(QString&& str, Args&&... args)
        : tl::unexpected<UString>(std::vformat(str.toStdString(), std::make_format_args(args...)))
    {
    }
#endif
};

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

template <typename Base, typename T>
concept isSubtype = std::is_base_of_v<Base, T>;

// =========================================================================================================================================

template <typename T, typename CppType>
concept isConvertable = std::convertible_to<UseType<T>, CppType>;

// =========================================================================================================================================

template <class...>
constexpr std::false_type always_false{};

// =========================================================================================================================================

template <typename T>
concept enumerable = std::is_enum_v<T>;

template <typename T>
concept enumerableToStream = enumerable<T> && requires(std::ostream os, T value) {
    { os << value };
};

template <typename T>
concept enumerableFromStream = enumerable<T> && requires(std::istream os, T value) {
    { os >> value };
};

// =========================================================================================================================================

} // namespace pack

template <typename T>
std::ostream& operator<<(std::ostream& os, const tl::expected<T, pack::UString>& /*value*/)
{
    // os << value.value_or();
    return os;
}
