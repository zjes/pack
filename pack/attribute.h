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
#include "pack/types.h"
#include <pack/options.h>
#ifdef WITH_QT
#include <QObject>
#endif
namespace pack {

// =========================================================================================================================================


// =========================================================================================================================================

class Attribute
{
public:
    enum class NodeType
    {
        Node,
        Value,
        Enum,
        List,
        Map,
        Variant
    };

public:
    template <typename... Args>
    Attribute(NodeType type, const Args&... args);
    Attribute(NodeType type, const UString& key = {});
    Attribute(const Attribute&) = default;
    Attribute(Attribute&&)      = default;
    virtual ~Attribute();

    virtual int     compare(const Attribute& other) const = 0;
    virtual UString typeName() const                      = 0;
    virtual void    set(const Attribute& other)           = 0;
    virtual void    set(Attribute&& other)                = 0;
    virtual bool    empty() const                         = 0;
    virtual void    clear()                               = 0;

    const UString& key() const;

    int operator<=>(const Attribute& other) const;

    Attribute& operator=(const Attribute&) = default;
    Attribute& operator=(Attribute&&)      = default;

    NodeType type() const;

protected:
    UString  m_key;
    NodeType m_type;
};

// =========================================================================================================================================

template <typename... Args>
Attribute::Attribute(NodeType type, const Args&... args)
    : Attribute(type)
{
    if (auto ret = pickOption<Key>(args...); ret) {
        m_key = ret->value;
    }
}

// =========================================================================================================================================

} // namespace pack
