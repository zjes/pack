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
#include "pack/convert.h"
#include <any>

namespace pack {

// =========================================================================================================================================

struct FieldOption
{
};

// =========================================================================================================================================

struct Key : public FieldOption
{
    explicit Key(const string_t& key)
        : value(key)
    {
    }

    string_t value;
};

// =========================================================================================================================================

struct Default : public FieldOption
{
    template <typename T>
    explicit Default(const T& def)
        : value(UseType<T>(def))
    {
    }

    template <typename T>
    T get() const
    {
        if (value.type() == typeid(int32_t)) {
            return convert<T>(std::any_cast<int32_t>(value));
        } else if (value.type() == typeid(int64_t)) {
            return convert<T>(std::any_cast<int64_t>(value));
        } else if (value.type() == typeid(uint32_t)) {
            return convert<T>(std::any_cast<uint32_t>(value));
        } else if (value.type() == typeid(uint64_t)) {
            return convert<T>(std::any_cast<uint64_t>(value));
        } else if (value.type() == typeid(float)) {
            return convert<T>(std::any_cast<float>(value));
        } else if (value.type() == typeid(double)) {
            return convert<T>(std::any_cast<double>(value));
        } else if (value.type() == typeid(bool)) {
            return convert<T>(std::any_cast<bool>(value));
        } else if (value.type() == typeid(const char*)) {
            return convert<T>(std::any_cast<const char*>(value));
        } else if (value.type() == typeid(string_t)) {
            return convert<T>(std::any_cast<string_t>(value));
        }
        return convert<T>(std::any_cast<T>(value));
    }

    std::any value;
};

// =========================================================================================================================================

template <typename... Args>
constexpr auto allIsOptions()
{
    return (std::is_base_of<FieldOption, Args>::value && ...);
}

template <>
constexpr auto allIsOptions()
{
    return true;
}

// =========================================================================================================================================

template <typename... Options>
using isOptions = std::enable_if_t<allIsOptions<Options...>()>;

// =========================================================================================================================================

template <typename T, typename... Options>
constexpr const T* pickOption(const Options&... opts)
{
    const T* ret = nullptr;
    (
        [&](const auto& opt) {
            if constexpr (std::is_same_v<std::decay_t<decltype(opt)>, std::decay_t<T>>) {
                ret = &opt;
            }
        }(opts),
        ...);

    return ret;
}

// =========================================================================================================================================

} // namespace pack
