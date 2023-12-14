#pragma once
#include <pack/attribute.h>
#include <pack/meta.h>

namespace pack {

#define FIELD(key, ...)                                                                                                                    \
    {                                                                                                                                      \
        pack::Key(key##_s), ##__VA_ARGS__                                                                                                  \
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
    inline static const std::vector<pack::UString>& staticFieldNames()                                                                     \
    {                                                                                                                                      \
        static std::vector<pack::UString> names = []() {                                                                                   \
            auto thisFields = #__VA_ARGS__##_s.split(",?\\s+");                                                                            \
            auto baseFields = Super::staticFieldNames();                                                                                   \
            baseFields.insert(baseFields.end(), thisFields.begin(), thisFields.end());                                                     \
            return baseFields;                                                                                                             \
        }();                                                                                                                               \
        return names;                                                                                                                      \
    }

#define META(Name, ...)                                                                                                                    \
    using Super = Node;                                                                                                                    \
    using Super::Super;                                                                                                                    \
    pack::Meta m_meta = pack::Meta(#Name, __VA_ARGS__);                                                                                    \
    META_METHODS(Name, __VA_ARGS__)

#define META_BASE(Name, Parent, ...)                                                                                                       \
    using Super = Parent;                                                                                                                  \
    using Super::Super;                                                                                                                    \
    pack::Meta m_meta = pack::Meta(#Name, Parent::meta(), __VA_ARGS__);                                                                    \
    META_METHODS(Name, __VA_ARGS__)


class Node : public Attribute
{
public:
    Node();
    Node(Node&& other)      = default;
    Node(const Node& other) = default;

    int     compare(const Attribute& other) const override;
    UString typeName() const override;
    void    set(const Attribute& other) override;
    void    set(Attribute&& other) override;
    bool    empty() const override;
    void    clear() override;

    bool operator==(const Node& other) const;

    static const std::vector<pack::UString>& staticFieldNames();

    virtual const Meta& meta() const = 0;

protected:
    void copyFields(const Node& other);
    void moveFields(Node&& other);
};

} // namespace pack
