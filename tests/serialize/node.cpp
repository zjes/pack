#include "tests/catch2.h"
#include <pack/pack.h>

namespace {

struct Parrot : public pack::Node
{
    enum class Color
    {
        Blue,
        Red,
        Green
    };

    pack::String      name  = FIELD("name");
    pack::Enum<Color> color = FIELD("color");

    META(Parrot, name, color);
};

struct EggOfParrot : public Parrot
{
    pack::Int32 size = FIELD("size");

    META_BASE(EggOfParrot, Parrot, size);
};

struct BunchOfParrots : public pack::Node
{
    pack::List<Parrot> parrots = FIELD("parrots");

    META(BunchOfParrots, parrots);
};

struct SickParrot : public pack::Node
{
    struct Parasite : public pack::Node
    {
        pack::String name = FIELD("name");
        META(Parasite, name);
    };

    pack::String sick     = FIELD("sick");
    Parasite     parasite = FIELD("parasite");
    META(SickParrot, sick, parasite);
};

} // namespace

TEST_CASE("Serialize/Node YAML")
{
    {
        Parrot parrot;
        parrot.color = Parrot::Color::Blue;
        parrot.name  = "norwegian"_s;

        auto res = pack::serialize(pack::Serializer::Yaml, parrot);
        CHECK(res);
        CHECK(*res == "name: norwegian\ncolor: Blue");
    }

    {
        EggOfParrot egg;
        egg.color = Parrot::Color::Green;
        egg.name  = "sibirian"_s;
        egg.size  = 42;
        auto res  = pack::serialize(pack::Serializer::Yaml, egg);
        CHECK(res);
        CHECK(*res == "size: 42\nname: sibirian\ncolor: Green");
    }

    {
        BunchOfParrots bunch;

        auto& parrot1 = bunch.parrots.append();
        parrot1.name  = "canadian"_s;
        parrot1.color = Parrot::Color::Blue;

        auto& parrot2 = bunch.parrots.append();
        parrot2.name  = "brazilian"_s;
        parrot2.color = Parrot::Color::Red;

        auto res = pack::serialize(pack::Serializer::Yaml, bunch);
        CHECK(res);
        CHECK(*res == "parrots:\n  - name: canadian\n    color: Blue\n  - name: brazilian\n    color: Red");
    }

    {
        SickParrot parrot;
        parrot.sick          = "parasite"_s;
        parrot.parasite.name = "worm"_s;
        auto res             = pack::serialize(pack::Serializer::Yaml, parrot);
        CHECK(res);
        CHECK(*res == "sick: parasite\nparasite:\n  name: worm");
    }
}

TEST_CASE("Serialize/Node Json")
{
    {
        Parrot parrot;
        parrot.color = Parrot::Color::Blue;
        parrot.name  = "norwegian"_s;

        auto res = pack::serialize(pack::Serializer::Json, parrot);
        CHECK(res);
        CHECK(*res == R"({"name":"norwegian","color":"Blue"})");
    }

    {
        EggOfParrot egg;
        egg.color = Parrot::Color::Green;
        egg.name  = "sibirian"_s;
        egg.size  = 42;
        auto res  = pack::serialize(pack::Serializer::Json, egg);
        CHECK(res);
        CHECK(*res == R"({"size":42,"name":"sibirian","color":"Green"})");
    }

    {
        BunchOfParrots bunch;

        auto& parrot1 = bunch.parrots.append();
        parrot1.name  = "canadian"_s;
        parrot1.color = Parrot::Color::Blue;

        auto& parrot2 = bunch.parrots.append();
        parrot2.name  = "brazilian"_s;
        parrot2.color = Parrot::Color::Red;

        auto res = pack::serialize(pack::Serializer::Json, bunch);
        CHECK(res);
        CHECK(*res == R"({"parrots":[{"name":"canadian","color":"Blue"},{"name":"brazilian","color":"Red"}]})");
    }

    {
        SickParrot parrot;
        parrot.sick          = "parasite"_s;
        parrot.parasite.name = "worm"_s;
        auto res             = pack::serialize(pack::Serializer::Json, parrot);
        CHECK(res);
        CHECK(*res == R"({"sick":"parasite","parasite":{"name":"worm"}})");
    }
}

TEST_CASE("Deserialize/Node YAML")
{
    {
        Parrot parrot;

        auto res = pack::deserialize(pack::Serializer::Yaml, "name: norwegian\ncolor: Blue", parrot);
        CHECK(res);
        CHECK(parrot.color == Parrot::Color::Blue);
        CHECK(parrot.name == "norwegian"_s);
    }

    {
        EggOfParrot egg;
        auto        res = pack::deserialize(pack::Serializer::Yaml, "size: 42\nname: sibirian\ncolor: Green", egg);
        CHECK(res);
        CHECK(egg.color == Parrot::Color::Green);
        CHECK(egg.name == "sibirian"_s);
        CHECK(egg.size == 42);
    }

    {
        BunchOfParrots bunch;

        auto res = pack::
            deserialize(pack::Serializer::Yaml, "parrots:\n  - name: canadian\n    color: Blue\n  - name: brazilian\n    color: Red", bunch);

        CHECK(res);

        REQUIRE(bunch.parrots.size() == 2);
        auto& parrot1 = bunch.parrots[0];
        CHECK(parrot1.name == "canadian"_s);
        CHECK(parrot1.color == Parrot::Color::Blue);

        auto& parrot2 = bunch.parrots[1];
        CHECK(parrot2.name == "brazilian"_s);
        CHECK(parrot2.color == Parrot::Color::Red);
    }

    {
        SickParrot parrot;
        auto       res = pack::deserialize(pack::Serializer::Yaml, "sick: parasite\nparasite:\n  name: worm", parrot);
        CHECK(res);
        CHECK(parrot.sick == "parasite"_s);
        CHECK(parrot.parasite.name == "worm"_s);
    }
}

TEST_CASE("Deserialize/Node JSON")
{
    {
        Parrot parrot;

        auto res = pack::deserialize(pack::Serializer::Yaml, R"({"name":"norwegian","color":"Blue"})", parrot);
        CHECK(res);
        CHECK(parrot.color == Parrot::Color::Blue);
        CHECK(parrot.name == "norwegian"_s);
    }

    {
        EggOfParrot egg;
        auto        res = pack::deserialize(pack::Serializer::Yaml, R"({"size":42,"name":"sibirian","color":"Green"})", egg);
        CHECK(res);
        CHECK(egg.color == Parrot::Color::Green);
        CHECK(egg.name == "sibirian"_s);
        CHECK(egg.size == 42);
    }

    {
        BunchOfParrots bunch;

        auto res = pack::deserialize(
            pack::Serializer::Yaml, R"({"parrots":[{"name":"canadian","color":"Blue"},{"name":"brazilian","color":"Red"}]})", bunch);

        CHECK(res);

        REQUIRE(bunch.parrots.size() == 2);
        auto& parrot1 = bunch.parrots[0];
        CHECK(parrot1.name == "canadian"_s);
        CHECK(parrot1.color == Parrot::Color::Blue);

        auto& parrot2 = bunch.parrots[1];
        CHECK(parrot2.name == "brazilian"_s);
        CHECK(parrot2.color == Parrot::Color::Red);
    }

    {
        SickParrot parrot;
        auto       res = pack::deserialize(pack::Serializer::Json, R"({"sick":"parasite","parasite":{"name":"worm"}})", parrot);
        CHECK(res);
        CHECK(parrot.sick == "parasite"_s);
        CHECK(parrot.parasite.name == "worm"_s);
    }
}
