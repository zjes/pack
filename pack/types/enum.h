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

namespace pack {

// =========================================================================================================================================

/// Enum class interface
class IEnum : public Attribute
{
public:
    using Values = std::vector<std::pair<UString, int>>;

    template <typename... Options>
    explicit IEnum(Options&&... args)
    requires allIsOptions<Options...>
        : Attribute(NodeType::Enum, std::forward<Options>(args)...)
    {
    }

public:
    /// Returns a string representation of the enum value
    virtual UString asString() const = 0;

    /// Sets enum value from string
    virtual void fromString(const UString& value) = 0;

    /// Returns a string representation of the enum value
    virtual int asInt() const = 0;

    /// Sets enum value from string
    virtual void fromInt(int value) = 0;

    /// Return values map for Name -> int pairs
    virtual Values values() const = 0;
};

// =========================================================================================================================================

template <typename T>
class Enum : public IEnum
{
public:
    using IEnum::IEnum;
    using Default = Default<T>;

public:
    /// ctor. Initialize enum with value and options
    /// @param value value to initialize
    /// @param opts options
    template <typename... Options>
    Enum(const T& value, Options&&... opts)
    requires allIsOptions<Options...>;

    /// ctor. Initialize enun with  options
    /// @param opts options
    template <typename... Options>
    Enum(Options&&... opts)
    requires allIsOptions<Options...>;

    Enum(const Enum& other) = default;
    Enum(Enum&& other)      = default;
    Enum();

public:
    /// Returns default value
    const T& defValue() const;

    /// Returns value
    const T& value() const;

    /// Returns value
    operator const T&() const;

    /// Sets the value
    /// @param val value to set
    template <typename Value>
    void setValue(Value&& val);

    /// Assigmen operator
    Enum& operator=(const T& val);

public:
    /// Compares enums
    int compare(const Attribute& other) const override;

    /// Returns type name enum<T>
    UString typeName() const override;

    /// Return values map for Name -> int pairs
    Values values() const override;

    /// Sets the enum
    void set(const Attribute& other) override;

    /// Sets the enum
    void set(Attribute&& other) override;

    /// Returns true if enum has value
    bool empty() const override;

    /// Clears enum value
    void clear() override;

    /// Returns enum value as string representation
    UString asString() const override;

    /// Sets enum value from string
    void fromString(const UString& value) override;

    /// Returns enum as int representation
    int asInt() const override;

    /// Sets enum value from int
    void fromInt(int value) override;

private:
    void _setValue(T value);

protected:
    T m_value = {};
    T m_def   = {};
};

// =========================================================================================================================================

} // namespace pack

#include "pack/types/private/enum.inl" // IWYU pragma: keep
