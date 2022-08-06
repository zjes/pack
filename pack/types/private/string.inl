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

#include "pack/types/string.h"

namespace pack {

template <typename T, typename = std::enable_if_t<isSame<T, string_t>::value>>
inline bool operator==(const String& l, const T& r)
{
    return l.value() == r;
}

template <typename T, typename = std::enable_if_t<isSame<T, string_t>::value>>
inline bool operator==(const T& l, const String& r)
{
    return r.value() == l;
}

template <typename T, typename = std::enable_if_t<isSame<T, string_t>::value>>
inline bool operator!=(const String& l, const T& r)
{
    return l.value() != r;
}

template <typename T, typename = std::enable_if_t<isSame<T, string_t>::value>>
inline bool operator!=(const T& l, const String& r)
{
    return r.value() != l;
}

template <typename T, typename = std::enable_if_t<isSame<T, string_t>::value>>
inline T operator+(const T& l, const String& r)
{
    return l + r.value();
}

template <typename T, typename = std::enable_if_t<isSame<T, string_t>::value>>
inline String operator+(const String& l, const T& r)
{
    return l.value() + r;
}

inline bool String::empty() const
{
    return isEmpty(value());
}

inline int String::size() const
{
    return int(value().size());
}

inline std::ostream& operator<<(std::ostream& ss, const String& s)
{
    ss << s.value();
    return ss;
}

inline void String::operator+=(const String& other)
{
    setValue(value() + other.value());
}

inline void String::operator+=(typename String::ConstRefType other)
{
    setValue(value() + other);
}

} // namespace pack
