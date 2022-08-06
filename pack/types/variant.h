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
#include <variant>

namespace pack {

// =========================================================================================================================================

class IVariant : public Attribute
{
public:
    IVariant(const string_t& key)
        : Attribute(NodeType::Variant, key)
    {
    }

    virtual const Attribute* get() const                                     = 0;
    virtual Attribute*       get()                                           = 0;
    virtual bool             findBetter(const std::vector<string_t>& fields) = 0;
};

// =========================================================================================================================================

template <typename... Types>
class Variant : public IVariant
{
public:
    using IVariant::IVariant;

    using CppType = typename std::variant<Types...>;

    Variant();
    Variant(const Variant& other);
    Variant(Variant&& other);

    Variant& operator=(const Variant& other);
    Variant& operator=(Variant&& other);

    template <typename T, typename = std::enable_if_t<!std::is_same_v<std::decay_t<T>, Variant>>>
    Variant(T&& val)
        : IVariant({})
        , m_value(val)
    {
    }

    template <typename T>
    bool is() const;

    template <typename T>
    const T& get() const;

    template <typename T>
    T& get();

    template <typename T>
    T& reset();

public:
    const Attribute* get() const override;
    Attribute*       get() override;
    bool             findBetter(const std::vector<string_t>& fields) override;
    static string_t  typeInfo();

public:
    bool     compare(const Attribute& other) const override;
    string_t typeName() const override;
    void     set(const Attribute& other) override;
    void     set(Attribute&& other) override;
    bool     hasValue() const override;
    void     clear() override;

private:
    CppType m_value;
};

// =========================================================================================================================================


// =========================================================================================================================================

} // namespace pack

#include "pack/types/private/variant.inl" // IWYU pragma: keep
