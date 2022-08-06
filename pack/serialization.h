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

#include "pack/attribute.h"
#include "pack/expected.h"
#include "pack/utils.h"
#include <string>

namespace pack {

enum class Option
{
    No           = 1 << 0,
    WithDefaults = 1 << 1,
    PrettyPrint  = 1 << 2
};

ENABLE_FLAGS(Option)

namespace json {
    Expected<string_t> serialize(const Attribute& node, Option opt = Option::No);
    Expected<void>     deserialize(const string_t& content, Attribute& node);
    Expected<void>     deserializeFile(const string_t& fileName, Attribute& node);
    Expected<void>     serializeFile(const string_t& fileName, const Attribute& node, Option opt = Option::No);
} // namespace json

namespace yaml {
    Expected<string_t> serialize(const Attribute& node, Option opt = Option::No);
    Expected<void>     deserialize(const string_t& content, Attribute& node);
    Expected<void>     deserializeFile(const string_t& fileName, Attribute& node);
    Expected<void>     serializeFile(const string_t& fileName, const Attribute& node, Option opt = Option::No);
} // namespace yaml

#ifdef WITH_PROTOBUF
namespace protobuf {
#ifdef WITH_QTSTRING
    Expected<QByteArray> serialize(const Attribute& node, Option opt = Option::No);
    Expected<void>       deserialize(const QByteArray& content, Attribute& node);
    Expected<void>       deserializeFile(const QString& fileName, Attribute& node);
#else
    Expected<std::string> serialize(const Attribute& node, Option opt = Option::No);
    Expected<void>        deserialize(const std::string& content, Attribute& node);
    Expected<void>        deserializeFile(const std::string& fileName, Attribute& node);
#endif
} // namespace protobuf
#endif

} // namespace pack
