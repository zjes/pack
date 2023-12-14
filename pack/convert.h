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

#include <concepts>
#include <pack/magic-enum.h>
#include <pack/utils.h>
#include <sstream>
#include <string>
#ifdef WITH_QT
#include <QString>
#endif

namespace pack {

// =========================================================================================================================================

template <typename Return, typename Value>
Return convert(Value&& value);

template <typename Return, typename Value>
Return convert(Value&& value, const Return& def);

template <typename Return, typename Value>
concept canConvert = requires(Value value) { convert(value)->Return; };

// =========================================================================================================================================

template <typename T>
struct ConvertType
{
};

// =========================================================================================================================================

template <>
struct ConvertType<UString>
{
    static UString convert(std::convertible_to<UString> auto value)
    {
        return value;
    }
    static UString convert(std::floating_point auto value)
    {
        return UString::fromNumber(value);
    }
    static UString convert(std::integral auto value)
    {
        return UString::fromNumber(value);
    }
    static UString convert(bool value)
    {
        return value ? "true"_s : "false"_s;
    }
    static UString convert(enumerableToStream auto value)
    {
        std::stringstream ss;
        ss << value;
        return UString(ss.str());
    }
    static UString convert(enumerable auto value)
    {
        return UString(magic_enum::enum_name(value).data());
    }
    static UString convert(const std::string& value)
    {
        return UString(value);
    }
#ifdef WITH_QT
    static UString convert(const QString& value)
    {
        return UString(value);
    }
#endif
};

// =========================================================================================================================================

template <>
struct ConvertType<std::string>
{
    static std::string convert(std::convertible_to<std::string> auto value)
    {
        return value;
    }
    static std::string convert(std::floating_point auto value)
    {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }
    static std::string convert(std::integral auto value)
    {
        return std::to_string(value);
    }
    static std::string convert(bool value)
    {
        return value ? std::string{"true"} : std::string{"false"};
    }
    static std::string convert(enumerableToStream auto value)
    {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }
    static std::string convert(enumerable auto value)
    {
        return magic_enum::enum_name(value).data();
    }
#ifdef WITH_QT
    static std::string convert(const QString& value)
    {
        return value.toStdString();
    }
#endif
};

// =========================================================================================================================================

#ifdef WITH_QT
template <>
struct ConvertType<QString>
{
    static QString convert(std::convertible_to<QString> auto value)
    {
        return value;
    }
    static QString convert(std::convertible_to<std::string> auto value)
    {
        return QString::fromStdString(value);
    }
    static QString convert(std::floating_point auto value)
    {
        return QString::number(value);
    }
    static QString convert(std::integral auto value)
    {
        return QString::number(value);
    }
    static QString convert(bool value)
    {
        return value ? QStringLiteral("true") : QStringLiteral("false");
    }
    static QString convert(enumerableToStream auto value)
    {
        std::stringstream ss;
        ss << value;
        return QString::fromStdString(ss.str());
    }
    static QString convert(enumerable auto value)
    {
        return QString::fromStdString(magic_enum::enum_name(value).data());
    }
};
#endif

// =========================================================================================================================================

template <typename T>
requires std::signed_integral<T>
struct ConvertType<T>
{
    static T convert(std::same_as<UString> auto value)
    {
        return value.template toNumber<T>();
    }
    static T convert(std::convertible_to<std::string> auto&& value)
    {
        if constexpr (std::is_same_v<T, int8_t> || std::is_same_v<T, int16_t>) {
            return static_cast<T>(std::stoi(value));
        } else if constexpr (std::is_same_v<T, int32_t>) {
            return static_cast<T>(std::stol(value));
        } else if constexpr (std::is_same_v<T, int64_t>) {
            return static_cast<T>(std::stoll(value));
        } else {
            static_assert(always_false<T>, "Unsupported type to cast into signed integral type");
        }
    }
    static T convert(std::floating_point auto value)
    {
        return static_cast<T>(value);
    }
    static T convert(std::integral auto value)
    {
        return static_cast<T>(value);
    }
    static T convert(enumerable auto value)
    {
        return static_cast<T>(value);
    }
    static T convert(bool value)
    {
        return value ? 1 : 0;
    }
#ifdef WITH_QT
    static int8_t convert(const QString& value)
    {
        if constexpr (std::is_same_v<T, int8_t> || std::is_same_v<T, int16_t>) {
            return static_cast<T>(value.toInt());
        } else if constexpr (std::is_same_v<T, int32_t>) {
            return static_cast<T>(value.toLong());
        } else if constexpr (std::is_same_v<T, int64_t>) {
            return static_cast<T>(value.toLongLong());
        } else {
            static_assert(always_false<T>, "Unsupported type to cast into signed integral type");
        }
    }
#endif
};

// =========================================================================================================================================

template <typename T>
requires std::unsigned_integral<T>
struct ConvertType<T>
{
    static T convert(std::same_as<UString> auto&& value)
    {
        return value.template toNumber<T>();
    }
    static T convert(std::convertible_to<std::string> auto&& value)
    {
        if constexpr (std::is_same_v<T, uint8_t> || std::is_same_v<T, uint16_t>) {
            return static_cast<T>(std::stoul(value));
        } else if constexpr (std::is_same_v<T, uint32_t>) {
            return static_cast<T>(std::stoul(value));
        } else if constexpr (std::is_same_v<T, uint64_t>) {
            return static_cast<T>(std::stoull(value));
        } else {
            static_assert(always_false<T>, "Unsupported type to cast into unsigned integral type");
        }
    }
    static T convert(std::floating_point auto&& value)
    {
        return static_cast<T>(value);
    }
    static T convert(std::integral auto&& value)
    {
        return static_cast<T>(value);
    }
    static T convert(bool&& value)
    {
        return value ? 1 : 0;
    }
#ifdef WITH_QT
    static int8_t convert(QString&& value)
    {
        if constexpr (std::is_same_v<T, int8_t> || std::is_same_v<T, int16_t>) {
            return static_cast<T>(value.toUInt());
        } else if constexpr (std::is_same_v<T, int32_t>) {
            return static_cast<T>(value.toULong());
        } else if constexpr (std::is_same_v<T, int64_t>) {
            return static_cast<T>(value.toULongLong());
        } else {
            static_assert(always_false<T>, "Unsupported type to cast into unsigned integral type");
        }
    }
#endif
};

// =========================================================================================================================================

template <typename T>
requires std::floating_point<T>
struct ConvertType<T>
{
    static T convert(std::same_as<UString> auto&& value)
    {
        return value.template toNumber<T>();
    }
    static T convert(std::convertible_to<std::string> auto&& value)
    {
        if constexpr (std::is_same_v<T, float>) {
            return static_cast<T>(std::stof(value));
        } else if constexpr (std::is_same_v<T, double>) {
            return static_cast<T>(std::stod(value));
        } else {
            static_assert(always_false<T>, "Unsupported type to cast into floating type");
        }
    }
    static T convert(std::floating_point auto&& value)
    {
        return static_cast<T>(value);
    }
    static T convert(std::integral auto&& value)
    {
        return static_cast<T>(value);
    }
    static T convert(bool&& value)
    {
        return value ? 1 : 0;
    }
#ifdef WITH_QT
    static int8_t convert(QString&& value)
    {
        if constexpr (std::is_same_v<T, float>) {
            return static_cast<T>(value.toFloat());
        } else if constexpr (std::is_same_v<T, double>) {
            return static_cast<T>(value.toDouble());
        } else {
            static_assert(always_false<T>, "Unsupported type to cast into floating type");
        }
    }
#endif
};

// =========================================================================================================================================

template <>
struct ConvertType<bool>
{
    static bool convert(std::convertible_to<UString> auto&& value)
    {
        return UString(value) == "true"_s || UString(value) == "1"_s || UString(value) == "on"_s;
    }
    static bool convert(std::floating_point auto&& value)
    {
        return value != 0;
    }
    static bool convert(std::integral auto&& value)
    {
        return value != 0;
    }
    static bool convert(bool&& value)
    {
        return value;
    }
#ifdef WITH_QT
    static int8_t convert(QString&& value)
    {
        return value == "true"_s || value == "1"_s || value == "on"_s;
    }
#endif
};

// =========================================================================================================================================

template <typename T>
requires enumerable<T>
struct ConvertType<T>
{
    static T convert(std::convertible_to<std::string> auto value, const T& def = {})
    {
        T result = def;
        if constexpr (enumerableFromStream<T>) {
            try {
                std::stringstream ss;
                ss << value;
                ss >> result;
            } catch (const std::exception&) {
                result = def;
            }
        } else {
            if (auto re = magic_enum::enum_cast<T>(value); re) {
                result = re.value();
            }
        }
        return result;
    }
    static T convert(std::integral auto value, const T& def = {})
    {
        if (auto re = magic_enum::enum_cast<T>(value); re) {
            return re.value();
        }
        return def;
    }
    static T convert(const UString& value, const T& def = {})
    {
        return ConvertType<T>::convert(value.toStdString(), def);
    }
};

// =========================================================================================================================================

template <typename Return, typename Value>
Return convert(Value&& value)
{
    return ConvertType<Return>::convert(std::forward<Value>(value));
}

template <typename Return, typename Value>
Return convert(Value&& value, const Return& def)
{
    return ConvertType<Return>::convert(std::forward<Value>(value), def);
}

// =========================================================================================================================================

} // namespace pack
