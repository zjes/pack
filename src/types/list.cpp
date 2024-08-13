#include "pack/types/list.h"

namespace pack {

IList::IList()
    : Attribute(NodeType::List)
{
}

IList::~IList() = default;

} // namespace pack
