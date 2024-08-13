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
    ~IEnum() override;

    using Values = std::vector<std::pair<UString, int>>;

    template <typename... Options>
    explicit IEnum(Options&&... args)
    requires allIsOptions<Options...>
        : Attribute(NodeType::Enum, std::forward<Options>(args)...)
    {
    }

public:
    /// Returns a string representation of the enum value
    [[nodiscard]] virtual UString asString() const = 0;

    /// Sets enum value from string
    virtual void fromString(const UString& value) = 0;

    /// Returns a string representation of the enum value
    [[nodiscard]] virtual int asInt() const = 0;

    /// Sets enum value from string
    virtual void fromInt(int value) = 0;

    /// Return values map for Name -> int pairs
    [[nodiscard]] virtual Values values() const = 0;
};

// =========================================================================================================================================

template <typename T>
class Enum : public IEnum
{
public:
    using IEnum::IEnum;
    using Default = DefaultValue<T>;

public:
    /// ctor. Initialize enum with value and options
    /// @param value value to initialize
    /// @param opts options
    template <typename... Options>
    Enum(const T& value, Options&&... opts)
    requires allIsOptions<Options...>
        : Enum(std::forward<Options>(opts)...)
    {
        setValue(value);
    }

    /// ctor. Initialize enun with  options
    /// @param opts options
    template <typename... Options>
    Enum(Options&&... opts)
    requires allIsOptions<Options...>
        : IEnum(std::forward<Options>(opts)...)
    {
        if (auto ret = pickOption<Default>(opts...)) {
            m_def   = ret->get();
            m_value = m_def;
        }
    }

    Enum(const Enum& other)
        : m_value(other.m_value)
    {
    }

    Enum(Enum&& other)
        : m_value(std::move(other.m_value))
    {
    }

    Enum()
        : IEnum()
    {
    }


public:
    /// Returns default value
    [[nodiscard]] const T& defValue() const
    {
        return m_def;
    }

    /// Returns value
    [[nodiscard]] const T& value() const
    {
        return m_value;
    }

    /// Returns value
    operator const T&() const
    {
        return m_value;
    }

    /// Sets the value
    /// @param val value to set
    template <typename Value>
    void setValue(Value&& val)
    {
        if constexpr (std::same_as<T, std::decay_t<Value>>) {
            _setValue(val);
        } else if constexpr (canConvert<T, Value>) {
            _setValue(convert<T>(val, m_def));
        } else {
            static_assert(always_false<Value>, "Unsupported type");
        }
    }

    /// Assigmen operator
    Enum& operator=(const T& val)
    {
        _setValue(val);
        return *this;
    }

public:
    /// Compares enums
    [[nodiscard]] int compare(const Attribute& other) const override
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

    /// Returns type name enum<T>
    [[nodiscard]] UString typeName() const override
    {
        return format("Enum<{}>"_s, magic_enum::enum_type_name<T>());
    }

    /// Return values map for Name -> int pairs
    [[nodiscard]] Values values() const override
    {
        IEnum::Values ret;
        for (const auto& [val, name] : magic_enum::enum_entries<T>()) {
            ret.emplace_back(std::string{name}, int(val));
        }
        return ret;
    }

    /// Sets the enum
    void set(const Attribute& other) override
    {
        if (auto casted = dynamic_cast<const Enum<T>*>(&other)) {
            _setValue(*casted);
        }
    }

    /// Sets the enum
    void set(Attribute&& other) override
    {
        if (auto casted = dynamic_cast<Enum<T>*>(&other)) {
            _setValue(std::move(*casted));
        }
    }

    /// Returns true if enum has value
    [[nodiscard]] bool empty() const override
    {
        return m_value == m_def;
    }

    /// Clears enum value
    void clear() override
    {
        _setValue(m_def);
    }

    /// Returns enum value as string representation
    [[nodiscard]] UString asString() const override
    {
        return convert<UString>(value());
    }

    /// Sets enum value from string
    void fromString(const UString& value) override
    {
        _setValue(convert<T>(value, m_def));
    }

    /// Returns enum as int representation
    [[nodiscard]] int asInt() const override
    {
        return int(value());
    }

    /// Sets enum value from int
    void fromInt(int value) override
    {
        _setValue(convert<T>(value, m_def));
    }

private:
    void _setValue(T val)
    {
        if (value() != val) {
            m_value = val;
        }
    }

protected:
    T m_value = {};
    T m_def   = {};
};

// =========================================================================================================================================

} // namespace pack
