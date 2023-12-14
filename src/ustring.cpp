#include "pack/ustring.h"
#include <algorithm>
#include <iostream>
#ifndef WITH_QT
#include <utf8.h>
#include <regex>
#else
#include <QRegularExpression>
#endif

namespace pack {

UString::UString(const char* val)
#ifdef WITH_QT
    : m_string(QString::fromStdString(val))
#else
    : m_string(val)
#endif
{
}

UString::UString(const std::string& val)
#ifdef WITH_QT
    : m_string(QString::fromStdString(val))
#else
    : m_string(val)
#endif
{
}

#ifdef WITH_QT
UString::UString(const QString& val)
    : m_string(val)
{
}
#endif


UString::UString(const char* str, std::size_t size)
#ifdef WITH_QT
    : m_string(QString::fromUtf8(str, size))
#else
    : m_string(std::string(str, size))
#endif
{
}

bool UString::isEmpty() const
{
#ifdef WITH_QT
    return m_string.isEmpty();
#else
    return m_string.empty();
#endif
}

std::string UString::toStdString() const
{
#ifdef WITH_QT
    return m_string.toStdString();
#else
    return m_string;
#endif
}

#ifdef WITH_QT
[[nodiscard]] QString UString::toQString() const
{
    return m_string;
}
#endif

void UString::clear()
{
    m_string.clear();
}

bool UString::startsWith(const UString& s, CaseSensitivity cs) const
{
#ifdef WITH_QT
    return m_string.startsWith(s.m_string, cs == CaseSensitivity::Insensitive ? Qt::CaseInsensitive : Qt::CaseSensitive);
#else
    if (cs == CaseSensitivity::Insensitive) {
        auto lower = s.toLower();
        return toLower().m_string.find(lower.m_string) == 0;
    }
    return m_string.find(s.m_string) == 0;
#endif
}

UString UString::toLower() const
{
#ifdef WITH_QT
    return m_string.toLower();
#else
    auto s32 = utf8::utf8to32(m_string);

    std::transform(s32.begin(), s32.end(), s32.begin(), [](wchar_t ch) {
        return std::tolower(ch, std::locale("en_US.utf8"));
    });

    return utf8::utf32to8(s32);
#endif
}

UString UString::toUpper() const
{
#ifdef WITH_QT
    return m_string.toUpper();
#else
    auto s32 = utf8::utf8to32(m_string);

    std::transform(s32.begin(), s32.end(), s32.begin(), [](wchar_t ch) {
        return std::toupper(ch, std::locale("en_US.utf8"));
    });

    return utf8::utf32to8(s32);
#endif
}

UString UString::trimmed() const
{
#ifdef WITH_QT
    return m_string.trimmed();
#else
    static std::string toTrim = " \t\n\r";

    std::string out = m_string;
    out.erase(out.find_last_not_of(toTrim) + 1);
    out.erase(0, out.find_first_not_of(toTrim));
    return out;
#endif
}

UString& UString::append(char ch)
{
#ifdef WITH_QT
    m_string.append(QLatin1Char(ch));
#else
    m_string.append(1, ch);
#endif
    return *this;
}

UString& UString::append(const char* str)
{
#ifdef WITH_QT
    m_string.append(QLatin1String(str));
#else
    m_string.append(str);
#endif
    return *this;
}

UString& UString::append(const UString& str)
{
    m_string.append(str.m_string);
    return *this;
}

UString UString::operator+=(char ch)
{
    append(ch);
    return *this;
}

UString UString::operator+=(const char* str)
{
    append(str);
    return *this;
}

UString UString::operator+=(const UString& str)
{
    append(str);
    return *this;
}

UString& UString::prepend(char ch)
{
#ifdef WITH_QT
    m_string.prepend(QLatin1Char(ch));
#else
    m_string.insert(0, 1, ch);
#endif
    return *this;
}

UString& UString::prepend(const char* str)
{
#ifdef WITH_QT
    m_string.prepend(QLatin1String(str));
#else
    m_string.insert(0, str);
#endif
    return *this;
}

UString& UString::prepend(const UString& str)
{
    m_string.insert(0, str.m_string);
    return *this;
}

#ifdef WITH_QT
UString& UString::append(const QString& str)
{
    m_string.append(str);
    return *this;
}

UString& UString::prepend(const QString& str)
{
    m_string.prepend(str);
    return *this;
}
#endif

int UString::size() const
{
    return int(m_string.size());
}

std::strong_ordering UString::compare(const UString& other) const
{
    if (m_string < other.m_string)
        return std::strong_ordering::less;
    if (m_string > other.m_string)
        return std::strong_ordering::greater;
    return std::strong_ordering::equal;
}

std::vector<UString> UString::split(const UString& sep, SplitOption options) const
{
#ifdef WITH_QT
    Qt::SplitBehavior beh;
    if (isSet(options, SplitOption::SkipEmpty)) {
        beh |= Qt::SkipEmptyParts;
    }
    std::vector<UString> ret;
    auto res = m_string.split(QRegularExpression(sep.toQString()), beh);
    for(const auto& it: res) {
        if (it.isEmpty() && isSet(options, SplitOption::SkipEmpty)) {
            continue;
        }
        ret.push_back(std::move(it));
    }
    return ret;
#else
    std::regex rex(sep.toStdString());
    try {
        std::vector<UString> ret;
        std::sregex_token_iterator iter(m_string.begin(), m_string.end(), rex, -1);
        std::sregex_token_iterator end;
        for (; iter != end; ++iter) {
            UString it(*iter);
            if (isSet(options, SplitOption::Trim)) {
                it = it.trimmed();
            }
            if (it.isEmpty() && isSet(options, SplitOption::SkipEmpty)) {
                continue;
            }
            ret.push_back(std::move(it));
        }
        return ret;
    } catch (const std::regex_error&) {
        return {*this};
    }
#endif
}

} // namespace pack
