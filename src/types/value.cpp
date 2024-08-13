#include "pack/types/value.h"

namespace pack {

IValue::~IValue() = default;
IValue::IValue(const IValue& other)     = default;
IValue::IValue(IValue&& other) noexcept = default;

}
