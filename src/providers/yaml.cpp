#include "yaml.h"
#include "pack/visitor.h"
#include <iostream>
#include <yaml-cpp/yaml.h>

template <>
struct YAML::convert<pack::UString>
{
    static Node encode(const pack::UString& str)
    {
        return Node(str.toStdString());
    }

    static bool decode(const Node& node, pack::UString& rhs)
    {
        if (!node.IsScalar()) {
            return false;
        }
        rhs = pack::UString(node.Scalar());
        return true;
    }
};

template <>
struct YAML::convert<std::vector<std::byte>>
{
    static Node encode(const std::vector<std::byte>& data)
    {
        auto         val = reinterpret_cast<const unsigned char*>(data.data());
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

        data = std::vector<std::byte>(val, val + bin.size());
        return true;
    }
};

namespace pack {

template <Type ValType>
struct Convert
{
    using CppType = typename ResolveType<ValType>::type;

    static void decode(Value<ValType>& node, const YAML::Node& yaml)
    {
        node = yaml.as<CppType>();
    }

    static void encode(const Value<ValType>& node, YAML::Node& yaml, Option /*opt*/)
    {
        yaml = YAML::convert<CppType>::encode(node.value());
    }
};

// =========================================================================================================================================

struct YamlWorker : public Serialize<YamlWorker>
{
    template <typename T>
    static void packValue(const T& val, YAML::Node& yaml, Option opt)
    {
        if (!val.empty() || isSet(opt, Option::WithDefaults)) {
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

    static void packValue(const Node& node, YAML::Node& yaml, Option opt)
    {
        for (auto& it : node.meta().fields()) {
            if (!node.empty() || isSet(opt, Option::WithDefaults)) {
                YAML::Node child = yaml[it->key()];
                visit(*it, child, opt);
            }
        }
    }

    static void packValue(const IEnum& en, YAML::Node& yaml, Option /*opt*/)
    {
        yaml = YAML::convert<UString>::encode(en.asString());
    }
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
        en.fromString(yaml.as<UString>());
    }

    static void unpackValue(IMap& map, const YAML::Node& yaml)
    {
        for (const auto& child : yaml) {
            auto  key = child.first.as<UString>();
            auto& obj = map.create(key);
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

    static void unpackValue(Node& node, const YAML::Node& yaml)
    {
        for (auto& it : node.meta().fields()) {
            auto found = yaml[it->key()];
            if (found.IsDefined()) {
                visit(*it, found);
            }
        }
    }
};

// =========================================================================================================================================

expected<UString> YamlSerialization::run(const Attribute& node, Option opt)
{
    try {
        YAML::Node yaml;
        YamlWorker::visit(node, yaml, opt);
        return YAML::Dump(yaml);
    } catch (const std::exception& e) {
        return unexpected(e.what());
    }
}

expected<void> YamlDeserialization::run(Attribute& node, const UString& content)
{
    try {
        YAML::Node yaml = YAML::Load(content.toStdString());
        YamlDeserializer::visit(node, yaml);
        return {};
    } catch (const std::exception& e) {
        return unexpected(e.what());
    }
}

} // namespace pack
