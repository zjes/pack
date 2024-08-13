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
#include "pack/types/value.h"
#include <algorithm>
#include <map>
#include <vector>

namespace pack {

// =========================================================================================================================================

class IMap : public Attribute
{
public:
    ~IMap() override;

    template <typename... Options>
    explicit IMap(Options&&... args)
        : Attribute(NodeType::Map, std::forward<Options>(args)...)
    {
    }

    [[nodiscard]] virtual std::vector<UString> keys() const                  = 0;
    [[nodiscard]] virtual int                  size() const                  = 0;
    [[nodiscard]] virtual const UString&       keyByIndex(int index) const   = 0;
    [[nodiscard]] virtual const Attribute&     get(const UString& key) const = 0;
    virtual Attribute&                         create(const UString& key)    = 0;
};

// =========================================================================================================================================

template <typename T>
class Map : public IMap
{
public:
    using MapType       = std::vector<std::pair<UString, T>>;
    using Iterator      = typename MapType::iterator;
    using ConstIterator = typename MapType::const_iterator;
    using IsValueMap    = std::is_base_of<IValue, T>;
    using ValueType     = typename T::CppType;

public:
    template <typename... Options>
    Map(Map&& value, Options&&... opts)
    requires allIsOptions<Options...>
        : IMap(std::forward<Options>(opts)...)
    {
        setValue(std::move(value.m_value));
    }

    template <typename... Options>
    Map(MapType&& value, Options&&... opts)
    requires allIsOptions<Options...>
        : IMap(std::forward<Options>(opts)...)
    {
        setValue(std::move(value));
    }

    template <typename... Options>
    Map(const Map& value, Options&&... opts)
    requires allIsOptions<Options...>
        : IMap(std::forward<Options>(opts)...)
    {
        setValue(value.m_value);
    }

    template <typename... Options>
    Map(const MapType& value, Options&&... opts)
    requires allIsOptions<Options...>
        : IMap(std::forward<Options>(opts)...)
    {
        setValue(value);
    }

    template <typename... Options>
    Map(Options&&... opts)
    requires allIsOptions<Options...>
        : IMap(std::forward<Options>(opts)...)
    {
    }

    Map(const std::initializer_list<typename MapType::value_type>& value)
    {
        setValue(value);
    }

#ifdef WITH_QT
    template <typename VT, typename... Options>
    Map(const QHash<QString, VT>& map, Options&&... opts)
    requires allIsOptions<Options...> && std::convertible_to<VT, ValueType>
        : IMap(std::forward<Options>(opts)...)
    {
        for (const auto& [key, value] : map.asKeyValueRange()) {
            append(convert<UString>(key), convert<ValueType>(value));
        }
    }

    template <typename VT, typename... Options>
    Map(const QMap<QString, VT>& map, Options&&... opts)
    requires allIsOptions<Options...> && std::convertible_to<VT, ValueType>
        : IMap(std::forward<Options>(opts)...)
    {
        for (const auto& [key, value] : map.asKeyValueRange()) {
            append(convert<UString>(key), convert<ValueType>(value));
        }
    }
#endif

    template <typename... Options>
    Map(std::map<UString, ValueType>&& map, Options&&... opts)
    requires allIsOptions<Options...>
        : IMap(std::forward<Options>(opts)...)
    {
        setValue(std::move(map));
    }

    template <typename... Options>
    Map(const std::map<UString, ValueType>& map, Options&&... opts)
    requires allIsOptions<Options...>
        : IMap(std::forward<Options>(opts)...)
    {
        setValue(map);
    }

public:
    ConstIterator begin() const
    {
        return m_value.begin();
    }

    ConstIterator end() const
    {
        return m_value.begin();
    }

    Iterator begin()
    {
        return m_value.end();
    }

    Iterator end()
    {
        return m_value.end();
    }

public:
    void setValue(const MapType& map)
    {
        m_value = map;
    }

    void setValue(MapType&& map)
    {
        m_value = std::move(map);
    }

    void setValue(const std::map<UString, ValueType>& map)
    {
        m_value.clear();
        for (const auto& [key, value] : map) {
            m_value.emplace_back(key, value);
        }
    }

    void setValue(std::map<UString, ValueType>&& map)
    {
        m_value.clear();
        for (const auto& [key, value] : map) {
            m_value.emplace_back(std::move(key), std::move(value));
        }
    }

#ifdef WITH_QT
    void setValue(QHash<QString, ValueType>&& map)
    {
        m_value.clear();
        for (const auto& key : map.keys()) {
            m_value.emplace_back(key, map[key]);
        }
    }

    void setValue(const QHash<QString, ValueType>& map)
    {
        m_value.clear();
        for (const auto& key : map.keys()) {
            m_value.emplace_back(std::move(key), std::move(map[key]));
        }
    }

    void setValue(QMap<QString, ValueType>&& map)
    {
        m_value.clear();
        for (const auto& key : map.keys()) {
            m_value.emplace_back(key, map[key]);
        }
    }

    void setValue(const QMap<QString, ValueType>& map)
    {
        m_value.clear();
        for (const auto& key : map.keys()) {
            m_value.emplace_back(std::move(key), std::move(map[key]));
        }
    }
#endif

public:
    T& append(const UString& key)
    {
        m_value.emplace_back(key, std::move(T{}));
        return m_value.back().second;
    }

    void append(const UString& key, const T& val)
    {
        m_value.emplace_back(key, val);
    }

    void append(const UString& key, T&& val)
    {
        m_value.emplace_back(key, std::move(val));
    }

public:
    const T& operator[](const UString& key) const
    {
        auto it = std::find_if(m_value.begin(), m_value.end(), [&key](const auto& pair) {
            return pair.first == key;
        });

        if (it != m_value.end()) {
            return it->second;
        }
        throw std::out_of_range(std::format("Key '{}' was not found", key));
    }

    T& operator[](const UString& key)
    {
        auto it = std::find_if(m_value.begin(), m_value.end(), [&key](const auto& pair) {
            return pair.first == key;
        });

        if (it != m_value.end()) {
            return it->second;
        }
        throw std::out_of_range(std::format("Key '{}' was not found", key));
    }

public:
    [[nodiscard]] std::vector<UString> keys() const override
    {
        std::vector<UString> keys;
        for (const auto& [key, _] : m_value) {
            keys.push_back(key);
        }

        return keys;
    }

    [[nodiscard]] int size() const override
    {
        return int(m_value.size());
    }

    [[nodiscard]] const UString& keyByIndex(int index) const override
    {
        if (index < 0 || index >= int(m_value.size())) {
            throw std::out_of_range(std::format("Index '{}' was not found", index));
        }
        return m_value.at(size_t(index)).first;
    }

    [[nodiscard]] const Attribute& get(const UString& key) const override
    {
        auto found = std::find_if(m_value.begin(), m_value.end(), [&](const auto& pair) {
            return pair.first == key;
        });

        if (found != m_value.end()) {
            return found->second;
        }

        throw std::out_of_range(std::format("Key '{}' was not found", key));
    }

    [[nodiscard]] Attribute& create(const UString& key) override
    {
        return append(key);
    }

    [[nodiscard]] bool contains(const UString& key) const
    {
        auto found = std::find_if(m_value.begin(), m_value.end(), [&](const auto& pair) {
            return pair.first == key;
        });
        return found != m_value.end();
    }

    bool remove(const UString& key)
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

public:
    [[nodiscard]] int compare(const Attribute& other) const override
    {
        if (auto casted = dynamic_cast<const Map*>(&other)) {
            return casted->m_value == m_value ? 0 : 1;
        }
        return -1;
    }

    [[nodiscard]] UString typeName() const override
    {
        return _typeName();
    }

    void set(const Attribute& other) override
    {
        if (auto casted = dynamic_cast<const Map*>(&other)) {
            setValue(casted->m_value);
        }
    }

    void set(Attribute&& other) override
    {
        if (auto casted = dynamic_cast<const Map*>(&other)) {
            setValue(std::move(casted->m_value));
        }
    }

    [[nodiscard]] bool empty() const override
    {
        return m_value.empty();
    }

    void clear() override
    {
        m_value.clear();
    }


    static UString _typeName()
    {
        return pack::format("Map<{}>"_s, T::typeInfo());
    }

private:
    MapType m_value;
};

// =========================================================================================================================================

} // namespace pack
