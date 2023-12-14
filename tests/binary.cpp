#include <catch2/catch.hpp>
#include <pack/pack.h>
#include <vector>

TEST_CASE("Binary")
{
    pack::Binary bin;
    bin.setString("parrot is dead"_s);
    CHECK(bin.asString() == "parrot is dead"_s);
    CHECK(bin.value()[0] == std::byte('p'));
}

