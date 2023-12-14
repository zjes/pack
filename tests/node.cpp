#include <catch2/catch.hpp>
#include <iostream>
#include <pack/pack.h>

namespace item {

struct Item : public pack::Node
{
    pack::String item = FIELD("item");
    META(Item, item);
};

} // namespace item

TEST_CASE("simple node")
{
    struct Person : public pack::Node
    {
        pack::String name = FIELD("name");

        META(Person, name);
    };

    struct Up : public Person
    {
        pack::String work = FIELD("work");

        META_BASE(Up, Person, work);
    };

    struct UpItem : public item::Item
    {
        pack::String work = FIELD("work");

        META_BASE(UpItem, item::Item, work);
    };

    Person p;
    CHECK(p.typeName() == "Person");

    p.name = "some name"_s;

    Up u;
    CHECK(u.typeName() == "Up");

    u.name = "u name"_s;
    u.work = "u work"_s;

    auto fieldCompare = [](const std::vector<pack::UString>& from, const std::vector<pack::UString>& to)
    {
        REQUIRE(from.size() == to.size());

        for (int i = 0; i < from.size(); ++i) {
            CHECK(from[i] == to[i]);
        }
    };

    fieldCompare(p.staticFieldNames(), {"name"_s});
    fieldCompare(u.staticFieldNames(), {"name"_s, "work"_s});

    for (const auto& field : u.meta().fields()) {
        std::cerr << "dn " << field->key() << std::endl;
    }

    Up u1 = u;
    std::cerr << "copy " << u1.name << " " << u1.work << std::endl;

    std::cerr << "cmp1 " << (u1 == u) << std::endl;

    u1.name = "different"_s;

    std::cerr << "cmp2 " << (u1 == u) << std::endl;

    UpItem ui;
    ui.item = "aaaa"_s;
    ui.work = "wwww"_s;
    std::cerr << "ui " << ui.item << " " << ui.work << std::endl;
}
