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

#include <cmath>
#include <pack/attribute.h>
#include <pack/types.h>
#include <pack/utils.h>

namespace pack {

// =========================================================================================================================================

class IValue : public Attribute
{
public:
    template <typename... Options>
    IValue(Options&&... opts)
    requires allIsOptions<Options...>
        : Attribute(NodeType::Value, opts...)
    {
    }
    ~IValue() override;

    IValue(const IValue& other);
    IValue(IValue&& other) noexcept;

    [[nodiscard]] virtual Type valueType() const = 0;
};

// =========================================================================================================================================

template <typename T>
concept isValue = std::is_base_of_v<IValue, T>;


template <typename T, typename CppType>
concept isValueConstructable = isSame<CppType, T> || isConvertable<CppType, T>;

// =========================================================================================================================================

template <Type ValType>
class Value : public IValue
{
public:
    using CppType                  = typename ResolveType<ValType>::type;
    using RefType                  = std::conditional_t<ValType == Type::String || ValType == Type::Bytes, CppType&, CppType>;
    using ConstRefType             = std::conditional_t<ValType == Type::String || ValType == Type::Bytes, const CppType&, CppType>;
    static constexpr Type ThisType = ValType;
    using Default                  = DefaultValue<CppType>;

public:
    /// ctor. Initialize value with value and options
    /// @param value value to initialize
    /// @param options options to initialize
    template <typename T, typename... Options>
    Value(T&& value, Options&&... options)
    requires isValueConstructable<T, CppType> && allIsOptions<Options...>
        : Value(std::forward<Options>(options)...)
    {
        setValue(std::forward<T>(value));
    }

    /// ctor. Initialize value with  options
    /// @param options options to initialize
    template <typename... Options>
    Value(Options&&... options)
    requires allIsOptions<Options...>
        : IValue(std::forward<Options>(options)...)
    {
        if (auto ret = pickOption<Default>(options...)) {
            m_def = ret->get();
            m_val = m_def;
        }
    }

    Value(const Value& other)
        : IValue(other)
        , m_val(other.m_val)
    {
    }

    Value(Value&& other)
        : IValue(std::move(other))
        , m_val(std::move(other.m_val))
    {
    }

    Value()
        : IValue()
    {
    }

    Value& operator=(const Value& other)
    {
        setValue(other.m_val);
        return *this;
    }

    Value& operator=(ConstRefType other)
    {
        setValue(other);
        return *this;
    }

public:
    ConstRefType value() const
    {
        return m_val;
    }

    ConstRefType defValue() const
    {
        return m_def;
    }

    template <typename T>
    void setValue(T&& val)
    {
        if constexpr (isValueConstructable<T, CppType>) {
            if (compare(val)) {
                m_val = val;
            }
        } else if constexpr (std::is_same_v<decltype(*this), std::decay_t<T>> || std::is_base_of_v<Value, std::decay_t<T>>) {
            if (compare(val.value())) {
                m_val = val.value();
            }
        } else {
            static_assert(always_false<T>, "Unsupported type");
        }
    }

    template <typename T>
    Value& operator=(T&& val)
    {
        setValue(std::forward<T>(val));
        return *this;
    }

    operator ConstRefType() const
    {
        return value();
    }

    template <typename T>
    int operator<=>(const T& val) const
    requires isValueConstructable<T, CppType> || std::same_as<Value<ValType>, T>
    {
        return compare(val);
    }

    template <typename T>
    bool operator==(const T& val) const
    requires isValueConstructable<T, CppType> || std::same_as<Value<ValType>, T>
    {
        return compare(val) == 0;
    }

public:
    [[nodiscard]] int compare(const Attribute& other) const override
    {
        if (auto casted = dynamic_cast<const Value<ValType>*>(&other)) {
            return compare(casted->value());
        }
        return false;
    }

    [[nodiscard]] int compare(ConstRefType other) const
    {
        if (value() < other)
            return -1;
        if (value() > other)
            return 1;
        return 0;
    }

    [[nodiscard]] UString typeName() const override
    {
        return format("Value<{}>"_s, valueTypeName(ValType));
    }

    void set(const Attribute& other) override
    {
        if (auto casted = dynamic_cast<const Value<ValType>*>(&other)) {
            setValue(*casted);
        }
    }

    void set(Attribute&& other) override
    {
        if (auto casted = dynamic_cast<const Value<ValType>*>(&other)) {
            setValue(std::move(*casted));
        }
    }

    [[nodiscard]] bool empty() const override
    {
        if constexpr (ValType == Type::Float) {
            return std::fabs(m_val - m_def) <= std::numeric_limits<float>::epsilon();
        } else if constexpr (ValType == Type::Double) {
            return std::fabs(m_val - m_def) <= std::numeric_limits<double>::epsilon();
        } else {
            return m_val == m_def;
        }
    }

    [[nodiscard]] Type valueType() const override
    {
        return ValType;
    }

    void clear() override
    {
        setValue(m_def);
    }

    static UString typeInfo()
    {
        return valueTypeName(ValType);
    }

protected:
    CppType m_val = {};
    CppType m_def = {};
};

// =========================================================================================================================================

template <Type ValType>
class NumericValue : public Value<ValType>
{
public:
    using Value<ValType>::Value;
    using Value<ValType>::operator==;
    using Value<ValType>::operator<=>;

    void operator+=(const NumericValue& other)
    {
        setValue(this->m_val + other.m_val);
    }

    void operator+=(typename Value<ValType>::ConstRefType other)
    {
        this->setValue(this->m_val + other);
    }

    void operator-=(const NumericValue& other)
    {
        this->setValue(this->m_val - other.m_val);
    }

    void operator-=(typename Value<ValType>::ConstRefType other)
    {
        this->setValue(this->m_val - other);
    }

    void operator*=(const NumericValue& other)
    {
        this->setValue(this->m_val * other.m_val);
    }

    void operator*=(typename Value<ValType>::ConstRefType other)
    {
        this->setValue(this->m_val * other);
    }

    void operator/=(const NumericValue& other)
    {
        this->setValue(this->m_val / other.m_val);
    }

    void operator/=(typename Value<ValType>::ConstRefType other)
    {
        this->setValue(this->m_val / other);
    }

    [[nodiscard]] UString typeName() const override
    {
        return format("NumericValue<{}>"_s, valueTypeName(ValType));
    }
};


// =========================================================================================================================================

} // namespace pack
