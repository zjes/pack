#include "tests/protobuf/example.h"
#include <catch2/catch_all.hpp>
#include <pack/pack.h>

TEST_CASE("Protobuf")
{
    test::Person proto;
    proto.id        = 42;
    proto.doubleVal = 42.2;
    proto.email     = "someeml"_s;
    proto.ids       = {12, 13, 14};
    proto.name      = "name"_s;
    proto.names     = {"name1"_s, "name2"_s};

    auto it = pack::serialize(pack::Serializer::Protobuf, proto);
    REQUIRE(it);
    std::cerr << *pack::serialize(pack::Serializer::ProtobufText, proto) << std::endl;

    test::Person proto2;
    pack::deserialize(pack::Serializer::Protobuf, *it, proto2);

    CHECK(proto2 == proto);
}
