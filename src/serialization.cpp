#include "pack/serialization.h"
#include "providers/json.h"
#include "providers/protobuf.h"
#include "providers/yaml.h"
#include <fstream>

namespace pack {

static expected<UString> read(const UString& filename)
{
    std::ifstream st(filename.toStdString());
    if (st.is_open()) {
        return UString(std::string{std::istreambuf_iterator<char>(st), std::istreambuf_iterator<char>()});
    }
    return unexpected(std::format("Cannot read file {}", filename));
}

static expected<void> write(const UString& filename, const UString& content)
{
    std::ofstream st(filename.toStdString());
    if (st.is_open()) {
        st << content.toStdString();
        st.close();
        return {};
    }
    return unexpected(std::format("Cannot read file {}", filename));
}

class Serialization
{
public:
    template <typename T>
    static expected<UString> serialize(const Attribute& node, Option opt)
    {
        return T().run(node, opt);
    }
    template <typename T>
    static expected<void> deserialize(Attribute& node, const UString& content)
    {
        return T().run(node, content);
    }
};

expected<UString> serialize(Serializer serializer, const Attribute& node, Option opt)
{
    switch (serializer) {
        case Serializer::Json:
            return Serialization::serialize<JsonSerialization>(node, opt);
        case Serializer::Yaml:
            return Serialization::serialize<YamlSerialization>(node, opt);
        case Serializer::Protobuf:
            return Serialization::serialize<ProtobufSerialization>(node, opt);
        case Serializer::ProtobufText:
            return Serialization::serialize<ProtobufTextSerialization>(node, opt);
    }
    return unexpected(format("Unimplemented type: {}"_s, convert<UString>(serializer)));
}

expected<void> serializeFile(Serializer serializer, const UString& fileName, const Attribute& node, Option opt)
{
    if (auto content = serialize(serializer, node, opt)) {
        return write(fileName, *content);
    } else {
        return unexpected(content.error());
    }
}

expected<void> deserialize(Serializer serializer, const UString& content, Attribute& node)
{
    switch (serializer) {
        case Serializer::Json:
            return Serialization::deserialize<JsonDeserialization>(node, content);
        case Serializer::Yaml:
            return Serialization::deserialize<YamlDeserialization>(node, content);
        case Serializer::Protobuf:
            return Serialization::deserialize<ProtobufDeserialization>(node, content);
        case Serializer::ProtobufText:
            return Serialization::deserialize<ProtobufTextDeserialization>(node, content);
    }
    return unexpected(format("Unimplemented type: {}"_s, convert<UString>(serializer)));
}

expected<void> deserializeFile(Serializer serializer, const UString& fileName, Attribute& node)
{
    if (auto content = read(fileName)) {
        return deserialize(serializer, *content, node);
    } else {
        return unexpected(content.error());
    }
}

} // namespace pack
