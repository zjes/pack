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

#include "pack/utils.h"
#include <sstream>
#include <string>
#include <type_traits>
#ifdef WITH_QSTRING
#include <QString>
#endif

namespace pack {
// =========================================================================================================================================

template <typename T, typename ValueT>
std::enable_if_t<std::is_same_v<T, std::string> || std::is_constructible_v<std::string, T>, std::string> convert(ValueT&& value)
{
    if constexpr (std::is_same_v<std::string, ValueT>) {
        return value;
    } else if constexpr (std::is_floating_point_v<ValueT>) {
        std::stringstream ss;
        ss << value;
        return ss.str();
    } else if constexpr (std::is_integral_v<ValueT> && !std::is_same_v<bool, ValueT>) {
        return std::to_string(value);
    } else if constexpr (std::is_same_v<bool, ValueT>) {
        return value ? std::string{"true"} : std::string{"false"};
    } else if constexpr (std::is_constructible_v<std::string, ValueT> && !std::is_same_v<std::string, ValueT>) {
        return std::string{value};
#ifdef WITH_QTSTRING
    } else if constexpr (std::is_same_v<QString, ValueT>) {
        return value.toStdString();
#endif
    } else {
        static_assert(always_false<ValueT>, "Unsupported type to cast into std::string");
    }
}

// =========================================================================================================================================

template <typename T, typename ValueT>
std::enable_if_t<std::is_same_v<T, int8_t>, int8_t> convert(ValueT&& value)
{
    if constexpr (std::is_same_v<int8_t, ValueT>) {
        return value;
    } else if constexpr (std::is_constructible_v<std::string, ValueT>) {
        return type(std::stoi(value));
#ifdef WITH_QTSTRING
    } else if constexpr (std::is_same_v<QString, ValueT>) {
        return type(value.toShort());
#endif
    } else if constexpr (std::is_integral_v<ValueT> || std::is_floating_point_v<ValueT>) {
        return static_cast<int8_t>(value);
    } else {
        static_assert(always_false<ValueT>, "Unsupported type to cast into int8_t");
    }
}

// =========================================================================================================================================

template <typename T, typename ValueT>
std::enable_if_t<std::is_same_v<T, uint8_t>, uint8_t> convert(ValueT&& value)
{
    if constexpr (std::is_same_v<uint8_t, ValueT>) {
        return value;
    } else if constexpr (std::is_constructible_v<std::string, ValueT>) {
        return uint8_t(std::stoul(value));
#ifdef WITH_QTSTRING
    } else if constexpr (std::is_same_v<QString, ValueT>) {
        return uint8_t(value.toUShort());
#endif
    } else if constexpr (std::is_integral_v<ValueT> || std::is_floating_point_v<ValueT>) {
        return static_cast<uint8_t>(value);
    } else {
        static_assert(always_false<ValueT>, "Unsupported type to cast into uint8_t");
    }
}

// =========================================================================================================================================

template <typename T, typename ValueT>
std::enable_if_t<std::is_same_v<T, int16_t>, int16_t> convert(ValueT&& value)
{
    if constexpr (std::is_same_v<int16_t, ValueT>) {
        return value;
    } else if constexpr (std::is_constructible_v<std::string, ValueT>) {
        return int16_t(std::stoi(value));
#ifdef WITH_QTSTRING
    } else if constexpr (std::is_same_v<QString, ValueT>) {
        return int16_t(value.toInt());
#endif
    } else if constexpr (std::is_integral_v<ValueT> || std::is_floating_point_v<ValueT>) {
        return static_cast<int16_t>(value);
    } else {
        static_assert(always_false<ValueT>, "Unsupported type to cast into int16_t");
    }
}

// =========================================================================================================================================

template <typename T, typename ValueT>
std::enable_if_t<std::is_same_v<T, uint16_t>, uint16_t> convert(ValueT&& value)
{
    if constexpr (std::is_same_v<uint16_t, ValueT>) {
        return value;
    } else if constexpr (std::is_constructible_v<std::string, ValueT>) {
        return uint16_t(std::stoul(value));
#ifdef WITH_QTSTRING
    } else if constexpr (std::is_same_v<QString, ValueT>) {
        return uint16_t(value.toUInt());
#endif
    } else if constexpr (std::is_integral_v<ValueT> || std::is_floating_point_v<ValueT>) {
        return static_cast<uint16_t>(value);
    } else {
        static_assert(always_false<ValueT>, "Unsupported type to cast into uint16_t");
    }
}

// =========================================================================================================================================

template <typename T, typename ValueT>
std::enable_if_t<std::is_same_v<T, int32_t>, int32_t> convert(ValueT&& value)
{
    if constexpr (std::is_same_v<int32_t, ValueT>) {
        return int32_t(value);
    } else if constexpr (std::is_constructible_v<std::string, ValueT>) {
        return int32_t(std::stol(value));
#ifdef WITH_QTSTRING
    } else if constexpr (std::is_same_v<QString, ValueT>) {
        return int32_t(value.toLong());
#endif
    } else if constexpr (std::is_integral_v<ValueT> || std::is_floating_point_v<ValueT>) {
        return static_cast<int32_t>(value);
    } else {
        static_assert(always_false<ValueT>, "Unsupported type to cast into int32_t");
    }
}

// =========================================================================================================================================

template <typename T, typename ValueT>
std::enable_if_t<std::is_same_v<T, uint32_t>, uint32_t> convert(ValueT&& value)
{
    if constexpr (std::is_same_v<uint32_t, ValueT>) {
        return value;
    } else if constexpr (std::is_constructible_v<std::string, ValueT>) {
        return uint32_t(std::stoul(value));
#ifdef WITH_QTSTRING
    } else if constexpr (std::is_same_v<QString, ValueT>) {
        return uint32_t(value.toULong());
#endif
    } else if constexpr (std::is_integral_v<ValueT> || std::is_floating_point_v<ValueT>) {
        return static_cast<uint32_t>(value);
    } else {
        static_assert(always_false<ValueT>, "Unsupported type to cast into uint32_t");
    }
}

// =========================================================================================================================================

template <typename T, typename ValueT>
std::enable_if_t<std::is_same_v<T, int64_t>, int64_t> convert(ValueT&& value)
{
    if constexpr (std::is_same_v<int64_t, ValueT>) {
        return value;
    } else if constexpr (std::is_constructible_v<std::string, ValueT>) {
        return int64_t(std::stoll(value));
#ifdef WITH_QTSTRING
    } else if constexpr (std::is_same_v<QString, ValueT>) {
        return int64_t(value.toLongLong());
#endif
    } else if constexpr (std::is_integral_v<ValueT> || std::is_floating_point_v<ValueT>) {
        return static_cast<int64_t>(value);
    } else {
        static_assert(always_false<ValueT>, "Unsupported type to cast into int64_t");
    }
}

// =========================================================================================================================================

template <typename T, typename ValueT>
std::enable_if_t<std::is_same_v<T, uint64_t>, uint64_t> convert(ValueT&& value)
{
    if constexpr (std::is_same_v<uint64_t, ValueT>) {
        return value;
    } else if constexpr (std::is_constructible_v<std::string, ValueT>) {
        return uint64_t(std::stoull(value));
#ifdef WITH_QTSTRING
    } else if constexpr (std::is_same_v<QString, ValueT>) {
        return uint64_t(value.toULongLong());
#endif
    } else if constexpr (std::is_integral_v<ValueT> || std::is_floating_point_v<ValueT>) {
        return static_cast<uint64_t>(value);
    } else {
        static_assert(always_false<ValueT>, "Unsupported type to cast into uint64_t");
    }
}

// =========================================================================================================================================

template <typename T, typename ValueT>
std::enable_if_t<std::is_same_v<T, float>, float> convert(ValueT&& value)
{
    if constexpr (std::is_same_v<float, ValueT>) {
        return value;
    } else if constexpr (std::is_constructible_v<std::string, ValueT>) {
        return std::stof(value);
#ifdef WITH_QTSTRING
    } else if constexpr (std::is_same_v<QString, ValueT>) {
        return float(value.toDouble());
#endif
    } else if constexpr (std::is_integral_v<ValueT> || std::is_floating_point_v<ValueT>) {
        return static_cast<float>(value);
    } else {
        static_assert(always_false<ValueT>, "Unsupported type to cast into float");
    }
}

// =========================================================================================================================================

template <typename T, typename ValueT>
std::enable_if_t<std::is_same_v<T, double>, double> convert(ValueT&& value)
{
    if constexpr (std::is_same_v<double, ValueT>) {
        return value;
    } else if constexpr (std::is_constructible_v<std::string, ValueT>) {
        return std::stod(value);
#ifdef WITH_QTSTRING
    } else if constexpr (std::is_same_v<QString, ValueT>) {
        return value.toDouble();
#endif
    } else if constexpr (std::is_integral_v<ValueT> || std::is_floating_point_v<ValueT>) {
        return static_cast<double>(value);
    } else {
        static_assert(always_false<ValueT>, "Unsupported type to cast into double");
    }
}

// =========================================================================================================================================

template <typename T, typename ValueT>
std::enable_if_t<std::is_same_v<T, bool>, bool> convert(ValueT&& value)
{
    if constexpr (std::is_same_v<bool, ValueT>) {
        return value;
    } else if constexpr (std::is_constructible_v<std::string, ValueT>) {
        auto lower = tolower(value);
        return lower == "true" || lower == "1" || lower == "on";
#ifdef WITH_QTSTRING
    } else if constexpr (std::is_same_v<QString, ValueT>) {
        auto lower = value.toLower();
        return lower == "true"_s || lower == "1"_s || lower == "on"_s;
#endif
    } else if constexpr (std::is_integral_v<ValueT>) {
        return value != 0;
    } else if constexpr (std::is_floating_point_v<ValueT>) {
        return value > ValueT(0.f) || value < ValueT(0.f);
    } else {
        static_assert(always_false<ValueT>, "Unsupported type to cast into bool");
    }
}

// =========================================================================================================================================

#ifdef WITH_QTSTRING
template <typename T, typename ValueT>
std::enable_if_t<std::is_same_v<T, QString>, QString> convert(ValueT&& value)
{
    if constexpr (std::is_same_v<QString, ValueT> || std::is_convertible_v<QString, ValueT>) {
        return value;
    } else if constexpr (std::is_constructible_v<std::string, ValueT>) {
        return QString::fromStdString(value);
    } else if constexpr (std::is_integral_v<ValueT> && !std::is_same_v<bool, ValueT>) {
        return QString::number(value);
    } else if constexpr (std::is_floating_point_v<ValueT>) {
        return QString::number(double(value));
    } else if constexpr (std::is_same_v<bool, ValueT>) {
        return value ? "true"_s : "false"_s;
    } else {
        static_assert(always_false<ValueT>, "Unsupported type to cast into QString");
    }
}
#endif

template <typename T, typename ValueT>
std::enable_if_t<std::is_same_v<T, std::vector<std::byte>>, std::vector<std::byte>> convert(ValueT&& value)
{
    if constexpr (std::is_same_v<std::vector<std::byte>, ValueT> || std::is_convertible_v<std::vector<std::byte>, ValueT>) {
        return value;
    } else if constexpr (std::is_constructible_v<std::string, ValueT>) {
        std::string data = value;
        auto        val  = reinterpret_cast<const std::byte*>(data.c_str());
        return std::vector<std::byte>(val, val + data.size());
#ifdef WITH_QTSTRING
    } else if constexpr (std::is_same_v<QString, ValueT>) {
        auto val = reinterpret_cast<const std::byte*>(value.data());
        return std::vector<std::byte>(val, val + value.size());
#endif
    } else if constexpr ((std::is_integral_v<ValueT> || std::is_floating_point_v<ValueT>)&&!std::is_same_v<bool, ValueT>) {
        auto val = reinterpret_cast<const std::byte*>(&value);
        return std::vector<std::byte>(val, val + sizeof(value));
    } else if constexpr (std::is_same_v<bool, ValueT>) {
        return std::vector<std::byte>(value ? 1 : 0);
    } else {
        static_assert(always_false<ValueT>, "Unsupported type to cast into std::vector<std::byte>");
    }
}

} // namespace pack
