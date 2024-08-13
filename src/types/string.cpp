#include "pack/types/string.h"

namespace pack {

String::~String() = default;

String::String(const String& other) = default;

int String::size() const
{
    return int(value().size());
}

String::operator UString() const
{
    return value();
}

String String::operator+=(const String& other)
{
    setValue(value() + other.value());
    return *this;
}

String String::operator+=(typename String::ConstRefType other)
{
    setValue(value() + other);
    return *this;
}

String& String::operator=(const String& other)
{
    setValue(other.m_val);
    return *this;
}

String& String::operator=(String&& other)
{
    setValue(other.m_val);
    return *this;
}


#ifdef WITH_QT
QDebug operator<<(QDebug debug, const String& value)
{
    debug << qPrintable(value.value().toQString());
    return debug;
}
#endif

std::ostream& operator<<(std::ostream& ss, const String& s)
{
    ss << s.value().toStdString();
    return ss;
}

} // namespace pack
