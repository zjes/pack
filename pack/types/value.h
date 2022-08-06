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
#include "pack/types.h"
#include "pack/utils.h"

namespace pack {

// =========================================================================================================================================

class IValue : public Attribute
{
public:
    template <typename... Options, typename = isOptions<Options...>>
    IValue(Options&&... opts)
        : Attribute(NodeType::Value, opts...)
    {
    }

    IValue(const IValue& other)     = default;
    IValue(IValue&& other) noexcept = default;

    virtual Type valueType() const = 0;
};

// =========================================================================================================================================

template <typename T>
using isValue = std::is_base_of<IValue, T>;

template <typename T, typename CppType>
using isValueConstructable = std::disjunction<typename pack::isSame<CppType, T>, typename pack::isConvertable<CppType, T>>;

// =========================================================================================================================================

template <Type ValType>
class Value : public IValue
{
public:
    using CppType                  = typename ResolveType<ValType>::type;
    using RefType                  = std::conditional_t<ValType == Type::String || ValType == Type::Binary, CppType&, CppType>;
    using ConstRefType             = std::conditional_t<ValType == Type::String || ValType == Type::Binary, const CppType&, CppType>;
    static constexpr Type ThisType = ValType;

public:
    /// ctor. Initialize value with value and options
    /// @param value value to initialize
    /// @param options options to initialize
    template <
        typename T,
        typename = std::enable_if_t<isValueConstructable<T, CppType>::value>,
        typename... Options,
        typename = isOptions<Options...>>
    Value(T&& value, Options&&... options);

    /// ctor. Initialize value with  options
    /// @param options options to initialize
    template <typename... Options, typename = isOptions<Options...>>
    Value(Options&&... options);

    Value(const Value& other);
    Value(Value&& other) noexcept;
    Value();

    Value& operator=(const Value& other);
    Value& operator=(ConstRefType other);

public:
    ConstRefType value() const;
    ConstRefType defValue() const;

    template <typename T>
    void setValue(T&& val);

    template <typename T>
    Value& operator=(T&& val);
           operator ConstRefType() const;

    template <typename T>
    bool operator==(const T& val) const;

    bool operator==(const Value& other) const;
    bool operator==(ConstRefType val) const;
    bool operator!=(const Value& other) const;
    bool operator!=(ConstRefType val) const;

public:
    bool     compare(const Attribute& other) const override;
    bool     compare(ConstRefType other) const;
    string_t typeName() const override;
    void     set(const Attribute& other) override;
    void     set(Attribute&& other) override;
    bool     hasValue() const override;
    Type     valueType() const override;
    void     clear() override;

    static string_t typeInfo();

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

    void operator+=(const NumericValue& other);
    void operator+=(typename Value<ValType>::ConstRefType other);

    void operator-=(const NumericValue& other);
    void operator-=(typename Value<ValType>::ConstRefType other);

    void operator*=(const NumericValue& other);
    void operator*=(typename Value<ValType>::ConstRefType other);

    void operator/=(const NumericValue& other);
    void operator/=(typename Value<ValType>::ConstRefType other);
};

// =========================================================================================================================================

} // namespace pack

#include "pack/types/private/value.inl" // IWYU pragma: keep
