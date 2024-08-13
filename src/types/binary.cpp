#include "pack/types/binary.h"

namespace pack {

Binary::Binary()
    : Value<Type::Bytes>()
{
}

Binary::~Binary()                        = default;
Binary::Binary(const Binary&)            = default;
Binary::Binary(Binary&&)                 = default;
Binary& Binary::operator=(const Binary&) = default;
Binary& Binary::operator=(Binary&&)      = default;

Binary Binary::fromString(const UString& data)
{
    Binary bin;
    bin.setString(data);
    return bin;
}

void Binary::setString(const UString& data)
{
    std::string            copy = data.toStdString();
    std::vector<std::byte> array(copy.size());
    std::transform(copy.begin(), copy.end(), array.begin(), [](char c) {
        return std::byte(c);
    });
    setValue(array);
}

UString Binary::asString() const
{
    auto val = reinterpret_cast<const char*>(value().data());
    return UString(std::string(val, value().size()));
}

} // namespace pack
