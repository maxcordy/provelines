/*
 * Var.cpp
 *
 * Created on 2013-08-09
 */

#include "Var.hpp"

#include "Visitor.hpp"

using std::string;

namespace util {
namespace parser {

// constructors
Var::Var(const string & name) : name(name) {}

// public methods
string Var::getName() const {
    return name;
}

void Var::accept(Visitor & visitor) const {
    visitor.var(*this);
}

} // namespace parser
} // namespace util
