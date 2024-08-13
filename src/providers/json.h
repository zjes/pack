#pragma once

#include "pack/attribute.h"
#include "pack/serialization.h"

namespace pack {

class JsonSerialization
{
public:
    static expected<UString> run(const Attribute& node, Option opt);
};

class JsonDeserialization
{
public:
    static expected<void> run(Attribute& node, const UString& content);
};

} // namespace pack
