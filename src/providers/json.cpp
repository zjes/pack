#include "json.h"
#include "pack/visitor.h"
#include <nlohmann/json.hpp>

namespace pack {

// =========================================================================================================================================

template <Type ValType>
struct Convert
{
    using CppType = typename ResolveType<ValType>::type;

    template <typename Type>
    static Type get(const nlohmann::ordered_json& json)
    {
        if constexpr (std::is_same_v<Type, UString>) {
            return UString(json.get<std::string>());
        } else if constexpr (std::is_same_v<Type, Binary>) {
            auto bin = json.get_binary();
        } else {
            return json.get<Type>();
        }
    }

    template <typename Type>
    static void set(nlohmann::ordered_json& json, const Type& value)
    {
        if constexpr (std::is_same_v<Type, UString>) {
            json = value.toStdString();
        } else {
            json = value;
        }
    }

    static void decode(Value<ValType>& node, const nlohmann::ordered_json& json)
    {
        if (!json.is_null()) {
            try {
                node = get<CppType>(json);
            } catch (const nlohmann::json::type_error& /*err*/) {
                if constexpr (ValType != Type::Bytes) {
                    node = convert<CppType>(get<std::string>(json));
                }
            }
        }
    }

    static void encode(const Value<ValType>& node, nlohmann::ordered_json& json)
    {
        set(json, node.value());
    }
};

// =========================================================================================================================================

class JsonWorker : public Serialize<JsonWorker>
{
public:
    template <typename T>
    static void packValue(const T& val, nlohmann::ordered_json& json, Option opt)
    {
        if (!val.empty() || isSet(opt, Option::WithDefaults)) {
            Convert<T::ThisType>::encode(val, json);
        }
    }

    static void packValue(const IMap& val, nlohmann::ordered_json& json, Option opt)
    {
        if (val.size()) {
            for (int i = 0; i < val.size(); ++i) {
                const auto&            key  = val.keyByIndex(i);
                const Attribute&       node = val.get(key);
                nlohmann::ordered_json child;

                visit(node, child, opt);
                json[key.toStdString()] = child;
            }
        } else if (isSet(opt, Option::WithDefaults)) {
            json = nlohmann::json::object();
        }
    }

    static void packValue(const IList& val, nlohmann::ordered_json& json, Option opt)
    {
        if (val.size()) {
            for (int i = 0; i < val.size(); ++i) {
                const Attribute&       node = val.get(i);
                nlohmann::ordered_json child;
                visit(node, child, opt);
                json.push_back(child);
            }
        } else if (isSet(opt, Option::WithDefaults)) {
            json = nlohmann::json::array();
        }
    }

    static void packValue(const Node& node, nlohmann::ordered_json& json, Option opt)
    {
        json = nlohmann::json::object();
        for (auto& it : node.meta().fields()) {
            if (!node.empty() || isSet(opt, Option::WithDefaults)) {
                nlohmann::ordered_json& child = json[it->key().toStdString()];
                visit(*it, child, opt);
            }
        }
    }

    static void packValue(const IEnum& en, nlohmann::ordered_json& json, Option /*opt*/)
    {
        json = en.asString().toStdString();
    }
};

// =========================================================================================================================================

class JsonDeserializer : public Deserialize<JsonDeserializer>
{
public:
    template <typename T>
    static void unpackValue(T& val, const nlohmann::ordered_json& json)
    {
        Convert<T::ThisType>::decode(val, json);
    }

    static void unpackValue(IEnum& en, const nlohmann::ordered_json& json)
    {
        en.fromString(UString(json.get<std::string>()));
    }

    static void unpackValue(IMap& map, const nlohmann::ordered_json& json)
    {
        for (const auto& [key, value] : json.items()) {
            auto& obj = map.create(key);
            visit(obj, value);
        }
    }

    static void unpackValue(IList& list, const nlohmann::ordered_json& json)
    {
        for (const auto& child : json) {
            auto& obj = list.create();
            visit(obj, child);
        }
    }

    static void unpackValue(Node& node, const nlohmann::ordered_json& json)
    {
        for (auto& it : node.meta().fields()) {
            if (json.contains(it->key().toStdString())) {
                visit(*it, json[it->key().toStdString()]);
            }
        }
    }
};

// =========================================================================================================================================

expected<UString> JsonSerialization::run(const Attribute& node, Option opt)
{
    try {
        nlohmann::ordered_json json;
        JsonWorker::visit(node, json, opt);
        if (isSet(opt, Option::PrettyPrint)) {
            return UString(json.dump(4));
        } else {
            return UString(json.dump());
        }

    } catch (const std::exception& e) {
        return unexpected(e.what());
    }
}

expected<void> JsonDeserialization::run(Attribute& node, const UString& content)
{
    try {
        nlohmann::ordered_json json = nlohmann::ordered_json::parse(content.toStdString());
        JsonDeserializer::visit(node, json);
        return {};
    } catch (const std::exception& e) {
        return unexpected(e.what());
    }
}


} // namespace pack
