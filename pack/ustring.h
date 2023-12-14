#pragma once

#ifdef WITH_QT
#include <QString>
#else
#include <string>
#endif
#include <fmt/format.h>
#include <ostream>
#include <pack/flags.h>
#include <vector>

namespace pack {

enum class CaseSensitivity
{
    Insensitive,
    Sensitive
};

enum class SplitOption
{
    SkipEmpty = 1 << 0,
    KeepEmpty = 1 << 1,
    Trim      = 1 << 2,
    NoTrim    = 1 << 3,
};

ENABLE_FLAGS(SplitOption);

class UString
{

public:
#ifdef WITH_QT
    using Type = QString;
#else
    using Type = std::string;
#endif

public:
    UString() = default;
    UString(const char* val);
    UString(const std::string& val);
    UString(const char* str, std::size_t size);

#ifdef WITH_QT
    UString(const QString& val);
#endif

public:
    void               clear();
    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] bool startsWith(const UString& s, CaseSensitivity cs = CaseSensitivity::Sensitive) const;
    [[nodiscard]] bool endsWith(const UString& s, CaseSensitivity cs = CaseSensitivity::Sensitive) const;

public:
    [[nodiscard]] UString toLower() const;
    [[nodiscard]] UString toUpper() const;
    [[nodiscard]] UString trimmed() const;

public:
    template <typename T>
    requires std::integral<T> || std::floating_point<T>
    static UString fromNumber(T value);

    template <typename T>
    requires std::integral<T> || std::floating_point<T>
    int toNumber() const;

public:
    UString& append(char ch);
    UString& append(const char* str);
    UString& append(const UString& str);
    UString& prepend(char c);
    UString& prepend(const char* str);
    UString& prepend(const UString& str);
#ifdef WITH_QT
    UString& append(const QString& str);
    UString& prepend(const QString& str);
#endif
    UString operator+=(char ch);
    UString operator+=(const char* str);
    UString operator+=(const UString& str);

public:
    int size() const;

public:
    [[nodiscard]] std::string toStdString() const;
#ifdef WITH_QT
    [[nodiscard]] QString toQString() const;
#endif

public:
    template <typename T>
    requires std::convertible_to<T, Type> bool
    operator==(const T& other) const;

    bool operator==(const UString& other) const;

    template <typename T>
    requires std::convertible_to<T, Type>
    std::strong_ordering operator<=>(const T& other) const;

    std::strong_ordering operator<=>(const UString& other) const;

    std::strong_ordering compare(const UString& other) const;

    [[nodiscard]] std::vector<UString> split(const UString& sep, SplitOption options = SplitOption::KeepEmpty | SplitOption::Trim) const;

    friend std::ostream& operator<<(std::ostream& ss, const pack::UString& value);
#ifdef WITH_QT
    friend QDebug operator<<(QDebug debug, const UString& value);
#endif

private:
    Type m_string;
}; // namespace pack

template <typename... Args>
UString format(const UString& fmt, const Args&... args);

std::ostream& operator<<(std::ostream& ss, const UString& value);
#ifdef WITH_QT
QDebug operator<<(QDebug debug, const UString& value);
#endif

} // namespace pack

// =========================================================================================================================================

/// String literal
pack::UString operator"" _s(const char* str, std::size_t size);

template <typename T>
requires std::convertible_to<T, std::string>
pack::UString operator+(const pack::UString& l, const T& r);

template <typename T>
requires std::same_as<T, pack::UString>
pack::UString operator+(const pack::UString& l, const T& r);

// =========================================================================================================================================

namespace fmt {

/// Helper to format UString enity into fmt
template <>
struct formatter<pack::UString>
{
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx);

    template <typename FormatContext>
    auto format(const pack::UString& attr, FormatContext& ctx);
};

} // namespace fmt

// =========================================================================================================================================

#include <pack/private/ustring.inl>
