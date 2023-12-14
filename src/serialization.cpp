#include "pack/serialization.h"
#ifdef WITH_QT
#include <QFile>
#endif
#include <fstream>

namespace pack {

static expected<UString> read(const UString& filename)
{
#ifdef WITH_QT
    QFile file(filename.toQString());
    if (file.open(QIODevice::ReadOnly)) {
        return UString(QString::fromUtf8(file.readAll()));
    }
#else
    std::ifstream st(filename.toStdString());
    if (st.is_open()) {
        return UString(std::string{std::istreambuf_iterator<char>(st), std::istreambuf_iterator<char>()});
    }
#endif
    return unexpected("Cannot read file {}"_s, filename);
}

static expected<void> write(const UString& filename, const UString& content)
{
#ifdef WITH_QT
    std::ofstream st(filename.toStdString());
#else
    std::ofstream st(filename.toStdString());
#endif
    if (st.is_open()) {
        st << content;
        st.close();
        return {};
    }
    return unexpected("Cannot read file {}"_s, filename);
}


expected<UString> serialize(Serializers serializer, const Attribute& node, Option opt)
{
    return unexpected("Unimplemented"_s);
}

expected<void> serializeFile(Serializers serializer, const UString& fileName, const Attribute& node, Option opt)
{
    if (auto content = serialize(serializer, node, opt)) {
        return write(fileName, *content);
    } else {
        return unexpected(content.error());
    }
}

expected<void> deserialize(Serializers serializer, const UString& content, Attribute& node)
{
    return unexpected("Unimplemented"_s);
}

expected<void> deserializeFile(Serializers serializer, const UString& fileName, Attribute& node)
{
    if (auto content = read(fileName)) {
        return deserialize(serializer, *content, node);
    } else {
        return unexpected(content.error());
    }
}

namespace json {
    expected<UString> serialize(const Attribute& node, Option opt)
    {
        return pack::serialize(Serializers::Json, node, opt);
    }

    expected<void> serializeFile(const UString& fileName, const Attribute& node, Option opt)
    {
        return pack::serializeFile(Serializers::Json, fileName, node, opt);
    }

    expected<void> deserialize(const UString& content, Attribute& node)
    {
        return pack::deserialize(Serializers::Json, content, node);
    }

    expected<void> deserializeFile(const UString& fileName, Attribute& node)
    {
        return deserializeFile(Serializers::Json, fileName, node);
    }
} // namespace json

namespace yaml {
    expected<UString> serialize(const Attribute& node, Option opt)
    {
        return pack::serialize(Serializers::Yaml, node, opt);
    }

    expected<void> serializeFile(const UString& fileName, const Attribute& node, Option opt)
    {
        return pack::serializeFile(Serializers::Yaml, fileName, node, opt);
    }

    expected<void> deserialize(const UString& content, Attribute& node)
    {
        return pack::deserialize(Serializers::Yaml, content, node);
    }

    expected<void> deserializeFile(const UString& fileName, Attribute& node)
    {
        return deserializeFile(Serializers::Yaml, fileName, node);
    }
} // namespace yaml

} // namespace pack
