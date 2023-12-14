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
#include <optional>

namespace pack {

// =========================================================================================================================================

/// Common interface to list data container
class IList : public Attribute
{
public:
    template <typename... Options>
    explicit IList(Options&&... options)
    requires allIsOptions<Options...>
        : Attribute(NodeType::List, std::forward<Options>(options)...)
    {
    }

    /// Returns the size of the list
    virtual int size() const = 0;

    /// Returns item by index
    virtual const Attribute& get(int index) const = 0;

    /// emplace and return newly create element
    virtual Attribute& create() = 0;

    virtual bool isValueList() const = 0;
    virtual Type valueType() const   = 0;
};

// =========================================================================================================================================

template <typename T>
class List : public IList
{
public:
    using ListType      = std::vector<T>;
    using Iterator      = typename ListType::iterator;
    using ConstIterator = typename ListType::const_iterator;
    using IsValueList   = std::is_base_of<IValue, T>;
    using ValueType     = typename T::CppType;

public:
    List(List&& value) = default;
    List(const List& value) = default;

    List& operator=(const List& other) = default;
    List& operator=(List&& other) = default;

    List(ListType&& value);
    List(const ListType& value);

    template <typename... Options>
    List(ListType&& value, Options&&... opts)
    requires isValueConstructable<T, ListType> && allIsOptions<Options...>;

    template <typename... Options>
    List(std::initializer_list<T> values, Options&&... opts)
    requires isValueConstructable<T, ValueType> && allIsOptions<Options...>;

    template <typename... Options>
    List(Options&&... opts)
    requires allIsOptions<Options...>;

public:
    const ListType& toVector() const;
    ListType&       toVector();

    void setVector(const ListType&);
    void setVector(ListType&&);

    template <typename TT>
    void setVector(std::vector<TT>&& value)
    requires isValueConstructable<TT, ValueType>;

    template <typename TT>
    void setVector(const std::vector<TT>& value)
    requires isValueConstructable<TT, ValueType>;

    void operator=(std::initializer_list<ValueType> values);
    bool operator==(const std::vector<ValueType>& values) const;
    bool operator==(const List& values) const;

public:
    int              size() const override;
    const Attribute& get(int index) const override;
    Attribute&       create() override;
    bool             isValueList() const override;
    Type             valueType() const override;

public:
    ConstIterator begin() const;
    ConstIterator end() const;
    Iterator      begin();
    Iterator      end();

public:
    void append(const T& value);
    void append(T&& value);
    void append(List&& value);
    void append(const List& value);
    void append(List::ListType&& value);
    void append(const List::ListType& value);

    template <typename TT>
    void append(std::vector<TT>&& value)
    requires isValueConstructable<TT, ValueType>;

    template <typename TT>
    void append(const std::vector<TT>& value)
    requires isValueConstructable<TT, ValueType>;

    T& append();

    const T& operator[](int index) const;
    T&       operator[](int index);

    template <typename ToFind>
    std::optional<T> findItem(ToFind&& func) const;
    template <typename ToFind>
    ConstIterator find(ToFind&& func) const;

    template <typename ToFind>
    int indexOf(ToFind&& func) const;

    template <typename ToRemove>
    bool remove(ToRemove&& toRemove);

    template <typename Func>
    void sort(Func&& func);
    void sort();

    template <typename Func>
    List sorted(Func&& func) const;
    List sorted() const;

public:
    int     compare(const Attribute& other) const override;
    UString typeName() const override;
    void    set(const Attribute& other) override;
    void    set(Attribute&& other) override;
    bool    empty() const override;
    void    clear() override;

    static UString _typeName();

private:
    ListType m_value;
};

} // namespace pack

#include "pack/types/private/list.inl" // IWYU pragma: keep
