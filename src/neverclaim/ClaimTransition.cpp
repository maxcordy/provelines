/*
 * ClaimTransition.cpp
 *
 * Created on 2013-08-02
 */

#include "ClaimTransition.hpp"

#include "ClaimState.hpp"
#include "util/Assert.hpp"
#include "util/parser/BoolExp.hpp"

using util::parser::BoolExp;
using std::string;
using std::tr1::shared_ptr;

namespace neverclaim {

ClaimTransition::ClaimTransition(const string & source, const string & target,
        const shared_ptr<BoolExp> & boolExp)
        : source(ClaimState::makeState(source)),
          target(ClaimState::makeState(target)), boolExp(boolExp) {
    checkRep();
}

shared_ptr<ClaimState> ClaimTransition::getSource() const {
    return source;
}

shared_ptr<ClaimState> ClaimTransition::getTarget() const {
    return target;
}

shared_ptr<BoolExp> ClaimTransition::getBoolExp() const {
    return boolExp;
}

void ClaimTransition::checkRep() const {
    if (doCheckRep) {
        assertTrue(source);
        assertTrue(target);
        assertTrue(boolExp);
    }
}

} // namespace neverclaim
