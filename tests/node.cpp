#include <catch2/catch_all.hpp>
#include <iostream>
#include <pack/pack.h>

namespace {

struct Item : public pack::Node
{
    pack::String item = FIELD("item");
    META(Item, item);
};

struct ItemList : public pack::Node
{
    pack::Int32List lst = FIELD("lst");
    META(ItemList, lst);
};

} // namespace

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

    struct UpItem : public Item
    {
        pack::String work = FIELD("work");

        META_BASE(UpItem, Item, work);
    };

    Person p;
    CHECK(p.typeName() == "Person");

    p.name = "some name"_s;

    Up u;
    CHECK(u.typeName() == "Up");

    u.name = "u name"_s;
    u.work = "u work"_s;

    auto fieldCompare = [](const std::vector<pack::UString>& from, const std::vector<pack::UString>& to) {
        REQUIRE(from.size() == to.size());

        for (size_t i = 0; i < from.size(); ++i) {
            CHECK(from[i] == to[i]);
        }
    };

    fieldCompare(p.staticFieldNames(), {"name"_s});
    fieldCompare(u.staticFieldNames(), {"name"_s, "work"_s});

    for (const auto& field : u.meta().fields()) {
        std::cerr << "dn " << field->key() << std::endl;
    }

    Up u1   = u;
    u1.name = "different"_s;

    UpItem ui;
    ui.item = "aaaa"_s;
    ui.work = "wwww"_s;
}

TEST_CASE("list node")
{
    ItemList l;
    l.lst.append(42);
    CHECK(l.lst.key() == "lst");
}
