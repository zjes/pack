#include <catch2/catch.hpp>
#include <pack/pack.h>

TEST_CASE("Value")
{
    SECTION("Int")
    {
        pack::Int32 def;
        REQUIRE(def.value() == 0);
        REQUIRE(def.defValue() == 0);
        REQUIRE(!def.hasValue());

        pack::Int32 val(42);
        REQUIRE(val.value() == 42);
        REQUIRE(val.defValue() == 0);
        REQUIRE(val.hasValue());

        pack::Int32 val1 = 42;
        REQUIRE(val1.value() == 42);
        REQUIRE(val1.defValue() == 0);
        REQUIRE(val1.hasValue());

        pack::Int32 val2(42, pack::Default(11));
        REQUIRE(val2.value() == 42);
        REQUIRE(val2.defValue() == 11);
        REQUIRE(val2.hasValue());

        pack::Int32 val3(42, pack::Default(11), pack::Key("some key"_s));
        REQUIRE(val3.value() == 42);
        REQUIRE(val3 == 42);
        REQUIRE(val3 != 11);
        REQUIRE(val3.defValue() == 11);
        REQUIRE(val3.key() == "some key"_s);
        REQUIRE(val3.hasValue());
        REQUIRE(val3.valueType() == pack::Type::Int32);
        REQUIRE(val3.typeName() == "Value<Int32>"_s);

        REQUIRE(val == val3);
        REQUIRE(val != def);

        val3.clear();
        REQUIRE(val3 == 11);
        REQUIRE(val3.defValue() == 11);
        REQUIRE(!val3.hasValue());

        val1.clear();
        REQUIRE(val1 == 0);
        REQUIRE(!val1.hasValue());

        val1 = 10;
        val1 += 12;
        REQUIRE(val1 == 22);

        auto val4 = val1 + 20;
        REQUIRE(val4 == 42);

        val4 *= 2;
        REQUIRE(val4 == 84);

        val4 /= 2;
        REQUIRE(val4 == 42);

        val4 -= 2;
        REQUIRE(val4 == 40);
    }

    SECTION("Float")
    {
        pack::Float def;
        REQUIRE(def.value() == 0.f);
        REQUIRE(def.defValue() == 0.f);
        REQUIRE(!def.hasValue());

        pack::Float val(42.f);
        REQUIRE(val.value() == 42.f);
        REQUIRE(val.defValue() == 0.f);
        REQUIRE(val.hasValue());

        pack::Float val1 = 42.f;
        REQUIRE(val1.value() == 42.f);
        REQUIRE(val1.defValue() == 0.f);
        REQUIRE(val1.hasValue());

        pack::Float val2(42.f, pack::Default(11.f));
        REQUIRE(val2.value() == 42.f);
        REQUIRE(val2.defValue() == 11.f);
        REQUIRE(val2.hasValue());

        pack::Float val3(42.f, pack::Default(11.f), pack::Key("some key"_s));
        REQUIRE(val3.value() == 42.f);
        REQUIRE(val3 == 42.f);
        REQUIRE(val3 != 11.f);
        REQUIRE(val3.defValue() == 11.f);
        REQUIRE(val3.key() == "some key"_s);
        REQUIRE(val3.hasValue());
        REQUIRE(val3.valueType() == pack::Type::Float);
        REQUIRE(val3.typeName() == "Value<Float>"_s);

        REQUIRE(val == val3);
        REQUIRE(val != def);

        val3.clear();
        REQUIRE(val3 == 11.f);
        REQUIRE(val3.defValue() == 11.f);
        REQUIRE(!val3.hasValue());

        val1.clear();
        REQUIRE(val1 == 0.f);
        REQUIRE(!val1.hasValue());
    }

    SECTION("String")
    {
        pack::String def;
        REQUIRE(pack::isEmpty(def.value()));
        REQUIRE(pack::isEmpty(def.defValue()));
        REQUIRE(!def.hasValue());

        pack::String val("parrot"_s);
        REQUIRE(val.value() == "parrot"_s);
        REQUIRE(pack::isEmpty(val.defValue()));
        REQUIRE(val.hasValue());

        pack::String val1 = "parrot"_s;
        REQUIRE(val1.value() == "parrot"_s);
        REQUIRE(pack::isEmpty(val1.defValue()));
        REQUIRE(val1.hasValue());

        pack::String val2("parrot"_s, pack::Default("dead"_s));
        REQUIRE(val2.value() == "parrot"_s);
        REQUIRE(val2.defValue() == "dead"_s);
        REQUIRE(val2.hasValue());

        pack::String val3("parrot"_s, pack::Default("dead"_s), pack::Key("some key"_s));
        REQUIRE(val3.value() == "parrot"_s);
        REQUIRE(val3 == "parrot"_s);
        REQUIRE(val3 != "alive"_s);
        REQUIRE(val3.defValue() == "dead"_s);
        REQUIRE(val3.key() == "some key"_s);
        REQUIRE(val3.hasValue());
        REQUIRE(val3.valueType() == pack::Type::String);
        REQUIRE(val3.typeName() == "Value<String>"_s);

        REQUIRE(val == val3);
        REQUIRE(val != def);

        val3.clear();
        REQUIRE(val3 == "dead"_s);
        REQUIRE(val3.defValue() == "dead"_s);
        REQUIRE(!val3.hasValue());

        val1.clear();
        REQUIRE(val1.empty());
        REQUIRE(!val1.hasValue());

        val1 = "Parrot"_s;
        CHECK(val1 == "Parrot"_s);
        val1 += " is"_s;
        CHECK(val1 == "Parrot is"_s);

        pack::string_t res = val1 + " dead"_s;
        CHECK(res == "Parrot is dead"_s);
        pack::string_t res1 = "dead "_s + val1;
        CHECK(res1 == "dead Parrot is"_s);

        pack::String res2 = val1 + " dead"_s;
        CHECK(res2 == "Parrot is dead"_s);
        pack::String res3 = "dead "_s + val1;
        CHECK(res3 == "dead Parrot is"_s);
    }

    SECTION("Copy")
    {
        pack::Int64 val1(42, pack::Default(11));
        pack::Int64 val2(33, pack::Default(12));
        CHECK(val1 == 42);
        CHECK(val1.defValue() == 11);
        CHECK(val2 == 33);
        CHECK(val2.defValue() == 12);

        val1 = val2;

        CHECK(val1 == 33);
        CHECK(val1.defValue() == 11); // default should remain the same
    }

    SECTION("Move")
    {
        pack::Int64 val1(42, pack::Default(11));
        pack::Int64 val2(33, pack::Default(12));
        CHECK(val1 == 42);
        CHECK(val1.defValue() == 11);
        CHECK(val2 == 33);
        CHECK(val2.defValue() == 12);

        val1 = std::move(val2);

        CHECK(val1 == 33);
        CHECK(val1.defValue() == 11); // default should remain the same
    }
}
