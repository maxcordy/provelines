/*
 * ClaimPropertyState.cpp
 *
 * Created on 2013-08-13
 */

#include "ClaimPropertyState.hpp"

#include "neverclaim/ClaimState.hpp"
#include "util/Assert.hpp"

#include <utility>

using neverclaim::ClaimStatePtr;
using std::make_pair;
using std::string;
using std::tr1::unordered_map;

namespace core {
namespace fts {
namespace fstm {

unordered_map<unsigned int, core::fts::PropertyStatePtr> ClaimPropertyState::stateTable;

// constructors
ClaimPropertyState::ClaimPropertyState(const ClaimStatePtr & claimState)
        : claimState(claimState) {
    checkRep();
}

// public methods
const string & ClaimPropertyState::getLabel() const {
    return claimState->getLabel();
}

bool ClaimPropertyState::isAccepting() const {
    return claimState->isAccepting();
}

bool ClaimPropertyState::isFinal() const {
    return claimState->isFinal();
}

bool ClaimPropertyState::equals(const PropertyState & obj) const {
    const ClaimPropertyState & other =
            static_cast<const ClaimPropertyState &>(obj);
    return claimState->equals(*other.claimState);
}

unsigned int ClaimPropertyState::hashCode() const {
    return claimState->hashCode();
}

// static factory methods
PropertyStatePtr ClaimPropertyState::makePropertyState(const ClaimStatePtr &
        claimState) {
    assertTrue(claimState);

    if (stateTable.count(claimState->hashCode()) <= 0) {
        PropertyStatePtr propState(new ClaimPropertyState(claimState));
        stateTable.insert(make_pair(claimState->hashCode(), propState));
    }
    return stateTable[claimState->hashCode()];
}

// private methods
void ClaimPropertyState::checkRep() const {
    if (doCheckRep) {
        assertTrue(claimState);
    }
}

} // namespace fstm
} // namespace fts
} // namespace core
