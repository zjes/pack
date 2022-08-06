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

#include "pack/types/value.h"
#include "pack/utils.h"
#include <cassert>
#include <cmath>

namespace pack {

template <Type ValType>
template <typename T, typename, typename... Options, typename>
Value<ValType>::Value(T&& val, Options&&... opts)
    : Value(std::forward<Options>(opts)...)
{
    setValue(std::forward<T>(val));
}

template <Type ValType>
template <typename... Options, typename>
Value<ValType>::Value(Options&&... opts)
    : IValue(std::forward<Options>(opts)...)
{
    if (auto ret = pickOption<Default>(opts...)) {
        m_def = ret->template get<typename ResolveType<ValType>::type>();
        m_val = m_def;
    }
}

template <Type ValType>
Value<ValType>::Value(const Value& other)
    : IValue(other)
    , m_val(other.m_val)
{
}

template <Type ValType>
Value<ValType>::Value(Value&& other) noexcept
    : IValue(std::move(other))
    , m_val(std::move(other.m_val))
{
}

template <Type ValType>
Value<ValType>::Value()
    : IValue()
{
}

template <Type ValType>
Value<ValType>& Value<ValType>::operator=(const Value<ValType>& other)
{
    setValue(other.m_val);
    return *this;
}

template <Type ValType>
Value<ValType>& Value<ValType>::operator=(typename Value<ValType>::ConstRefType other)
{
    setValue(other);
    return *this;
}

template <Type ValType>
typename Value<ValType>::ConstRefType Value<ValType>::value() const
{
    return m_val;
}

template <Type ValType>
typename Value<ValType>::ConstRefType Value<ValType>::defValue() const
{
    return m_def;
}

template <Type ValType>
template <typename T>
void Value<ValType>::setValue(T&& val)
{
    if constexpr (isValueConstructable<T, CppType>::value) {
        if (!compare(val)) {
            m_val = val;
        }
    } else if constexpr (std::is_same_v<decltype(*this), std::decay_t<T>> || std::is_base_of_v<Value, std::decay_t<T>>) {
        if (!compare(val.value())) {
            m_val = val.value();
        }
    } else {
        static_assert(always_false<T>, "Unsupported type");
    }
}

template <Type ValType>
template <typename T>
Value<ValType>& Value<ValType>::operator=(T&& val)
{
    setValue(std::forward<T>(val));
    return *this;
}

template <Type ValType>
Value<ValType>::operator ConstRefType() const
{
    return value();
}

template <Type ValType>
template <typename T>
bool Value<ValType>::operator==(const T& other) const
{
    return compare(other);
}

template <Type ValType>
bool Value<ValType>::operator==(const Value& other) const
{
    return compare(other.value());
}

template <Type ValType>
bool Value<ValType>::operator==(Value<ValType>::ConstRefType val) const
{
    return compare(val);
}

template <Type ValType>
bool Value<ValType>::operator!=(const Value& other) const
{
    return !compare(other.value());
}

template <Type ValType>
bool Value<ValType>::operator!=(Value<ValType>::ConstRefType val) const
{
    return !compare(val);
}

template <Type ValType>
bool Value<ValType>::compare(const Attribute& other) const
{
    if (auto casted = dynamic_cast<const Value<ValType>*>(&other)) {
        return compare(casted->value());
    }
    return false;
}

template <Type ValType>
bool Value<ValType>::compare(ConstRefType val) const
{
    if constexpr (ValType == Type::Float) {
        return std::fabs(value() - val) <= std::numeric_limits<float>::epsilon();
    } else if constexpr (ValType == Type::Double) {
        return std::fabs(value() - val) <= std::numeric_limits<double>::epsilon();
    } else {
        return value() == val;
    }
    return false;
}

template <Type ValType>
string_t Value<ValType>::typeName() const
{
    return fromStdString(fmt::format("Value<{}>", valueTypeName(ValType)));
}

template <Type ValType>
void Value<ValType>::set(const Attribute& other)
{
    if (auto casted = dynamic_cast<const Value<ValType>*>(&other)) {
        setValue(*casted);
    }
}

template <Type ValType>
void Value<ValType>::set(Attribute&& other)
{
    if (auto casted = dynamic_cast<const Value<ValType>*>(&other)) {
        setValue(std::move(*casted));
    }
}

template <Type ValType>
bool Value<ValType>::hasValue() const
{
    if constexpr (ValType == Type::Float) {
        return std::fabs(m_val - m_def) > std::numeric_limits<float>::epsilon();
    } else if constexpr (ValType == Type::Double) {
        return std::fabs(m_val - m_def) > std::numeric_limits<double>::epsilon();
    } else {
        return m_val != m_def;
    }
}

template <Type ValType>
Type Value<ValType>::valueType() const
{
    return ValType;
}

template <Type ValType>
void Value<ValType>::clear()
{
    setValue(m_def);
}

template <Type ValType>
string_t Value<ValType>::typeInfo()
{
    return valueTypeName(ValType);
}

// =========================================================================================================================================

template <Type ValType>
void NumericValue<ValType>::operator+=(const NumericValue<ValType>& other)
{
    this->setValue(this->m_val + other.m_val);
}

template <Type ValType>
void NumericValue<ValType>::operator+=(typename Value<ValType>::ConstRefType other)
{
    this->setValue(this->m_val + other);
}

template <Type ValType>
void NumericValue<ValType>::operator-=(const NumericValue& other)
{
    this->setValue(this->m_val - other.m_val);
}

template <Type ValType>
void NumericValue<ValType>::operator-=(typename Value<ValType>::ConstRefType other)
{
    this->setValue(this->m_val - other);
}

template <Type ValType>
void NumericValue<ValType>::operator*=(const NumericValue& other)
{
    this->setValue(this->m_val * other.m_val);
}

template <Type ValType>
void NumericValue<ValType>::operator*=(typename Value<ValType>::ConstRefType other)
{
    this->setValue(this->m_val * other);
}

template <Type ValType>
void NumericValue<ValType>::operator/=(const NumericValue& other)
{
    this->setValue(this->m_val / other.m_val);
}

template <Type ValType>
void NumericValue<ValType>::operator/=(typename Value<ValType>::ConstRefType other)
{
    this->setValue(this->m_val / other);
}

// =========================================================================================================================================

} // namespace pack
