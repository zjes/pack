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
#include "pack/types/node.h"
#include "pack/formatter.h" // IWYU pragma: keep
#include "pack/serialization.h"
#include <algorithm>
#include <regex>

// =========================================================================================================================================

pack::INode::INode()
    : pack::Attribute(NodeType::Node)
{
}

// =========================================================================================================================================


pack::string_t pack::Node::dump() const
{
    if (auto cnt = yaml::serialize(*this)) {
        return *cnt;
    }
    return {};
}


pack::Expected<std::reference_wrapper<const pack::Attribute>> pack::Node::fieldByKey(const string_t& key) const
{
    const auto flds = fields();

    auto it = std::find_if(flds.begin(), flds.end(), [&](const auto& attr) {
        return attr.get().key() == key;
    });

    if (it != flds.end()) {
        return *it;
    }

    return unexpected("Field by key was {} not found"_s, key);
}

pack::Expected<std::reference_wrapper<const pack::Attribute>> pack::Node::fieldByName(const string_t& name) const
{
    auto names = fieldNames();
    auto it    = std::find(names.begin(), names.end(), name);
    if (it != names.end()) {
        return fields()[size_t(std::distance(names.begin(), it))];
    }
    return unexpected("Field by name was {} not found"_s, name);
}

bool pack::Node::compare(const pack::Attribute& other) const
{
    if (auto casted = dynamic_cast<const Node*>(&other)) {
        for (const auto& it : fields()) {
            auto ofield = casted->fieldByKey(it.get().key());
            if (!ofield) {
                return false;
            }

            if (!it.get().compare(*ofield)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

void pack::Node::set(const Attribute& other)
{
    if (auto casted = dynamic_cast<const Node*>(&other)) {
        for (const auto& it : fields()) {
            auto ofield = casted->fieldByKey(it.get().key());
            if (ofield) {
                it.get().set(*ofield);
            }
        }
    }
}

void pack::Node::set(Attribute&& other)
{
    if (auto casted = dynamic_cast<Node*>(&other)) {
        for (const auto& it : fields()) {
            auto ofield = casted->fieldByKey(it.get().key());
            if (ofield) {
                it.get().set(std::move(*ofield));
            }
        }
    }
}

bool pack::Node::hasValue() const
{
    for (const auto& it : fields()) {
        if (it.get().hasValue()) {
            return true;
        }
    }
    return false;
}

const std::string& pack::Node::fileDescriptor() const
{
    static std::string desc;
    return desc;
}

std::string pack::Node::protoName() const
{
    return toStdString(typeName());
}

void pack::Node::clear()
{
    for (auto& it : fields()) {
        it.get().clear();
    }
}

std::vector<pack::string_t> pack::split(const pack::string_t& str)
{
    try {
        static std::regex rgx(",?\\s+");
        std::string       copy = toStdString(str);

        std::vector<string_t>      ret;
        std::sregex_token_iterator iter(copy.begin(), copy.end(), rgx, -1);
        std::sregex_token_iterator end;
        for (; iter != end; ++iter)
            ret.push_back(fromStdString(*iter));
        return ret;
    } catch (const std::regex_error&) {
        return {str};
    }
}
