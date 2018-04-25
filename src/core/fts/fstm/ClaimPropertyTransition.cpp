/*
 * ClaimPropertyTransition.cpp
 *
 * Created on 2013-08-07
 */

#include "ClaimPropertyTransition.hpp"

#include "neverclaim/ClaimTransition.hpp"
#include "util/Assert.hpp"

using neverclaim::ClaimStatePtr;
using neverclaim::ClaimTransitionPtr;
using std::tr1::shared_ptr;
using util::parser::BoolExp;

namespace core {
namespace fts {
namespace fstm {

// constructors
ClaimPropertyTransition::ClaimPropertyTransition(const ClaimTransitionPtr &
        claimTransition) : claimTransition(claimTransition) {
    checkRep();
}

// public methods
ClaimStatePtr ClaimPropertyTransition::getSource() const {
    return claimTransition->getSource();
}

ClaimStatePtr ClaimPropertyTransition::getTarget() const {
    return claimTransition->getTarget();
}

shared_ptr<BoolExp> ClaimPropertyTransition::getBoolExp() const {
    return claimTransition->getBoolExp();
}

// private methods
void ClaimPropertyTransition::checkRep() const {
    if (doCheckRep) {
        assertTrue(claimTransition);
    }
}

} // namespace fstm
} // namespace fts
} // namespace core
