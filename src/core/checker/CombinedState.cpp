/* 
 * CombinedState.cpp
 * 
 * Created on August 14, 2013
 */

#include "CombinedState.hpp"

#include "util/Assert.hpp"

using core::fts::StatePtr;
using core::fts::PropertyStatePtr;

using std::string;

namespace core {
namespace checker {

CombinedState::CombinedState() {
    checkRep();
}

CombinedState::CombinedState(const StatePtr state,
        const PropertyStatePtr propertyState) :
state(state), propertyState(propertyState) {
    checkRep();
}

string CombinedState::toString() const {
    return "(" + state->getName() + ", " + propertyState->getLabel() + ")";
}

bool CombinedState::equals(const CombinedState & other) const {
    return state->equals(*other.state)
            && propertyState->equals(*other.propertyState);
}

unsigned int CombinedState::hashCode() const {
    unsigned int hashcode = 3;
    hashcode += 5 * state->hashCode();
    hashcode += 7 * propertyState->hashCode();
    return hashcode;
}

void CombinedState::checkRep() const {
    if (doCheckRep) {
        // Nothing here
    }
}
    
    
} // namespace checker
} // namespace core