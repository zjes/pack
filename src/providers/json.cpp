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
#include "pack/utils.h"
#include <nlohmann/json.hpp>

namespace pack::json {

// =========================================================================================================================================

template<typename Type>
Type get(const nlohmann::ordered_json& json)
{
#ifdef WITH_QTSTRING
    if constexpr (std::is_same_v<Type, QString>) {
        return QString::fromStdString(json.get<std::string>());
    } else if constexpr (std::is_same_v<Type, Binary>){
        auto bin = json.get_binary();
    } else {
        return json.get<Type>();
    }
#else
    return json.get<Type>();
#endif
}

template<typename Type>
void set(nlohmann::ordered_json& json, const Type& value)
{
#ifdef WITH_QTSTRING
    if constexpr (std::is_same_v<Type, QString>) {
        json = value.toStdString();
    } else {
        json = value;
    }
#else
    json = value;
#endif
}

template <Type ValType>
struct Convert
{
    using CppType = typename ResolveType<ValType>::type;

    static void decode(Value<ValType>& node, const nlohmann::ordered_json& json)
    {
        if (!json.is_null()) {
            try {
                node = get<CppType>(json);
            } catch (const nlohmann::json::type_error& /*err*/) {
                if constexpr (ValType != Type::Binary) {
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

class JsonSerializer : public Serialize<JsonSerializer>
{
public:
    template <typename T>
    static void packValue(const T& val, nlohmann::ordered_json& json, Option opt)
    {
        if (val.hasValue() || isSet(opt, Option::WithDefaults)) {
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
                json[toStdString(key)] = child;
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

    static void packValue(const INode& node, nlohmann::ordered_json& json, Option opt)
    {
        json = nlohmann::json::object();
        for (auto& it : node.fields()) {
            if (it.get().hasValue() || isSet(opt, Option::WithDefaults)) {
                nlohmann::ordered_json& child = json[toStdString(it.get().key())];
                visit(it.get(), child, opt);
            }
        }
    }

    static void packValue(const IEnum& en, nlohmann::ordered_json& json, Option /*opt*/)
    {
        json = toStdString(en.asString());
    }

    static void packValue(const IVariant& var, nlohmann::ordered_json& yaml, Option opt)
    {
        if (auto ptr = var.get()) {
            packValue(static_cast<const INode&>(*ptr), yaml, opt);
        }
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
        en.fromString(get<string_t>(json));
    }

    static void unpackValue(IMap& map, const nlohmann::ordered_json& json)
    {
        for (const auto& [key, value] : json.items()) {
            auto& obj = map.create(fromStdString(key));
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

    static void unpackValue(INode& node, const nlohmann::ordered_json& json)
    {
        for (auto& it : node.fields()) {
            if (json.contains(toStdString(it.get().key()))) {
                visit(it.get(), json[toStdString(it.get().key())]);
            }
        }
    }

    static void unpackValue(IVariant& var, const nlohmann::ordered_json& json)
    {
        std::vector<string_t> keys;
        for (const auto& it : json.items()) {
            keys.push_back(fromStdString(it.key()));
        }
        if (var.findBetter(keys)) {
            if (auto ptr = var.get()) {
                unpackValue(static_cast<INode&>(*ptr), json);
            }
        }
    }
};


// =========================================================================================================================================

Expected<string_t> serialize(const Attribute& node, Option opt)
{
    try {
        nlohmann::ordered_json json;
        JsonSerializer::visit(node, json, opt);
        if(isSet(opt, Option::PrettyPrint)) {
            return fromStdString(json.dump(4));
        } else {
            return fromStdString(json.dump());
        }
        
    } catch (const std::exception& e) {
        return unexpected(e.what());
    }
}

Expected<void> deserialize(const string_t& content, Attribute& node)
{
    try {
        nlohmann::ordered_json json = nlohmann::ordered_json::parse(toStdString(content));
        JsonDeserializer::visit(node, json);
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

} // namespace pack::json
