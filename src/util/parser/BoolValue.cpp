/*
 * BoolValue.cpp
 *
 * Created on 2013-08-09
 */
#include "BoolValue.hpp"
#include "Visitor.hpp"

namespace util {
namespace parser {

// constructors
BoolValue::BoolValue(bool value) : value(value) {}

// public methods
bool BoolValue::getValue() const {
    return value;
}

void BoolValue::accept(Visitor & visitor) const {
    visitor.boolValue(*this);
}

} // namespace parser
} // namespace util
