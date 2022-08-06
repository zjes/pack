/* =========================================================================================================================================
    ____ __ _ ____ __  __
   |    |  ` |    |  /  /
   | |  | |  | |__|    /
   | ___| |  | |  |    \
   |_|  |__,_|____|__\__\ DSO library

   Copyright (C) 2020 Eaton
   Copyright (C) 2020-2022 zJes

   This program is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
========================================================================================================================================= */
#pragma once

#include "pack/attribute.h"
#include "pack/expected.h"

namespace pack {

// =========================================================================================================================================

// Add metainformation to the value
#define FIELD(key, ...)                                                                                                                    \
    {                                                                                                                                      \
        pack::Key(key##_s), ##__VA_ARGS__                                                                                                  \
    }

#define META(className, ...)                                                                                                               \
    META_CTR(className, __VA_ARGS__)                                                                                                       \
    META_FIELDS(className)                                                                                                                 \
    META_FIELDS_IMPL(className, __VA_ARGS__)                                                                                               \
    META_AUX(className)                                                                                                                    \
    META_INFO(className)                                                                                                                   \
    META_STATIC_INFO(className)

#define META_BASE(className, base, ...)                                                                                                    \
    META_CTR_BASE(className, base, __VA_ARGS__)                                                                                            \
    META_FIELDS(className)                                                                                                                 \
    META_FIELDS_BASE_IMPL(className, base, __VA_ARGS__)                                                                                    \
    META_AUX(className)                                                                                                                    \
    META_INFO(className)                                                                                                                   \
    META_STATIC_INFO(className)

// =========================================================================================================================================

#define META_CTR(className, ...)                                                                                                           \
public:                                                                                                                                    \
    className(const className& other)                                                                                                      \
    {                                                                                                                                      \
        copyFields(other);                                                                                                                 \
    }                                                                                                                                      \
    className(className&& other)                                                                                                           \
    {                                                                                                                                      \
        moveFields(other);                                                                                                                 \
    }                                                                                                                                      \
    inline className& operator=(const className& other)                                                                                    \
    {                                                                                                                                      \
        copyFields(other);                                                                                                                 \
        return *this;                                                                                                                      \
    }                                                                                                                                      \
    inline className& operator=(className&& other)                                                                                         \
    {                                                                                                                                      \
        moveFields(other);                                                                                                                 \
        return *this;                                                                                                                      \
    }

#define META_CTR_BASE(className, base, ...)                                                                                                \
public:                                                                                                                                    \
    className(const className& other)                                                                                                      \
        : base(other)                                                                                                                      \
    {                                                                                                                                      \
        copyFields(other);                                                                                                                 \
    }                                                                                                                                      \
    className(className&& other)                                                                                                           \
        : base(other)                                                                                                                      \
    {                                                                                                                                      \
        moveFields(other);                                                                                                                 \
    }                                                                                                                                      \
    inline className& operator=(const className& other)                                                                                    \
    {                                                                                                                                      \
        base::operator=(other);                                                                                                            \
        copyFields(other);                                                                                                                 \
        return *this;                                                                                                                      \
    }                                                                                                                                      \
    inline className& operator=(className&& other)                                                                                         \
    {                                                                                                                                      \
        base::operator=(other);                                                                                                            \
        moveFields(other);                                                                                                                 \
        return *this;                                                                                                                      \
    }

#define META_AUX(className)                                                                                                                \
public:                                                                                                                                    \
    bool operator==(const className& other) const                                                                                          \
    {                                                                                                                                      \
        return compare(other);                                                                                                             \
    }                                                                                                                                      \
                                                                                                                                           \
private:                                                                                                                                   \
    void copyFields(const className& other)                                                                                                \
    {                                                                                                                                      \
        for (auto& it : _fields()) {                                                                                                       \
            it.get().set(*other.fieldByKey(it.get().key()));                                                                               \
        }                                                                                                                                  \
    }                                                                                                                                      \
    void moveFields(const className& other)                                                                                                \
    {                                                                                                                                      \
        for (auto& it : _fields()) {                                                                                                       \
            it.get().set(std::move(*other.fieldByKey(it.get().key())));                                                                    \
        }                                                                                                                                  \
    }

#define META_FIELDS(className)                                                                                                             \
public:                                                                                                                                    \
    inline std::vector<std::reference_wrapper<const pack::Attribute>> fields() const override                                              \
    {                                                                                                                                      \
        return _fields();                                                                                                                  \
    }                                                                                                                                      \
    inline std::vector<std::reference_wrapper<pack::Attribute>> fields() override                                                          \
    {                                                                                                                                      \
        return _fields();                                                                                                                  \
    }                                                                                                                                      \
    inline std::vector<pack::string_t> fieldNames() const override                                                                         \
    {                                                                                                                                      \
        return staticFieldNames();                                                                                                         \
    }

#define META_FIELDS_IMPL(className, ...)                                                                                                   \
public:                                                                                                                                    \
    inline static std::vector<pack::string_t> staticFieldNames()                                                                           \
    {                                                                                                                                      \
        static std::vector<pack::string_t> ret = pack::split(#__VA_ARGS__##_s);                                                            \
        return ret;                                                                                                                        \
    }                                                                                                                                      \
                                                                                                                                           \
protected:                                                                                                                                 \
    inline std::vector<std::reference_wrapper<const pack::Attribute>> _fields() const                                                      \
    {                                                                                                                                      \
        return std::apply(                                                                                                                 \
            [](const auto&... elems) {                                                                                                     \
                return std::vector<std::reference_wrapper<const pack::Attribute>>{std::cref(elems)...};                                    \
            },                                                                                                                             \
            std::forward_as_tuple(__VA_ARGS__));                                                                                           \
    }                                                                                                                                      \
    inline std::vector<std::reference_wrapper<pack::Attribute>> _fields()                                                                  \
    {                                                                                                                                      \
        return std::apply(                                                                                                                 \
            [](auto&... elems) {                                                                                                           \
                return std::vector<std::reference_wrapper<pack::Attribute>>{std::ref(elems)...};                                           \
            },                                                                                                                             \
            std::forward_as_tuple(__VA_ARGS__));                                                                                           \
    }

#define META_FIELDS_BASE_IMPL(className, base, ...)                                                                                        \
public:                                                                                                                                    \
    inline static std::vector<pack::string_t> staticFieldNames()                                                                           \
    {                                                                                                                                      \
        auto baseFields = base::staticFieldNames();                                                                                        \
        auto thisFields = pack::split(#__VA_ARGS__##_s);                                                                                   \
        baseFields.insert(baseFields.end(), thisFields.begin(), thisFields.end());                                                         \
        return baseFields;                                                                                                                 \
    }                                                                                                                                      \
                                                                                                                                           \
protected:                                                                                                                                 \
    inline std::vector<std::reference_wrapper<const pack::Attribute>> _fields() const                                                      \
    {                                                                                                                                      \
        auto baseFields = base::fields();                                                                                                  \
        auto thisFields = std::apply(                                                                                                      \
            [](const auto&... elems) {                                                                                                     \
                return std::vector<std::reference_wrapper<const pack::Attribute>>{elems...};                                               \
            },                                                                                                                             \
            std::forward_as_tuple(__VA_ARGS__));                                                                                           \
        baseFields.insert(baseFields.end(), thisFields.begin(), thisFields.end());                                                         \
        return baseFields;                                                                                                                 \
    }                                                                                                                                      \
    inline std::vector<std::reference_wrapper<pack::Attribute>> _fields()                                                                  \
    {                                                                                                                                      \
        auto baseFields = base::fields();                                                                                                  \
        auto thisFields = std::apply(                                                                                                      \
            [](auto&... elems) {                                                                                                           \
                return std::vector<std::reference_wrapper<pack::Attribute>>{elems...};                                                     \
            },                                                                                                                             \
            std::forward_as_tuple(__VA_ARGS__));                                                                                           \
        baseFields.insert(baseFields.end(), thisFields.begin(), thisFields.end());                                                         \
        return baseFields;                                                                                                                 \
    }

#define META_INFO(className)                                                                                                               \
public:                                                                                                                                    \
    inline pack::string_t typeName() const override                                                                                        \
    {                                                                                                                                      \
        return typeInfo();                                                                                                                 \
    }                                                                                                                                      \
    using CppType = className;

#ifdef WITH_QTSTRING
#define META_STATIC_INFO(className)                                                                                                        \
    static pack::string_t typeInfo()                                                                                                       \
    {                                                                                                                                      \
        return QLatin1String(m_className);                                                                                                 \
    }                                                                                                                                      \
    static constexpr const char* m_className = #className
#else
#define META_STATIC_INFO(className)                                                                                                        \
    static pack::string_t typeInfo()                                                                                                       \
    {                                                                                                                                      \
        return m_className;                                                                                                                \
    }                                                                                                                                      \
    static constexpr const char* m_className = #className
#endif

// =========================================================================================================================================

std::vector<pack::string_t> split(const pack::string_t& str);

// =========================================================================================================================================

/// Node interface
class INode : public Attribute
{
public:
    INode();

    template <typename... Args, typename = std::enable_if_t<std::is_base_of_v<FieldOption, Args...>>>
    INode(const Args&... args)
        : Attribute(NodeType::Node, args...)
    {
    }

    /// Dumps a class as yaml serialized string
    virtual string_t dump() const = 0;

    /// Returns a list of fields
    virtual std::vector<std::reference_wrapper<pack::Attribute>> fields() = 0;

    /// Returns a list of fields
    virtual std::vector<std::reference_wrapper<const pack::Attribute>> fields() const = 0;

    /// Returns a list of the fields names
    virtual std::vector<string_t> fieldNames() const = 0;

    virtual const std::string& fileDescriptor() const = 0;

    virtual std::string protoName() const = 0;
};

// =========================================================================================================================================

/// Node class
///
/// This class used as main holder of the structured data.
/// Typical usage is:
/// ---------------------------
/// struct MyClass: public pack::Node {
///     using pack::Node::Node;
///     pack::String str = FIELD("string-value", 1)
///     META(MyClass, str)
/// }
/// ---------------------------
/// Where each field has a usage name (str) and real name used to serialization/deserialization (string-value)
/// META macro just define metainformation about a struct, such as struct name and list of the fields.
class Node : public INode
{
public:
    using INode::INode;

    /// Compares classes by content
    bool compare(const Attribute& other) const override;

    /// Dumps a class as yaml serialized string
    string_t dump() const override;

    /// Returns field it's key
    Expected<std::reference_wrapper<const Attribute>> fieldByKey(const string_t& name) const;

    /// Returns field it's name
    Expected<std::reference_wrapper<const pack::Attribute>> fieldByName(const string_t& name) const;

    void set(const Attribute& other) override;
    void set(Attribute&& other) override;

    /// Checks if this class has any nondefault field
    bool hasValue() const override;

    const std::string& fileDescriptor() const override;

    std::string protoName() const override;

    void clear() override;
};

// =========================================================================================================================================

} // namespace pack
