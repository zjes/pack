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

#include "pack/types/list.h"

namespace pack {

template <typename T>
List<T>::List(List<T>::ListType&& value):
    m_value(std::move(value))
{
}

template <typename T>
List<T>::List(const List<T>::ListType& value):
    m_value(value)
{
}

template <typename T>
template <typename... Options>
List<T>::List(List<T>::ListType&& value, Options&&... opts)
requires isValueConstructable<T, ListType> && allIsOptions<Options...>
    : IList(std::forward<Options>(opts)...)
    , m_value(value)
{
}

template <typename T>
template <typename... Options>
List<T>::List(std::initializer_list<T> values, Options&&... opts)
requires isValueConstructable<T, ValueType> && allIsOptions<Options...>
    : IList(std::forward<Options>(opts)...)
    , m_value(values)
{
}

template <typename T>
template <typename... Options>
List<T>::List(Options&&... opts)
requires allIsOptions<Options...>
    : IList(std::forward<Options>(opts)...)
{
}

// Element access

template <typename T>
const typename List<T>::ListType& List<T>::toVector() const
{
    return m_value;
}

template <typename T>
typename List<T>::ListType& List<T>::toVector()
{
    return m_value;
}

template <typename T>
void List<T>::setVector(const typename List<T>::ListType& list)
{
    m_value = list;
}

template <typename T>
void List<T>::setVector(typename List<T>::ListType&& list)
{
    m_value = std::move(list);
}

template <typename T>
template <typename TT>
void List<T>::setVector(std::vector<TT>&& value)
requires isValueConstructable<TT, ValueType>
{
    m_value = std::vector<TT>(m_value.end(), std::make_move_iterator(value.begin()), std::make_move_iterator(value.end()));
}

template <typename T>
template <typename TT>
void List<T>::setVector(const std::vector<TT>& value)
requires isValueConstructable<TT, ValueType>
{
    m_value = std::vector<TT>(m_value.end(), value.begin(), value.end());
}

template <typename T>
void List<T>::operator=(std::initializer_list<ValueType> values)
{
    m_value = std::vector<T>(values.begin(), values.end());
}

template <typename T>
bool List<T>::operator==(const std::vector<ValueType>& values) const
{
    if (m_value.size() != values.size()) {
        return false;
    }
    for (size_t i = 0; i < m_value.size(); ++i) {
        if (m_value[i] != values[i]) {
            return false;
        }
    }
    return true;
}

template <typename T>
bool List<T>::operator==(const List& values) const
{
    return m_value == values.m_value;
}
template <typename T>
int List<T>::size() const
{
    return int(m_value.size());
}

// Iterators

template <typename T>
typename List<T>::ConstIterator List<T>::begin() const
{
    return m_value.begin();
}

template <typename T>
typename List<T>::ConstIterator List<T>::end() const
{
    return m_value.end();
}

template <typename T>
typename List<T>::Iterator List<T>::begin()
{
    return m_value.begin();
}

template <typename T>
typename List<T>::Iterator List<T>::end()
{
    return m_value.end();
}

// Attribute
template <typename T>
int List<T>::compare(const Attribute& other) const
{
    if (auto casted = dynamic_cast<const List*>(&other)) {
        return casted->toVector() == m_value;
    }
    return false;
}

template <typename T>
UString List<T>::typeName() const
{
    return _typeName();
}

template <typename T>
UString List<T>::_typeName()
{
    return fmt::format("List<{}>", T::typeInfo());
}

template <typename T>
void List<T>::set(const Attribute& other)
{
    if (auto casted = dynamic_cast<const List*>(&other)) {
        m_value = casted->toVector();
    }
}

template <typename T>
void List<T>::set(Attribute&& other)
{
    if (auto casted = dynamic_cast<const List*>(&other)) {
        m_value = std::move(casted->toVector());
    }
}

// IList

template <typename T>
void List<T>::clear()
{
    m_value.clear();
}

template <typename T>
const Attribute& List<T>::get(int index) const
{
    return m_value.at(size_t(index));
}

template <typename T>
Attribute& List<T>::create()
{
    return append();
}

template <typename T>
bool List<T>::isValueList() const
{
    return IsValueList::value;
}

template <typename T>
Type List<T>::valueType() const
{
    if constexpr (IsValueList::value) {
        return T::ThisType;
    } else {
        return Type::Unknown;
    }
}

// Append

template <typename T>
void List<T>::append(const T& value)
{
    m_value.emplace_back(value);
}

template <typename T>
void List<T>::append(T&& value)
{
    m_value.emplace_back(value);
}

template <typename T>
T& List<T>::append()
{
    return m_value.emplace_back();
}

template <typename T>
void List<T>::append(List<T>&& value)
{
    m_value.insert(m_value.end(), std::make_move_iterator(value.begin()), std::make_move_iterator(value.end()));
}

template <typename T>
void List<T>::append(const List<T>& value)
{
    m_value.insert(m_value.end(), value.begin(), value.end());
}

template <typename T>
void List<T>::append(List::ListType&& value)
{
    m_value.insert(m_value.end(), std::make_move_iterator(value.begin()), std::make_move_iterator(value.end()));
}

template <typename T>
void List<T>::append(const List::ListType& value)
{
    m_value.insert(m_value.end(), value.begin(), value.end());
}

template <typename T>
template <typename TT>
void List<T>::append(std::vector<TT>&& value)
requires isValueConstructable<TT, ValueType>
{
    m_value.insert(m_value.end(), std::make_move_iterator(value.begin()), std::make_move_iterator(value.end()));
}

template <typename T>
template <typename TT>
void List<T>::append(const std::vector<TT>& value)
requires isValueConstructable<TT, ValueType>
{
    m_value.insert(m_value.end(), value.begin(), value.end());
}

// List access

template <typename T>
const T& List<T>::operator[](int index) const
{
    return m_value.at(size_t(index));
}

template <typename T>
T& List<T>::operator[](int index)
{
    if (index < 0 || size_t(index) >= m_value.size()) {
        throw std::runtime_error("out of bounds");
    }
    return m_value[size_t(index)];
}

template <typename T>
template <typename ToFind>
std::optional<T> List<T>::findItem(ToFind&& toFind) const
{
    if (auto found = find(std::forward<ToFind>(toFind)); found != end()) {
        return *found;
    }
    return std::nullopt;
}

template <typename T>
template <typename ToFind>
typename List<T>::ConstIterator List<T>::find(ToFind&& toFind) const
{
    if constexpr (std::is_invocable_r_v<bool, ToFind, T>) {
        return std::find_if(m_value.begin(), m_value.end(), std::forward<ToFind>(toFind));
    } else {
        return std::find(m_value.begin(), m_value.end(), toFind);
    }
    return m_value.end();
}

template <typename T>
template <typename ToFind>
int List<T>::indexOf(ToFind&& toFind) const
{
    if (auto found = find(std::forward<ToFind>(toFind)); found != end()) {
        return int(std::distance(m_value.begin(), found));
    }
    return -1;
}

template <typename T>
template <typename ToRemove>
bool List<T>::remove(ToRemove&& toRemove)
{
    if (auto found = find(std::forward<ToRemove>(toRemove)); found != end()) {
        m_value.erase(found);
        return true;
    }
    return false;
}

template <typename T>
template <typename Func>
void List<T>::sort(Func&& func)
{
    std::sort(m_value.begin(), m_value.end(), std::forward<Func>(func));
}

template <typename T>
void List<T>::sort()
{
    std::sort(m_value.begin(), m_value.end());
}

template <typename T>
template <typename Func>
List<T> List<T>::sorted(Func&& func) const
{
    auto copy = m_value;
    std::sort(copy.begin(), copy.end(), std::forward<Func>(func));
    return copy;
}

template <typename T>
List<T> List<T>::sorted() const
{
    auto copy = m_value;
    std::sort(copy.begin(), copy.end());
    return List<T>(std::move(copy));
}

template <typename T>
bool List<T>::empty() const
{
    return m_value.empty();
}

} // namespace pack
