#pragma once

#include <pack/convert.h>
#include <pack/ustring.h>
#ifdef WITH_QT
#include <QDebug>
#endif

namespace pack {

template <typename... Args>
inline UString format(const UString& fmt, const Args&... args)
{
    return fmt::format(fmt::runtime(fmt.toStdString()), args...);
}

inline std::ostream& operator<<(std::ostream& ss, const pack::UString& value)
{
#ifdef WITH_QT
    ss << qPrintable(value.m_string);
#else
    ss << value.m_string;
#endif
    return ss;
}

#ifdef WITH_QT
inline QDebug operator<<(QDebug debug, const UString &value)
{
#ifdef WITH_QT
    debug << qPrintable(value.m_string);
#else
    ss << value.m_string;
#endif
    return debug;
}
#endif

template <typename T>
requires std::integral<T> || std::floating_point<T>
UString UString::fromNumber(T value)
{
    return format("{}", value);
}

template <typename T>
requires std::integral<T> || std::floating_point<T>
int UString::toNumber() const
{
    return convert<T>(m_string);
}

template <typename T>
requires std::convertible_to<T, UString::Type>
bool UString::operator==(const T& other) const
{
    return compare(other) == std::strong_ordering::equal;
}

inline bool UString::operator==(const UString& other) const
{
    return compare(other) == std::strong_ordering::equal;
}

template <typename T>
requires std::convertible_to<T, UString::Type>
std::strong_ordering UString::operator<=>(const T& other) const
{
    return compare(other);
}

inline std::strong_ordering UString::operator<=>(const UString& other) const
{
    return compare(other);
}

} // namespace pack

inline pack::UString operator"" _s(const char* str, std::size_t size)
{
    return pack::UString(str, size);
}

template <typename T>
requires std::convertible_to<T, std::string>
pack::UString operator+(const pack::UString& l, const T& r)
{
    return l + r;
}

template <typename T>
requires std::same_as<T, pack::UString>
pack::UString operator+(const pack::UString& l, const T& r)
{
#ifdef WITH_QT
    qInfo() << (l.toQString() + r.toQString()) << l.toQString() << r.toQString();
    return pack::UString(l.toQString() + r.toQString());
#else
    return pack::UString(l.toStdString() + r.toStdString());
#endif
}

namespace fmt {

/// Helper to format QString enity into fmt
template <typename ParseContext>
constexpr auto formatter<pack::UString>::parse(ParseContext& ctx)
{
    return ctx.begin();
}

template <typename FormatContext>
inline auto formatter<pack::UString>::format(const pack::UString& attr, FormatContext& ctx)
{
    return fmt::format_to(ctx.out(), "{}", attr.toStdString());
}

} // namespace fmt
