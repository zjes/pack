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
#include <fmt/format.h>
#include <pack/attribute.h>
#include <pack/serialization.h>

// =========================================================================================================================================

/// Helper to format pack enity into fmt
template <typename T>
struct fmt::formatter<T, std::enable_if_t<std::is_base_of<pack::Attribute, T>::value, char>>
{
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const T& attr, FormatContext& ctx)
    {
        if (auto ret = pack::json::serialize(attr)) {
            return fmt::format_to(ctx.out(), "{}", *ret);
        } else {
            return fmt::format_to(ctx.out(), "#Error: {}", ret.error());
        }
    }
};

// =========================================================================================================================================

#ifdef WITH_QTSTRING
/// Helper to format QString enity into fmt
template <>
struct fmt::formatter<QString>
{
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const QString& attr, FormatContext& ctx)
    {
        return fmt::format_to(ctx.out(), "{}", attr.toStdString());
    }
};
#endif

// =========================================================================================================================================
