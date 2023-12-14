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
#include <vector>

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

    virtual std::vector<UString> keys() const                  = 0;
    virtual int                  size() const                  = 0;
    virtual const UString&       keyByIndex(int index) const   = 0;
    virtual const Attribute&     get(const UString& key) const = 0;
    virtual Attribute&           create(const UString& key)    = 0;
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
    requires allIsOptions<Options...>;

    template <typename... Options>
    Map(MapType&& value, Options&&... opts)
    requires allIsOptions<Options...>;

    template <typename... Options>
    Map(const Map& value, Options&&... opts)
    requires allIsOptions<Options...>;

    template <typename... Options>
    Map(const MapType& value, Options&&... opts)
    requires allIsOptions<Options...>;

    template <typename... Options>
    Map(Options&&... opts)
    requires allIsOptions<Options...>;

#ifdef WITH_QT
    template <typename VT, typename... Options>
    Map(const QHash<QString, VT>& map, Options&&... opts)
    requires allIsOptions<Options...> && std::convertible_to<VT, typename T::CppType>;

    template <typename VT, typename... Options>
    Map(const QMap<QString, VT>& map, Options&&... opts)
    requires allIsOptions<Options...> && std::convertible_to<VT, typename T::CppType>;
#endif

    template <typename... Options>
    Map(std::map<UString, typename T::CppType>&& map, Options&&... opts)
    requires allIsOptions<Options...>;

    template <typename... Options>
    Map(const std::map<UString, typename T::CppType>& map, Options&&... opts)
    requires allIsOptions<Options...>;

public:
    ConstIterator begin() const;
    ConstIterator end() const;
    Iterator      begin();
    Iterator      end();

public:
    void setValue(const MapType& map);
    void setValue(MapType&& map);
    void setValue(const std::map<UString, typename T::CppType>& map);
    void setValue(std::map<UString, typename T::CppType>&& map);
#ifdef WITH_QT
    void setValue(QHash<QString, typename T::CppType>&& map);
    void setValue(const QHash<QString, typename T::CppType>& map);
    void setValue(QMap<QString, typename T::CppType>&& map);
    void setValue(const QMap<QString, typename T::CppType>& map);
#endif

public:
    T&   append(const UString& key);
    void append(const UString& key, const T& val);
    void append(const UString& key, T&& val);

public:
    const T& operator[](const UString& key) const;
    T&       operator[](const UString& key);

public:
    std::vector<UString> keys() const override;
    int                  size() const override;
    const UString&       keyByIndex(int index) const override;
    const Attribute&     get(const UString& key) const override;
    Attribute&           create(const UString& key) override;
    bool                 contains(const UString& key) const;
    bool                 remove(const UString& key);

public:
    int     compare(const Attribute& other) const override;
    UString typeName() const override;
    void    set(const Attribute& other) override;
    void    set(Attribute&& other) override;
    bool    empty() const override;
    void    clear() override;

    static UString _typeName();

private:
    MapType m_value;
};

// =========================================================================================================================================

} // namespace pack

#include "pack/types/private/map.inl" // IWYU pragma: keep
