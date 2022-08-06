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
#include "pack/expected.h"

namespace pack {

// =========================================================================================================================================

/// Enum class interface
class IEnum : public Attribute
{
public:
    using Values = std::vector<std::pair<std::string, int>>;

    template <typename... Options, typename = isOptions<Options...>>
    explicit IEnum(Options&&... args)
        : Attribute(NodeType::Enum, std::forward<Options>(args)...)
    {
    }

public:
    /// Returns a string representation of the enum value
    virtual string_t asString() const = 0;

    /// Sets enum value from string
    virtual Expected<void> fromString(const string_t& value) = 0;

    /// Returns a string representation of the enum value
    virtual int asInt() const = 0;

    /// Sets enum value from string
    virtual Expected<void> fromInt(int value) = 0;

    /// Return values map for Name -> int pairs
    virtual Values values() const = 0;
};

// =========================================================================================================================================

template <typename T>
class Enum : public IEnum
{
public:
    using IEnum::IEnum;

public:
    /// ctor. Initialize enum with value and options
    /// @param value value to initialize
    /// @param opts options
    template <typename... Options, typename = isOptions<Options...>>
    Enum(const T& value, Options&&... opts);

    /// ctor. Initialize enun with  options
    /// @param opts options
    template <typename... Options, typename = isOptions<Options...>>
    Enum(Options&&... opts);

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
    Expected<void> setValue(Value&& val);

    /// Assigmen operator
    Enum& operator=(const T& val);

public:
    /// Compares enums
    bool compare(const Attribute& other) const override;

    /// Returns type name enum<T>
    string_t typeName() const override;

    /// Return values map for Name -> int pairs
    Values values() const override;

    /// Sets the enum
    void set(const Attribute& other) override;

    /// Sets the enum
    void set(Attribute&& other) override;

    /// Returns true if enum has value
    bool hasValue() const override;

    /// Clears enum value
    void clear() override;

    /// Returns enum value as string representation
    string_t asString() const override;

    /// Sets enum value from string
    Expected<void> fromString(const string_t& value) override;

    /// Returns enum as int representation
    int asInt() const override;

    /// Sets enum value from int
    Expected<void> fromInt(int value) override;

private:
    static string_t asString(const T& value);
    void            _setValue(T value);

protected:
    T m_value = {};
    T m_def   = {};
};

// =========================================================================================================================================

} // namespace pack

#include "pack/types/private/enum.inl" // IWYU pragma: keep
