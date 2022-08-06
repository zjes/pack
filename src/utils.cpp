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
#include "pack/utils.h"

namespace pack {

// =========================================================================================================================================

std::string toStdString(const pack::string_t& str)
{
#ifdef WITH_QTSTRING
    auto data = str.toUtf8();
    return std::string(data.constData(), size_t(data.length()));
#else
    return str;
#endif
}

// =========================================================================================================================================

#ifdef WITH_QTSTRING
std::string toStdString(const QByteArray& str)
{
#ifdef WITH_QTSTRING
    return std::string(str.constData(), size_t(str.length()));
#else
    return str;
#endif
}
#endif

// =========================================================================================================================================

pack::string_t fromStdString(const std::string& str)
{
#ifdef WITH_QTSTRING
    return QString::fromUtf8(str.data(), int(str.size()));
#else
    return str;
#endif
}

// =========================================================================================================================================

std::string tolower(const std::string& src)
{
    std::string val = src;
    std::locale locale;
    auto        toLower = [&locale](char ch) {
        return std::use_facet<std::ctype<char>>(locale).tolower(ch);
    };

    std::transform(val.begin(), val.end(), val.begin(), toLower);
    return val;
}

// =========================================================================================================================================

bool isEmpty(const pack::string_t& str)
{
#ifdef WITH_QTSTRING
    return str.isEmpty();
#else
    return str.empty();
#endif
}

// =========================================================================================================================================

#ifdef WITH_QTSTRING
bool isEmpty(const QByteArray& str)
{
    return str.isEmpty();
}
#endif

// =========================================================================================================================================

}
