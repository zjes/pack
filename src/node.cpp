#include "pack/types/node.h"


namespace pack {

Node::Node()
    : Attribute(NodeType::Node)
{
}

void Node::copyFields(const Node& other)
{
    for (auto& it : meta().fields()) {
        it->set(*other.meta().field(it->key()));
    }
}

void Node::moveFields(Node&& other)
{
    for (auto& it : meta().fields()) {
        it->set(std::move(*other.meta().field(it->key())));
    }
}

int Node::compare(const Attribute& other) const
{
    if (const Node* node = dynamic_cast<const Node*>(&other)) {
        for (const auto& field : meta().fields()) {
            if (const auto* ofield = node->meta().field(field->key())) {
                if (int res = field->compare(*ofield); res != 0) {
                    return res;
                }
            } else {
                return -1;
            }
        }
    }
    return 0;
}

UString Node::typeName() const
{
    return meta().m_name;
}

void Node::set(const Attribute& other)
{
    if (const auto* node = dynamic_cast<const Node*>(&other)) {
        copyFields(*node);
    }
}

void Node::set(Attribute&& other)
{
    if (auto* node = dynamic_cast<Node*>(&other)) {
        moveFields(std::move(*node));
    }
}

bool Node::empty() const
{
    for (const auto& field : meta().fields()) {
        if (!field->empty()) {
            return false;
        }
    }

    return true;
}

void Node::clear()
{
    for (const auto& field : meta().fields()) {
        field->clear();
    }
}

bool Node::operator==(const Node& other) const
{
    return compare(other) == 0;
}

const std::vector<pack::UString>& Node::staticFieldNames()
{
    static std::vector<pack::UString> items;
    return items;
}

}
