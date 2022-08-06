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
#include "pack/serialization.h"
#include "pack/visitor.h"
#include "utils.h"
#include <yaml-cpp/yaml.h>

#ifdef WITH_QTSTRING
template <>
struct YAML::convert<QString>
{
    static Node encode(const QString& str)
    {
        return Node(str.toStdString());
    }

    static bool decode(const Node& node, QString& rhs)
    {
        if (!node.IsScalar()) {
            return false;
        }
        rhs = QString::fromStdString(node.Scalar());
        return true;
    }
};
#endif

template <>
struct YAML::convert<std::vector<std::byte>>
{
    static Node encode(const std::vector<std::byte>& data)
    {
        auto val = reinterpret_cast<const unsigned char*>(data.data());
        YAML::Binary bin(val, data.size());
        return Node(bin);
    }

    static bool decode(const Node& node, std::vector<std::byte>& data)
    {
        if (!node.IsScalar()) {
            return false;
        }

        auto bin = node.as<YAML::Binary>();
        auto val = reinterpret_cast<const std::byte*>(bin.data());

        data = std::vector<std::byte>(val, val+bin.size());
        return true;
    }
};

namespace pack::yaml {

// =========================================================================================================================================

template <Type ValType>
struct Convert
{
    using CppType = typename ResolveType<ValType>::type;

    static void decode(Value<ValType>& node, const YAML::Node& yaml)
    {
        node = yaml.as<CppType>();
    }

//    static void decode(List<Value<ValType>>& node, const YAML::Node& yaml)
//    {
//        for (const auto& it : yaml) {
//            node.append(it.as<CppType>());
//        }
//    }

//    static void decode(Map<Value<ValType>>& node, const YAML::Node& yaml)
//    {
//        for (const auto& it : yaml) {
//            node.append(it.first.as<string_t>(), it.second.as<CppType>());
//        }
//    }

    static void encode(const Value<ValType>& node, YAML::Node& yaml, Option /*opt*/)
    {
        yaml = YAML::convert<CppType>::encode(node.value());
    }

//    static void encode(const List<Value<ValType>>& node, YAML::Node& yaml, Option opt)
//    {
//        if (node.size()) {
//            if constexpr (ValType == Type::Bool) {
//                for (auto it : node) {
//                    yaml.push_back(YAML::convert<CppType>::encode(it));
//                }
//            } else {
//                for (const auto& it : node) {
//                    yaml.push_back(YAML::convert<CppType>::encode(it));
//                }
//            }
//        } else if (isSet(opt, Option::WithDefaults)) {
//            yaml = YAML::Node(YAML::NodeType::Sequence);
//        }
//    }

//    static void encode(const Map<Value<ValType>>& node, YAML::Node& yaml, Option opt)
//    {
//        if (node.size()) {
//            for (const auto& it : node) {
//                yaml[it.first] = YAML::convert<CppType>::encode(it.second);
//            }
//        } else if (isSet(opt, Option::WithDefaults)) {
//            yaml = YAML::Node(YAML::NodeType::Map);
//        }
//    }
};

// =========================================================================================================================================

class YamlDeserializer : public Deserialize<YamlDeserializer>
{
public:
    template <typename T>
    static void unpackValue(T& val, const YAML::Node& yaml)
    {
        Convert<T::ThisType>::decode(val, yaml);
    }

    static void unpackValue(IEnum& en, const YAML::Node& yaml)
    {
        en.fromString(yaml.as<string_t>());
    }

    static void unpackValue(IMap& map, const YAML::Node& yaml)
    {
        for (const auto& child : yaml) {
            string_t key = child.first.as<string_t>();
            auto&    obj = map.create(key);
            visit(obj, child.second);
        }
    }

    static void unpackValue(IList& list, const YAML::Node& yaml)
    {
        for (const auto& child : yaml) {
            auto& obj = list.create();
            visit(obj, child);
        }
    }

    static void unpackValue(INode& node, const YAML::Node& yaml)
    {
        for (auto& it : node.fields()) {
            auto found = yaml[it.get().key()];
            if (found.IsDefined()) {
                visit(it, found);
            }
        }
    }

    static void unpackValue(IVariant& var, const YAML::Node& yaml)
    {
        std::vector<string_t> keys;
        for (const auto& it : yaml) {
            keys.push_back(it.first.as<string_t>());
        }
        if (var.findBetter(keys)) {
            if (auto ptr = var.get()) {
                unpackValue(static_cast<INode&>(*ptr), yaml);
            }
        }
    }
};

// =========================================================================================================================================

class YamlSerializer : public Serialize<YamlSerializer>
{
public:
    template <typename T>
    static void packValue(const T& val, YAML::Node& yaml, Option opt)
    {
        if (val.hasValue() || isSet(opt, Option::WithDefaults)) {
            Convert<T::ThisType>::encode(val, yaml, opt);
        }
    }

    static void packValue(const IMap& val, YAML::Node& yaml, Option opt)
    {
        if (val.size()) {
            for (int i = 0; i < val.size(); ++i) {
                const auto&      key  = val.keyByIndex(i);
                const Attribute& node = val.get(key);
                YAML::Node       child;

                visit(node, child, opt);
                yaml[key] = child;
            }
        } else if (isSet(opt, Option::WithDefaults)) {
            yaml = YAML::Node(YAML::NodeType::Map);
        }
    }

    static void packValue(const IList& val, YAML::Node& yaml, Option opt)
    {
        if (val.size()) {
            for (int i = 0; i < val.size(); ++i) {
                const Attribute& node = val.get(i);
                YAML::Node       child;
                visit(node, child, opt);
                yaml.push_back(child);
            }
        } else if (isSet(opt, Option::WithDefaults)) {
            yaml = YAML::Node(YAML::NodeType::Sequence);
        }
    }

    static void packValue(const INode& node, YAML::Node& yaml, Option opt)
    {
        for (auto& it : node.fields()) {
            if (node.hasValue() || isSet(opt, Option::WithDefaults)) {
                YAML::Node child = yaml[it.get().key()];
                visit(it.get(), child, opt);
            }
        }
    }

    static void packValue(const IEnum& en, YAML::Node& yaml, Option /*opt*/)
    {
        yaml = YAML::convert<string_t>::encode(en.asString());
    }

    static void packValue(const IVariant& var, YAML::Node& yaml, Option opt)
    {
        if (auto ptr = var.get()) {
            packValue(static_cast<const INode&>(*ptr), yaml, opt);
        }
    }
};

// =========================================================================================================================================

Expected<string_t> serialize(const Attribute& node, Option opt)
{
    try {
        YAML::Node yaml;
        YamlSerializer::visit(node, yaml, opt);
        return fromStdString(YAML::Dump(yaml));
    } catch (const std::exception& e) {
        return unexpected(e.what());
    }
}

Expected<void> deserialize(const string_t& content, Attribute& node)
{
    try {
        YAML::Node yaml = YAML::Load(toStdString(content));
        YamlDeserializer::visit(node, yaml);
        return {};
    } catch (const std::exception& e) {
        return unexpected(e.what());
    }
}

Expected<void> deserializeFile(const string_t& fileName, Attribute& node)
{
    if (auto cnt = read(fileName)) {
        return deserialize(*cnt, node);
    } else {
        return unexpected(cnt.error());
    }
}

Expected<void> serializeFile(const string_t& fileName, const Attribute& node, Option opt)
{
    if (auto ret = serialize(node, opt)) {
        if (auto res = write(fileName, *ret); !res) {
            return unexpected(res.error());
        }
        return {};
    } else {
        return unexpected(ret.error());
    }
}

// =========================================================================================================================================

} // namespace pack::yaml
