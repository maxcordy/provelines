/*
 * OrExp.cpp
 *
 * Created on 2013-08-09
 */

#include "OrExp.hpp"

#include "Visitor.hpp"

using std::tr1::shared_ptr;

namespace util {
namespace parser {

// constructors
OrExp::OrExp(const shared_ptr<BoolExp> & lhs, const shared_ptr<BoolExp> &
        rhs) : BinExp(lhs, rhs) {}

// public methods
void OrExp::accept(Visitor & visitor) const {
    getLeftExp()->accept(visitor);
    getRightExp()->accept(visitor);
    visitor.orExp(*this);
}

} // namespace parser
} // namespace util
