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

#include "pack/types/binary.h"

namespace pack {

inline Binary::Binary()
    : Value<Type::Bytes>()
{
}

inline void Binary::setString(const UString& data)
{
    std::string copy = data.toStdString();

    auto val = reinterpret_cast<const std::byte*>(copy.data());
    setValue(std::vector<std::byte>(val, val + data.size()));
}

inline void Binary::setString(const char* data, size_t size)
{
    auto val = reinterpret_cast<const std::byte*>(data);
    setValue(std::vector<std::byte>(val, val + size));
}

inline UString Binary::asString() const
{
    auto val = reinterpret_cast<const char*>(value().data());
    return UString(std::string(val, value().size()));
}

inline Binary Binary::fromString(const UString& data)
{
    Binary bin;
    bin.setString(data);
    return bin;
}

} // namespace pack
