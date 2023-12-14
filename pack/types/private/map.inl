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

#include "pack/types/map.h"

namespace pack {

// =========================================================================================================================================

template <typename T>
template <typename... Options>
Map<T>::Map(Map<T>&& value, Options&&... opts)
requires allIsOptions<Options...>
    : IMap(std::forward<Options>(opts)...)
{
    setValue(std::move(value.m_value));
}

template <typename T>
template <typename... Options>
Map<T>::Map(MapType&& value, Options&&... opts)
requires allIsOptions<Options...>
    : IMap(std::forward<Options>(opts)...)
{
    setValue(std::move(value));
}

template <typename T>
template <typename... Options>
Map<T>::Map(const Map<T>& value, Options&&... opts)
requires allIsOptions<Options...>
    : IMap(std::forward<Options>(opts)...)
{
    setValue(value.m_value);
}

template <typename T>
template <typename... Options>
Map<T>::Map(const MapType& value, Options&&... opts)
requires allIsOptions<Options...>
    : IMap(std::forward<Options>(opts)...)
{
    setValue(value);
}

template <typename T>
template <typename... Options>
Map<T>::Map(Options&&... opts)
requires allIsOptions<Options...>
    : IMap(std::forward<Options>(opts)...)
{
}

#ifdef WITH_QT

template <typename T>
template <typename VT, typename... Options>
Map<T>::Map(const QHash<QString, VT>& map, Options&&... opts)
requires allIsOptions<Options...> && std::convertible_to<VT, typename T::CppType>
{
    for(const auto& [key, value] : map.asKeyValueRange()) {
        append(convert<UString>(key), convert<typename T::CppType>(value));
    }
}

template <typename T>
template <typename VT, typename... Options>
Map<T>::Map(const QMap<QString, VT>& map, Options&&... opts)
requires allIsOptions<Options...> && std::convertible_to<VT, typename T::CppType>
{
    for(const auto& [key, value] : map.asKeyValueRange()) {
        append(convert<UString>(key), convert<typename T::CppType>(value));
    }
}

#endif

template <typename T>
template <typename... Options>
Map<T>::Map(std::map<UString, typename T::CppType>&& map, Options&&... opts)
requires allIsOptions<Options...>
    : IMap(std::forward<Options>(opts)...)
{
    setValue(std::move(map));
}

template <typename T>
template <typename... Options>
Map<T>::Map(const std::map<UString, typename T::CppType>& map, Options&&... opts)
requires allIsOptions<Options...>
    : IMap(std::forward<Options>(opts)...)
{
    setValue(map);
}

// =========================================================================================================================================

template <typename T>
typename Map<T>::ConstIterator Map<T>::begin() const
{
    return m_value.begin();
}

template <typename T>
typename Map<T>::ConstIterator Map<T>::end() const
{
    return m_value.begin();
}

template <typename T>
typename Map<T>::Iterator Map<T>::begin()
{
    return m_value.end();
}

template <typename T>
typename Map<T>::Iterator Map<T>::end()
{
    return m_value.end();
}

// =========================================================================================================================================

template <typename T>
void Map<T>::setValue(const MapType& map)
{
    m_value = map;
}

template <typename T>
void Map<T>::setValue(MapType&& map)
{
    m_value = std::move(map);
}

template <typename T>
void Map<T>::setValue(const std::map<pack::UString, typename T::CppType>& map)
{
    m_value.clear();
    for (const auto& [key, value] : map) {
        m_value.emplace_back(key, value);
    }
}

template <typename T>
void Map<T>::setValue(std::map<pack::UString, typename T::CppType>&& map)
{
    m_value.clear();
    for (const auto& [key, value] : map) {
        m_value.emplace_back(std::move(key), std::move(value));
    }
}

#ifdef WITH_QT
template <typename T>
void Map<T>::setValue(const QHash<QString, typename T::CppType>& map)
{
    m_value.clear();
    for (const auto& key : map.keys()) {
        m_value.emplace_back(key, map[key]);
    }
}

template <typename T>
void Map<T>::setValue(QHash<QString, typename T::CppType>&& map)
{
    m_value.clear();
    for (const auto& key : map.keys()) {
        m_value.emplace_back(std::move(key), std::move(map[key]));
    }
}

template <typename T>
void Map<T>::setValue(const QMap<QString, typename T::CppType>& map)
{
    m_value.clear();
    for (const auto& key : map.keys()) {
        m_value.emplace_back(key, map[key]);
    }
}

template <typename T>
void Map<T>::setValue(QMap<QString, typename T::CppType>&& map)
{
    m_value.clear();
    for (const auto& key : map.keys()) {
        m_value.emplace_back(std::move(key), std::move(map[key]));
    }
}

#endif

// =========================================================================================================================================

template <typename T>
std::vector<UString> Map<T>::keys() const
{
    std::vector<UString> keys;
    for (const auto& [key, _] : m_value) {
        keys.push_back(key);
    }

    return keys;
}

template <typename T>
int Map<T>::size() const
{
    return int(m_value.size());
}

template <typename T>
const UString& Map<T>::keyByIndex(int index) const
{
    if (index < 0 || index >= int(m_value.size())) {
        throw std::out_of_range(fmt::format("Index '{}' was not found", index));
    }
    return m_value.at(size_t(index)).first;
}

template <typename T>
const Attribute& Map<T>::get(const UString& key) const
{
    auto found = std::find_if(m_value.begin(), m_value.end(), [&](const auto& pair) {
        return pair.first == key;
    });

    if (found != m_value.end()) {
        return found->second;
    }

    throw std::out_of_range(fmt::format("Key '{}' was not found", key));
}

template <typename T>
Attribute& Map<T>::create(const UString& key)
{
    return append(key);
}

template <typename T>
bool Map<T>::contains(const UString& key) const
{
    auto found = std::find_if(m_value.begin(), m_value.end(), [&](const auto& pair) {
        return pair.first == key;
    });
    return found != m_value.end();
}

template <typename T>
bool Map<T>::remove(const UString& key)
{
    auto found = std::find_if(m_value.begin(), m_value.end(), [&](const auto& pair) {
        return pair.first == key;
    });
    if (found != m_value.end()) {
        m_value.erase(found);
        return true;
    }
    return false;
}


// =========================================================================================================================================

template <typename T>
T& Map<T>::append(const UString& key)
{
    m_value.emplace_back(key, std::move(T{}));
    return m_value.back().second;
}

template <typename T>
void Map<T>::append(const UString& key, const T& val)
{
    m_value.emplace_back(key, val);
}

template <typename T>
void Map<T>::append(const UString& key, T&& val)
{
    m_value.emplace_back(key, std::move(val));
}

// =========================================================================================================================================

template <typename T>
int Map<T>::compare(const Attribute& other) const
{
    if (auto casted = dynamic_cast<const Map*>(&other)) {
        return casted->m_value == m_value ? 0 : 1;
    }
    return -1;
}

template <typename T>
UString Map<T>::typeName() const
{
    return _typeName();
}

template <typename T>
void Map<T>::set(const Attribute& other)
{
    if (auto casted = dynamic_cast<const Map*>(&other)) {
        setValue(casted->m_value);
    }
}

template <typename T>
void Map<T>::set(Attribute&& other)
{
    if (auto casted = dynamic_cast<const Map*>(&other)) {
        setValue(std::move(casted->m_value));
    }
}

template <typename T>
bool Map<T>::empty() const
{
    return m_value.empty();
}

template <typename T>
void Map<T>::clear()
{
    m_value.clear();
}

template <typename T>
UString Map<T>::_typeName()
{
    return pack::format("Map<{}>"_s, T::typeInfo());
}

// =========================================================================================================================================

template <typename T>
const T& Map<T>::operator[](const UString& key) const
{
    auto it = std::find_if(m_value.begin(), m_value.end(), [&key](const auto& pair) {
        return pair.first == key;
    });

    if (it != m_value.end()) {
        return it->second;
    }
    throw std::out_of_range(fmt::format("Key '{}' was not found", key));
}

template <typename T>
T& Map<T>::operator[](const UString& key)
{
    auto it = std::find_if(m_value.begin(), m_value.end(), [&key](const auto& pair) {
        return pair.first == key;
    });

    if (it != m_value.end()) {
        return it->second;
    }
    throw std::out_of_range(fmt::format("Key '{}' was not found", key));
}

// =========================================================================================================================================

} // namespace pack
