#pragma once
#include <iostream>
#include <pack/attribute.h>
#include <pack/meta.h>

namespace pack {

#define FIELD(key, ...)                                                                                                                    \
    {                                                                                                                                      \
        pack::Key(key##_s), __VA_ARGS__                                                                                                    \
    }

#define META_METHODS(Name, ...)                                                                                                            \
    const pack::Meta& meta() const override                                                                                                \
    {                                                                                                                                      \
        return m_meta;                                                                                                                     \
    }                                                                                                                                      \
    Name(Name&& other)                                                                                                                     \
        : Super(other)                                                                                                                     \
    {                                                                                                                                      \
        moveFields(std::move(other));                                                                                                      \
    }                                                                                                                                      \
    Name(const Name& other)                                                                                                                \
        : Super(other)                                                                                                                     \
    {                                                                                                                                      \
        copyFields(other);                                                                                                                 \
    }                                                                                                                                      \
    Name& operator=(const Name& other)                                                                                                     \
    {                                                                                                                                      \
        copyFields(other);                                                                                                                 \
        return *this;                                                                                                                      \
    }                                                                                                                                      \
    Name& operator=(Name&& other)                                                                                                          \
    {                                                                                                                                      \
        moveFields(std::move(other));                                                                                                      \
        return *this;                                                                                                                      \
    }                                                                                                                                      \
    inline static const std::vector<pack::UString>& staticFieldNames()                                                                     \
    {                                                                                                                                      \
        static std::vector<pack::UString> names = []() {                                                                                   \
            auto thisFields = #__VA_ARGS__##_s.split(",?\\s+");                                                                            \
            auto baseFields = Super::staticFieldNames();                                                                                   \
            baseFields.insert(baseFields.end(), thisFields.begin(), thisFields.end());                                                     \
            return baseFields;                                                                                                             \
        }();                                                                                                                               \
        return names;                                                                                                                      \
    }                                                                                                                                      \
    static pack::UString typeInfo()                                                                                                        \
    {                                                                                                                                      \
        return #Name;                                                                                                                      \
    }

#define META(Name, ...)                                                                                                                    \
public:                                                                                                                                    \
    using Super = Node;                                                                                                                    \
    using Super::Super;                                                                                                                    \
    META_METHODS(Name, __VA_ARGS__)                                                                                                        \
private:                                                                                                                                   \
    pack::Meta m_meta = pack::Meta(#Name, __VA_ARGS__);                                                                                    \
                                                                                                                                           \
public:                                                                                                                                    \
    using CppType = Name

#define META_BASE(Name, Parent, ...)                                                                                                       \
public:                                                                                                                                    \
    using Super = Parent;                                                                                                                  \
    using Super::Super;                                                                                                                    \
    META_METHODS(Name, __VA_ARGS__)                                                                                                        \
private:                                                                                                                                   \
    pack::Meta m_meta = pack::Meta(#Name, Parent::meta(), __VA_ARGS__);                                                                    \
                                                                                                                                           \
public:                                                                                                                                    \
    using CppType = Name


class Node : public Attribute
{
public:
    Node();
    Node(Node&& other)      = default;
    Node(const Node& other) = default;

    template <typename... Options>
    Node(Options&&... args)
    requires allIsOptions<Options...>
        : Attribute(NodeType::Node, std::forward<Options>(args)...)
    {
    }

    [[nodiscard]] int     compare(const Attribute& other) const override;
    [[nodiscard]] UString typeName() const override;
    void                  set(const Attribute& other) override;
    void                  set(Attribute&& other) override;
    [[nodiscard]] bool    empty() const override;
    void                  clear() override;

    bool operator==(const Node& other) const;

    static const std::vector<pack::UString>& staticFieldNames();

    [[nodiscard]] virtual const Meta& meta() const = 0;

    [[nodiscard]] virtual const std::string& fileDescriptor() const;
    [[nodiscard]] virtual std::string        protoName() const;

protected:
    void copyFields(const Node& other);
    void moveFields(Node&& other);
};

} // namespace pack
