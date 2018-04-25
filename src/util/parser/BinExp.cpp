/*
 * BinExp.cpp
 *
 * Created on 2013-08-09
 */

#include "BinExp.hpp"

#include "util/Assert.hpp"

using std::tr1::shared_ptr;

namespace util {
namespace parser {

// constructors
BinExp::BinExp(const shared_ptr<BoolExp> & lhs, const shared_ptr<BoolExp> &
        rhs) : lhs(lhs), rhs(rhs) {
    checkRep();
}

// public methods
shared_ptr<BoolExp> BinExp::getLeftExp() const {
    return lhs;
}

shared_ptr<BoolExp> BinExp::getRightExp() const {
    return rhs;
}

// private methods
void BinExp::checkRep() const {
    if (doCheckRep) {
        assertTrue(lhs);
        assertTrue(rhs);
    }
}

} // namespace parser
} // namespace util
