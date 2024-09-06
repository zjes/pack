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
#include <list>
#include <optional>

namespace pack {

// =========================================================================================================================================

/// Common interface to list data container
class IList : public Attribute
{
public:
    ~IList() override;

    /// Returns the size of the list
    [[nodiscard]] virtual int size() const = 0;

    /// Returns item by index
    [[nodiscard]] virtual const Attribute& get(int index) const = 0;

    /// emplace and return newly create element
    virtual Attribute& create() = 0;

    [[nodiscard]] virtual bool isValueList() const = 0;
    [[nodiscard]] virtual Type valueType() const   = 0;

protected:
    IList();

    template <typename... Options>
    explicit IList(Options&&... options)
    requires allIsOptions<Options...>
        : Attribute(NodeType::List, std::forward<Options>(options)...)
    {
    }
};

// =========================================================================================================================================

class Node;

template <typename T>
class List : public IList
{
public:
    using ListType      = std::list<T>;
    using Iterator      = typename ListType::iterator;
    using ConstIterator = typename ListType::const_iterator;
    using IsValueList   = std::is_base_of<IValue, T>;
    using ValueType     = typename T::CppType;

public:
    List(List&& other)
        : m_value(std::move(other.m_value))
    {
    }

    List(const List& other)
        : m_value(other.m_value)
    {
    }

    List& operator=(const List& other)
    {
        m_value = other.m_value;
        return *this;
    }

    List& operator=(List&& other)
    {
        m_value = std::move(other.m_value);
        return *this;
    }

    List(ListType&& value)
        : m_value(std::move(value))
    {
    }

    List(const ListType& value)
        : m_value(value)
    {
    }

    template <typename... Options>
    List(ListType&& value, Options&&... opts)
    requires isValueConstructable<T, ListType> && allIsOptions<Options...>
        : IList(std::forward<Options>(opts)...)
        , m_value(value)
    {
    }

    template <typename... Options>
    List(std::initializer_list<T> values, Options&&... opts)
    requires isValueConstructable<T, ValueType> && allIsOptions<Options...>
        : IList(std::forward<Options>(opts)...)
        , m_value(values)
    {
    }

    template <typename... Options>
    List(Options&&... opts)
    requires allIsOptions<Options...>
        : IList(std::forward<Options>(opts)...)
    {
    }

    template <typename Value>
    List(std::initializer_list<Value> values)
    requires(!isOption<Value>) && isValueConstructable<Value, T> && (!isSubtype<Node, Value>)
        : IList()
    {
        for (const auto& val : values) {
            m_value.emplace_back(val);
        }
    }

public:
    const ListType& toVector() const
    {
        return m_value;
    }

    ListType& toVector()
    {
        return m_value;
    }

    void setVector(const ListType& list)
    {
        m_value = list;
    }

    void setVector(ListType&& list)
    {
        m_value = std::move(list);
    }

    template <typename TT>
    void setVector(std::vector<TT>&& value)
    requires isValueConstructable<TT, ValueType>
    {
        m_value = ListType(m_value.end(), std::make_move_iterator(value.begin()), std::make_move_iterator(value.end()));
    }

    template <typename TT>
    void setVector(const std::vector<TT>& value)
    requires isValueConstructable<TT, ValueType>
    {
        m_value = ListType(m_value.end(), value.begin(), value.end());
    }

    void operator=(std::initializer_list<ValueType> values)
    {
        m_value = ListType(values.begin(), values.end());
    }


    bool operator==(const std::vector<ValueType>& values) const
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

    bool operator==(const List& values) const
    {
        return m_value == values.m_value;
    }

public:
    [[nodiscard]] int size() const override
    {
        return int(m_value.size());
    }

    [[nodiscard]] const Attribute& get(int index) const override
    {
        auto front = m_value.begin();
        std::advance(front, index);
        return *front;
    }

    Attribute& create() override
    {
        return append();
    }

    [[nodiscard]] bool isValueList() const override
    {
        return IsValueList::value;
    }

    [[nodiscard]] Type valueType() const override
    {
        if constexpr (IsValueList::value) {
            return T::ThisType;
        } else {
            return Type::Unknown;
        }
    }


public:
    ConstIterator begin() const
    {
        return m_value.begin();
    }

    ConstIterator end() const
    {
        return m_value.end();
    }

    Iterator begin()
    {
        return m_value.begin();
    }

    Iterator end()
    {
        return m_value.end();
    }

public:
    void append(const T& value)
    {
        m_value.emplace_back(value);
    }

    void append(T&& value)
    {
        m_value.emplace_back(value);
    }

    void append(List&& value)
    {
        m_value.insert(m_value.end(), std::make_move_iterator(value.begin()), std::make_move_iterator(value.end()));
    }

    void append(const List& value)
    {
        m_value.insert(m_value.end(), value.begin(), value.end());
    }

    // void append(List::ListType&& value)
    // {
    //     m_value.insert(m_value.end(), std::make_move_iterator(value.begin()), std::make_move_iterator(value.end()));
    // }

    // void append(const List::ListType& value)
    // {
    //     m_value.insert(m_value.end(), value.begin(), value.end());
    // }

    template <typename TT>
    void append(std::vector<TT>&& value)
    requires isValueConstructable<TT, ValueType>
    {
        m_value.insert(m_value.end(), std::make_move_iterator(value.begin()), std::make_move_iterator(value.end()));
    }

    template <typename TT>
    void append(const std::vector<TT>& value)
    requires isValueConstructable<TT, ValueType>
    {
        m_value.insert(m_value.end(), value.begin(), value.end());
    }

    T& append()
    {
        return m_value.emplace_back();
    }


    const T& operator[](int index) const
    {
        if (index < 0 || size_t(index) >= m_value.size()) {
            throw std::runtime_error("out of bounds");
        }
        return *std::next(m_value.begin(), index);
    }

    T& operator[](int index)
    {
        if (index < 0 || size_t(index) >= m_value.size()) {
            throw std::runtime_error("out of bounds");
        }
        return *std::next(m_value.begin(), index);
    }


    template <typename ToFind>
    std::optional<T> findItem(ToFind&& item) const
    {
        if (auto found = find(std::forward<ToFind>(item)); found != end()) {
            return *found;
        }
        return std::nullopt;
    }

    template <typename ToFind>
    ConstIterator find(ToFind&& func) const
    {
        if constexpr (std::is_invocable_r_v<bool, ToFind, T>) {
            return std::find_if(m_value.begin(), m_value.end(), std::forward<ToFind>(func));
        } else {
            return std::find(m_value.begin(), m_value.end(), func);
        }
        return m_value.end();
    }


    template <typename ToFind>
    int indexOf(ToFind&& toFind) const
    {
        if (auto found = find(std::forward<ToFind>(toFind)); found != end()) {
            return int(std::distance(m_value.begin(), found));
        }
        return -1;
    }

    template <typename ToRemove>
    bool remove(ToRemove&& toRemove)
    {
        if (auto found = find(std::forward<ToRemove>(toRemove)); found != end()) {
            m_value.erase(found);
            return true;
        }
        return false;
    }

    template <typename Func>
    void sort(Func&& func)
    {
        m_value.sort(std::forward<Func>(func));
    }

    void sort()
    {
        m_value.sort();
    }

    template <typename Func>
    List sorted(Func&& func) const
    {
        auto copy = m_value;
        copy.sort(std::forward<Func>(func));
        return copy;
    }

    List sorted() const
    {
        auto copy = m_value;
        copy.sort();
        return copy;
    }

public:
    [[nodiscard]] int compare(const Attribute& other) const override
    {
        if (auto casted = dynamic_cast<const List*>(&other)) {
            return casted->toVector() == m_value ? 0 : 1;
        }
        return -1;
    }

    [[nodiscard]] UString typeName() const override
    {
        return _typeName();
    }

    void set(const Attribute& other) override
    {
        if (auto casted = dynamic_cast<const List*>(&other)) {
            m_value = casted->toVector();
        }
    }

    void set(Attribute&& other) override
    {
        if (auto casted = dynamic_cast<const List*>(&other)) {
            m_value = std::move(casted->toVector());
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
        return std::format("List<{}>", T::typeInfo());
    }

private:
    ListType m_value;
};

} // namespace pack
