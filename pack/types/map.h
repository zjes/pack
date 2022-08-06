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
#include "pack/types/value.h"
#include <algorithm>
#include <map>
#include <regex>

namespace pack {

// =========================================================================================================================================

class IMap : public Attribute
{
public:
    template <typename... Options>
    explicit IMap(Options&&... args)
        : Attribute(NodeType::Map, std::forward<Options>(args)...)
    {
    }

    virtual std::vector<string_t> keys() const                   = 0;
    virtual int                   size() const                   = 0;
    virtual const string_t&       keyByIndex(int index) const    = 0;
    virtual const Attribute&      get(const string_t& key) const = 0;
    virtual Attribute&            create(const string_t& key)    = 0;
};

// =========================================================================================================================================

template <typename T>
class Map : public IMap
{
public:
    using MapType       = std::vector<std::pair<string_t, T>>;
    using Iterator      = typename MapType::iterator;
    using ConstIterator = typename MapType::const_iterator;
    using IsValueMap    = std::is_base_of<IValue, T>;
    using ValueType     = typename T::CppType;

public:
    template <typename... Options, typename = isOptions<Options...>>
    Map(const Map& value, Options&&... opts);

    template <typename... Options, typename = isOptions<Options...>>
    Map(Map&& value, Options&&... opts);

    template <typename... Options, typename = isOptions<Options...>>
    Map(const MapType& value, Options&&... opts);

    template <typename... Options, typename = isOptions<Options...>>
    Map(MapType&& value, Options&&... opts);

    template <typename... Options, typename = isOptions<Options...>>
    Map(Options&&... opts);

#ifdef WITH_QTSTRING
    template <typename... Options, typename = isOptions<Options...>>
    Map(QHash<QString, typename T::CppType>&& map, Options&&... opts);

    template <typename... Options, typename = isOptions<Options...>>
    Map(const QHash<QString, typename T::CppType>& map, Options&&... opts);

    template <typename... Options, typename = isOptions<Options...>>
    Map(QMap<QString, typename T::CppType>&& map, Options&&... opts);

    template <typename... Options, typename = isOptions<Options...>>
    Map(const QMap<QString, typename T::CppType>& map, Options&&... opts);
#endif

    template <typename... Options, typename = isOptions<Options...>>
    Map(std::map<string_t, typename T::CppType>&& map, Options&&... opts);

    template <typename... Options, typename = isOptions<Options...>>
    Map(const std::map<string_t, typename T::CppType>& map, Options&&... opts);

public:
    ConstIterator begin() const;
    ConstIterator end() const;
    Iterator      begin();
    Iterator      end();

public:
    void setValue(const MapType& map);
    void setValue(MapType&& map);
    void setValue(const std::map<pack::string_t, typename T::CppType>& map);
    void setValue(std::map<pack::string_t, typename T::CppType>&& map);
#ifdef WITH_QTSTRING
    void setValue(QHash<QString, typename T::CppType>&& map);
    void setValue(const QHash<QString, typename T::CppType>& map);
    void setValue(QMap<QString, typename T::CppType>&& map);
    void setValue(const QMap<QString, typename T::CppType>& map);
#endif

public:
    T&   append(const string_t& key);
    void append(const string_t& key, const T& val);
    void append(const string_t& key, T&& val);

public:
    const T& operator[](const string_t& key) const;
    T&       operator[](const string_t& key);

public:
    std::vector<string_t> keys() const override;
    int                   size() const override;
    const string_t&       keyByIndex(int index) const override;
    const Attribute&      get(const string_t& key) const override;
    Attribute&            create(const string_t& key) override;
    bool                  contains(const string_t& key) const;
    bool                  remove(const string_t& key);

public:
    bool     compare(const Attribute& other) const override;
    string_t typeName() const override;
    void     set(const Attribute& other) override;
    void     set(Attribute&& other) override;
    bool     hasValue() const override;
    void     clear() override;

    static string_t _typeName();

private:
    MapType m_value;
};

// =========================================================================================================================================

} // namespace pack

#include "pack/types/private/map.inl" // IWYU pragma: keep
