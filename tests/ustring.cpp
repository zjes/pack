#include <catch2/catch_all.hpp>
#include <pack/types.h>

TEST_CASE("string")
{
    {
        pack::UString str;
        CHECK(str.isEmpty());
        CHECK(str == ""_s);
    }

    {
        pack::UString str("val");
        CHECK(!str.isEmpty());
        CHECK(str == "val"_s);
    }

    {
        pack::UString str("val", 3);
        CHECK(!str.isEmpty());
        CHECK(str == "val"_s);
    }

    {
        pack::UString str(std::string("val"));
        CHECK(!str.isEmpty());
        CHECK(str == "val"_s);
    }

#ifdef WITH_QT
    {
        pack::UString str(QLatin1String("val"));
        CHECK(!str.isEmpty());
        CHECK(str == "val"_s);
    }
#endif

    {
        pack::UString str = "value"_s;
        CHECK(str == "value"_s);
        CHECK(!str.isEmpty());
        str.clear();
        CHECK(str.isEmpty());
    }

    {
        pack::UString str = "Value"_s;
        CHECK(str.startsWith("val"_s, pack::CaseSensitivity::Insensitive));
        CHECK(str.startsWith("Val"_s, pack::CaseSensitivity::Sensitive));
        CHECK(str.startsWith("Val"_s));

        CHECK(str.startsWith("val", pack::CaseSensitivity::Insensitive));
        CHECK(str.startsWith("Val", pack::CaseSensitivity::Sensitive));
        CHECK(str.startsWith("Val"));
    }

    {
        CHECK("UPPER"_s.toLower() == "upper"_s);
        CHECK("UPPER Č"_s.toLower() == "upper č"_s);
        CHECK("UPPER Ру"_s.toLower() == "upper ру"_s);
    }
    {
        CHECK("upper"_s.toUpper() == "UPPER"_s);
        CHECK("upper č"_s.toUpper() == "UPPER Č"_s);
        CHECK("upper ру"_s.toUpper() == "UPPER РУ"_s);
    }

    {
        CHECK(" upper č "_s.trimmed() == "upper č"_s);
        CHECK("\tupper č\n"_s.trimmed() == "upper č"_s);
    }

    {
        CHECK("val"_s.append('c') == "valc"_s);
        CHECK("val"_s.append("str") == "valstr"_s);
        CHECK("val"_s.append("str"_s) == "valstr"_s);
        CHECK("val"_s.append(std::string("str")) == "valstr"_s);

        CHECK("val"_s.prepend('c') == "cval"_s);
        CHECK("val"_s.prepend("str") == "strval"_s);
        CHECK("val"_s.prepend("str"_s) == "strval"_s);
        CHECK("val"_s.prepend(std::string("str")) == "strval"_s);

#ifdef WITH_QT
        CHECK("val"_s.append(QLatin1String("str")) == "valstr"_s);
        CHECK("val"_s.append(QLatin1String("str")) == "valstr"_s);
#endif
    }

    {
        CHECK("value"_s.contains("alu"));
        CHECK("value"_s.contains("ALU", pack::CaseSensitivity::Insensitive));
        CHECK("VALUE"_s.contains("alu", pack::CaseSensitivity::Insensitive));
    }

    {
        pack::UString str = "red parrot with red eyes";
        str.replace("red", "yellow");
        CHECK(str == "yellow parrot with yellow eyes");
    }

    {
        pack::UString str = "red parrot with red eyes";
        str.replace("RED", "yellow", pack::CaseSensitivity::Insensitive);
        CHECK(str == "yellow parrot with yellow eyes");
    }

    {
        pack::UString str = "red parrot with red eyes";
        str.replace(std::regex("\\bred\\b"), "yellow");
        CHECK(str == "yellow parrot with yellow eyes");
    }
}

TEST_CASE("format")
{
    CHECK(pack::format("some str {}"_s, 10) == "some str 10"_s);
}
