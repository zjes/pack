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
#include <pack/utils.h>

namespace pack {

inline int String::size() const
{
    return int(value().size());
}

inline std::ostream& operator<<(std::ostream& ss, const String& s)
{
    ss << s.value();
    return ss;
}

inline String String::operator+=(const String& other)
{
    setValue(value() + other.value());
    return *this;
}

inline String String::operator+=(typename String::ConstRefType other)
{
    setValue(value() + other);
    return *this;
}

inline String operator+(const UString& l, const String& r)
{
    UString str(l);
    str += r.value();
    return str;
}

inline String::operator UString() const
{
    return value();
}

#ifdef WITH_QT
inline QDebug operator<<(QDebug debug, const String &value)
{
    debug << value.value();
    return debug;
}
#endif

} // namespace pack
