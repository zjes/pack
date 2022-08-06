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

#include "pack/types/variant.h"
#include <algorithm>
#include <stdexcept>

namespace pack {

template <typename... Types>
Variant<Types...>::Variant()
    : IVariant({})
{
}

template <typename... Types>
Variant<Types...>::Variant(const Variant& other)
    : IVariant(other)
    , m_value(other.m_value)
{
}

template <typename... Types>
Variant<Types...>::Variant(Variant&& other)
    : IVariant(other)
    , m_value(std::move(other.m_value))
{
}

template <typename... Types>
Variant<Types...>& Variant<Types...>::operator=(const Variant& other)
{
    m_value = other.m_value;
    return *this;
}

template <typename... Types>
Variant<Types...>& Variant<Types...>::operator=(Variant&& other)
{
    m_value = std::move(other.m_value);
    return *this;
}

template <typename... Types>
template <typename T>
bool Variant<Types...>::is() const
{
    return std::holds_alternative<T>(m_value);
}

template <typename... Types>
template <typename T>
const T& Variant<Types...>::get() const
{
    assert(is<T>());
    return std::get<T>(m_value);
}

template <typename... Types>
template <typename T>
T& Variant<Types...>::get()
{
    assert(is<T>());
    return std::get<T>(m_value);
}

template <typename... Types>
const Attribute* Variant<Types...>::get() const
{
    const Attribute* attr = nullptr;
    try {
        std::visit(
            [&](const auto& var) {
                using T = std::decay_t<decltype(var)>;
                if (std::is_base_of_v<Attribute, T>) {
                    attr = &var;
                }
            },
            m_value);
    } catch (const std::bad_variant_access&) {
        return nullptr;
    }

    return attr;
}

template <typename... Types>
Attribute* Variant<Types...>::get()
{
    Attribute* attr = nullptr;

    try {
        std::visit(
            [&](auto& var) {
                using T = std::decay_t<decltype(var)>;
                if (std::is_base_of_v<Attribute, T>) {
                    attr = &var;
                }
            },
            m_value);
    } catch (const std::bad_variant_access&) {
        return nullptr;
    }

    return attr;
}

template <typename... Types>
template <typename T>
T& Variant<Types...>::reset()
{
    m_value = T{};
    return get<T>();
}

template <typename... Types>
bool Variant<Types...>::compare(const Attribute& other) const
{
    if (auto casted = dynamic_cast<const Variant<Types...>*>(&other)) {
        return m_value == casted->m_value;
    }
    return false;
}

template <typename... Types>
string_t Variant<Types...>::typeName() const
{
    return typeInfo();
}

template <typename... Types>
string_t Variant<Types...>::typeInfo()
{
    return "Variant"_s;
}


template <typename... Types>
void Variant<Types...>::set(const Attribute& other)
{
    if (auto casted = dynamic_cast<const Variant<Types...>*>(&other)) {
        m_value = casted->m_value;
    }
}

template <typename... Types>
void Variant<Types...>::set(Attribute&& other)
{
    if (auto casted = dynamic_cast<Variant<Types...>*>(&other)) {
        m_value = casted->m_value;
    }
}

template <typename... Types>
bool Variant<Types...>::hasValue() const
{
    return m_value.index() != std::variant_npos;
}

template <typename... Types>
void Variant<Types...>::clear()
{
    m_value = {};
}

template <typename T>
struct TypeHelper
{
    using Type = T;
};

template <typename... Ts, typename TF>
void foreachType(TF&& f)
{
    (f(TypeHelper<Ts>{}), ...);
}

template <typename... Types>
bool Variant<Types...>::findBetter(const std::vector<string_t>& fields)
{
    try {
        size_t typeHash;
        float  betterRating = 0.f;

        foreachType<Types...>([&](auto t) {
            using ImplType = typename decltype(t)::Type;

            int count = 0;
            for (const auto& fld : ImplType::staticFieldNames()) {
                count += std::find(fields.begin(), fields.end(), fld) == fields.end() ? 0 : 1;
            }
            float rating = float(count) / float(ImplType::staticFieldNames().size());
            if (rating >= betterRating) {
                betterRating = rating;
                typeHash     = typeid(ImplType).hash_code();
            }
        });

        foreachType<Types...>([&](auto t) {
            using ImplType = typename decltype(t)::Type;
            if (typeid(ImplType).hash_code() == typeHash) {
                m_value = ImplType{};
            }
        });

        return m_value.index() != std::variant_npos;
    } catch (const std::bad_variant_access&) {
        return false;
    }
}

} // namespace pack
