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
#include "pack/types/enum.h"

namespace pack {
// =========================================================================================================================================
// Enum implementation
// =========================================================================================================================================

template <typename T>
Enum<T>::Enum()
    : IEnum()
{
}

template <typename T>
template <typename... Options>
Enum<T>::Enum(const T& value, Options&&... opts)
requires allIsOptions<Options...>
    : Enum(std::forward<Options>(opts)...)
{
    setValue(value);
}

template <typename T>
template <typename... Options>
Enum<T>::Enum(Options&&... opts)
requires allIsOptions<Options...>
    : IEnum(std::forward<Options>(opts)...)
{
    if (auto ret = pickOption<Default>(opts...)) {
        m_def   = ret->get();
        m_value = m_def;
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
void Enum<T>::setValue(Value&& val)
{
    if constexpr(std::same_as<T, std::decay_t<Value>>) {
        _setValue(val);
    } else if constexpr(canConvert<T, Value>) {
        _setValue(convert<T>(val, m_def));
    } else {
        static_assert(always_false<Value>, "Unsupported type");
    }
}

template <typename T>
int Enum<T>::compare(const Attribute& other) const
{
    if (auto casted = dynamic_cast<const Enum<T>*>(&other)) {
        if (value() < casted->value())
            return -1;
        if (value() > casted->value())
            return 1;
        return 0;
    }
    return -1;
}

template <typename T>
UString Enum<T>::typeName() const
{
    return format("Enum<{}>"_s, magic_enum::enum_type_name<T>());
}

template <typename T>
IEnum::Values Enum<T>::values() const
{
    IEnum::Values ret;
    for (const auto& [val, name] : magic_enum::enum_entries<T>()) {
        ret.emplace_back(std::string{name}, int(val));
    }
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
bool Enum<T>::empty() const
{
    return m_value == m_def;
}

template <typename T>
UString Enum<T>::asString() const
{
    return convert<UString>(value());
}

template <typename T>
void Enum<T>::fromString(const UString& value)
{
    _setValue(convert<T>(value, m_def));
}

template <typename T>
int Enum<T>::asInt() const
{
    return int(value());
}

template <typename T>
void Enum<T>::fromInt(int value)
{
    _setValue(convert<T>(value, m_def));
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

} // namespace pack
