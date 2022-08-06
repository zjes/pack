#include <catch2/catch.hpp>
#include <pack/pack.h>

TEST_CASE("String")
{
    SECTION("Initialization")
    {
        {
            pack::String str = "some"_s;
            REQUIRE(str.value() == "some"_s);
            REQUIRE(str == "some"_s);
        }
        {
            pack::String str{"dead parrot"_s, pack::Default("parrot"_s)};
            REQUIRE(str.value() == "dead parrot"_s);
            REQUIRE(str == "dead parrot"_s);
            REQUIRE(str.defValue() == "parrot"_s);
            REQUIRE(!str.empty());
        }
        {
            pack::String str{"dead parrot"_s, pack::Default("dead parrot"_s)};
            REQUIRE(!str.empty());
            REQUIRE(!str.hasValue());
        }
        {
            pack::String str{pack::Key("key"_s), pack::Default("dead parrot"_s)};
            REQUIRE(!str.empty());
            REQUIRE(!str.hasValue());
            REQUIRE(str.key() == "key"_s);
        }
        {
            pack::String str{pack::Key("key"_s)};
            REQUIRE(str.empty());
            REQUIRE(!str.hasValue());
            REQUIRE(str.key() == "key"_s);
        }
    }

    SECTION("comaparision")
    {
        pack::String str = "some"_s;
        REQUIRE(str == "some"_s);
        REQUIRE(str != "some1"_s);
        REQUIRE("some"_s == str);
        REQUIRE("some1"_s != str);

        pack::String str1 = "some"_s;
        REQUIRE(str.compare(str1));
        REQUIRE(str == str1);

        pack::String str2 = "some1"_s;
        REQUIRE(!str.compare(str2));
        REQUIRE(str != str2);
    }

    SECTION("copy/move")
    {
        {
            pack::String str  = "some"_s;
            pack::String str1 = str;
            REQUIRE(str == str1);
        }
        {
            pack::String str{"dead parrot"_s, pack::Default("dead parrot"_s)};
            pack::String str1 = str;
            REQUIRE(str == str1);
        }
        {
            pack::String str{pack::Key("key"_s), pack::Default("dead parrot"_s)};
            pack::String str1{pack::Key("key2"_s), pack::Default("dead parrot again"_s)};
            str1 = str;
            REQUIRE(!str.hasValue());
            REQUIRE(str1.hasValue());
            REQUIRE(str == str1);
            REQUIRE(str == "dead parrot"_s);
            REQUIRE(str1.key() == "key2"_s);
            REQUIRE(str1.defValue() == "dead parrot again"_s);
        }
        {
            pack::String str  = "some"_s;
            pack::String str1 = std::move(str);
            REQUIRE(str1 == "some"_s);
        }
        {
            pack::String str{"dead parrot"_s, pack::Default("dead parrot"_s)};
            pack::String str1 = std::move(str);
            REQUIRE(str1 == "dead parrot"_s);
            REQUIRE(pack::isEmpty(str1.defValue()));
        }
        {
            pack::String str{pack::Key("key"_s), pack::Default("dead parrot"_s)};
            pack::String str1{pack::Key("key2"_s), pack::Default("dead parrot again"_s)};
            str1 = std::move(str);

            CHECK(str1.hasValue());
            CHECK(str1 == "dead parrot"_s);
            CHECK(str1.key() == "key2"_s);
            CHECK(str1.defValue() == "dead parrot again"_s); // default should remain the same
        }
    }
}
