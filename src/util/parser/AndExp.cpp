/*
 * AndExp.cpp
 *
 * Created on 2013-08-09
 */

#include "AndExp.hpp"
#include "Visitor.hpp"

using std::tr1::shared_ptr;

namespace util {
namespace parser {

// constructors
AndExp::AndExp(const shared_ptr<BoolExp> & lhs, const shared_ptr<BoolExp> &
        rhs) : BinExp(lhs, rhs) {}

// public methods
void AndExp::accept(Visitor & visitor) const {
    getLeftExp()->accept(visitor);
    getRightExp()->accept(visitor);
    visitor.andExp(*this);
}

} // namespace parser
} // namespace util
