#pragma once
#include <pack/ustring.h>
#include <pack/attribute.h>
#include <algorithm>

namespace pack {

class Meta
{
public:
    template <typename... Items>
    Meta(const UString& name, Items&... items)
        : m_name(name)
        , m_fields(std::vector<Attribute*>{&items...})
    {
    }

    template <typename... Items>
    Meta(const UString& name, const Meta& parent, Items&... items)
        : m_name(name)
        , m_fields(std::vector<Attribute*>{&items...})
    {
        m_fields.insert(m_fields.end(), parent.m_fields.begin(), parent.m_fields.end());
    }

    const std::vector<Attribute*>& fields() const
    {
        return m_fields;
    }

    std::vector<Attribute*>& fields()
    {
        return m_fields;
    }

    const Attribute* field(const UString& name) const
    {
        auto it = std::find_if(m_fields.begin(), m_fields.end(), [&name](const auto* fld) {
            return fld->key() == name;
        });
        return it != m_fields.end() ? *it : nullptr;
    }

private:
    friend class Node;

    UString                 m_name;
    std::vector<Attribute*> m_fields;
};

}
