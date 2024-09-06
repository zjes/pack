#pragma once

#include "pack/attribute.h"
#include "pack/serialization.h"

namespace pack {

class ProtobufSerialization
{
public:
    static expected<UString> run(const Attribute& node, Option opt);
};

class ProtobufDeserialization
{
public:
    static expected<void> run(Attribute& node, const UString& content);
};

class ProtobufTextSerialization
{
public:
    static expected<UString> run(const Attribute& node, Option opt);
};

class ProtobufTextDeserialization
{
public:
    static expected<void> run(Attribute& node, const UString& content);
};

} // namespace pack
