/*
 * NotExp.cpp
 *
 * Created on 2013-08-09
 */

#include "NotExp.hpp"

#include "Visitor.hpp"
#include "util/Assert.hpp"

using std::tr1::shared_ptr;

namespace util {
namespace parser {

// constructors
NotExp::NotExp(const shared_ptr<BoolExp> & exp) : exp(exp) {
    checkRep();
}

// public methods
shared_ptr<BoolExp> NotExp::getBoolExp() const {
    return exp;
}

void NotExp::accept(Visitor & visitor) const {
    exp->accept(visitor);
    visitor.notExp(*this);
}

// private methods
void NotExp::checkRep() const {
    if (doCheckRep) {
        assertTrue(exp);
    }
}

} // namespace parser
} // namespace util
