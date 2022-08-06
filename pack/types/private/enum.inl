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

#include "pack/types/enum.h"
#include <sstream>

#if defined(__clang__) && __clang_major__ >= 5 || defined(__GNUC__) && __GNUC__ >= 9
#define IS_MAGIC_ENUM
#include "pack/magic-enum.h"
#endif

namespace pack {
// =========================================================================================================================================
// Traits helpers
// =========================================================================================================================================

namespace details {

    template <typename StreamT, typename EnumT>
    struct canToStream
    {
        template <typename S, typename T>
        static auto test(int) -> decltype(std::declval<S&>() << std::declval<T>(), std::true_type());

        template <typename, typename>
        static auto test(...) -> std::false_type;

        static const bool value = decltype(test<StreamT, EnumT>(0))::value;
    };

    template <typename StreamT, typename EnumT>
    struct canFromStream
    {
        template <typename S, typename T>
        static auto test(int) -> decltype(std::declval<S&>() >> std::declval<T&>(), std::true_type());

        template <typename, typename>
        static auto test(...) -> std::false_type;

        static const bool value = decltype(test<StreamT, EnumT>(0))::value;
    };

} // namespace details

// =========================================================================================================================================
// Enum implementation
// =========================================================================================================================================

template <typename T>
Enum<T>::Enum()
    : IEnum()
{
}

template <typename T>
template <typename... Options, typename>
Enum<T>::Enum(const T& value, Options&&... opts)
    : Enum(std::forward<Options>(opts)...)
{
    setValue(value);
}

template <typename T>
template <typename... Options, typename>
Enum<T>::Enum(Options&&... opts)
    : IEnum(std::forward<Options>(opts)...)
{
    if (auto ret = pickOption<Default>(opts...)) {
        m_def   = std::any_cast<const T&>(ret->value);
        m_value = std::any_cast<const T&>(ret->value);
    }
}

template <typename T>
const T& Enum<T>::defValue() const
{
    return m_def;
}

template <typename T>
const T& Enum<T>::value() const
{
    return m_value;
}

template <typename T>
Enum<T>::operator const T&() const
{
    return m_value;
}

template <typename T>
Enum<T>& Enum<T>::operator=(const T& val)
{
    _setValue(val);
    return *this;
}

template <typename T>
template <typename Value>
Expected<void> Enum<T>::setValue(Value&& val)
{
    if constexpr (std::is_same_v<std::decay_t<Value>, T>) {
        _setValue(std::forward<Value>(val));
        return {};
    } else if constexpr (std::is_same_v<UseType<Value>, string_t>) {
        return fromString(std::forward<Value>(val));
    } else if constexpr (std::is_integral_v<Value>) {
        return fromInt(std::forward<Value>(val));
    } else {
        static_assert(always_false<Value>, "Unsupported type");
    }
}

template <typename T>
bool Enum<T>::compare(const Attribute& other) const
{
    if (auto casted = dynamic_cast<const Enum<T>*>(&other)) {
        return casted->value() == value();
    }
    return false;
}

template <typename T>
string_t Enum<T>::typeName() const
{
    return "Enum"_s;
}

template <typename T>
string_t Enum<T>::asString(const T& value)
{
    if constexpr (details::canToStream<std::stringstream, T>::value) {
        std::stringstream ss;
        ss << value;
        return fromStdString(ss.str());
    } else if constexpr (!details::canToStream<std::stringstream, T>::value) {
#ifdef IS_MAGIC_ENUM
        return fromStdString(magic_enum::enum_name(value).data());
#else
        // static_assert (false, "Cannot serialize enum, lacked stream oprator <<");
        return {};
#endif
    }
}

template <typename T>
IEnum::Values Enum<T>::values() const
{
    IEnum::Values ret;
#ifdef IS_MAGIC_ENUM
    for (const auto& [val, name] : magic_enum::enum_entries<T>()) {
        ret.emplace_back(name, int(val));
    }
#endif
    return ret;
}

template <typename T>
void Enum<T>::set(const Attribute& other)
{
    if (auto casted = dynamic_cast<const Enum<T>*>(&other)) {
        _setValue(*casted);
    }
}

template <typename T>
void Enum<T>::set(Attribute&& other)
{
    if (auto casted = dynamic_cast<Enum<T>*>(&other)) {
        _setValue(std::move(*casted));
    }
}

template <typename T>
bool Enum<T>::hasValue() const
{
    return m_value != m_def;
}

template <typename T>
string_t Enum<T>::asString() const
{
    return asString(value());
}

template <typename T>
Expected<void> Enum<T>::fromString(const string_t& value)
{
    if constexpr (details::canFromStream<std::stringstream, T>::value) {
        try {
            T val;

            std::stringstream ss;
            ss << value;
            ss >> val;

            _setValue(val);
        } catch (const std::exception&) {
            return unexpected("Wrong conversion from string"_s);
        }
    } else if constexpr (!details::canFromStream<std::stringstream, T>::value) {
#ifdef IS_MAGIC_ENUM
        if (auto re = magic_enum::enum_cast<T>(toStdString(value)); re) {
            _setValue(re.value());
        } else {
            return unexpected("Wrong conversion from string"_s);
        }
#else
        static_assert(fty::always_false<T>, "Unsupported conversion");
#endif
    } else {
        static_assert(always_false<T>, "Unsupported conversion");
    }
    return {};
}

template <typename T>
int Enum<T>::asInt() const
{
    return int(value());
}

template <typename T>
Expected<void> Enum<T>::fromInt(int value)
{
#ifdef IS_MAGIC_ENUM
    if (auto ret = magic_enum::enum_cast<T>(value); ret) {
        _setValue(*ret);
    } else {
        return unexpected("Wrong conversion from string"_s);
    }
#else
    setValue(static_cast<T>(value));
#endif
    return {};
}

template <typename T>
void Enum<T>::clear()
{
    _setValue(m_def);
}

template <typename T>
void Enum<T>::_setValue(T val)
{
    if (value() != val) {
        m_value = val;
    }
}

// =========================================================================================================================================

}
