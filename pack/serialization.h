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

namespace pack {

enum class Option
{
    No           = 1 << 0,
    WithDefaults = 1 << 1,
    PrettyPrint  = 1 << 2
};

ENABLE_FLAGS(Option)

enum class Serializers
{
    Json,
    Yaml
};

expected<UString> serialize(Serializers serializer, const Attribute& node, Option opt = Option::No);
expected<void>    serializeFile(Serializers serializer, const UString& fileName, const Attribute& node, Option opt = Option::No);
expected<void>    deserialize(Serializers serializer, const UString& content, Attribute& node);
expected<void>    deserializeFile(Serializers serializer, const UString& fileName, Attribute& node);

namespace json {
    expected<UString> serialize(const Attribute& node, Option opt = Option::No);
    expected<void>    serializeFile(const UString& fileName, const Attribute& node, Option opt = Option::No);
    expected<void>    deserialize(const UString& content, Attribute& node);
    expected<void>    deserializeFile(const UString& fileName, Attribute& node);
} // namespace json

namespace yaml {
    expected<UString> serialize(const Attribute& node, Option opt = Option::No);
    expected<void>    serializeFile(const UString& fileName, const Attribute& node, Option opt = Option::No);
    expected<void>    deserialize(const UString& content, Attribute& node);
    expected<void>    deserializeFile(const UString& fileName, Attribute& node);
} // namespace yaml

} // namespace pack
