/*
 * ClaimState.cpp
 *
 * Created on 2013-08-02
 */

#include "ClaimState.hpp"
#include "util/Assert.hpp"

#include <tr1/functional>

using std::pair;
using std::string;
using std::tr1::hash;
using std::tr1::shared_ptr;
using std::tr1::unordered_map;

namespace neverclaim {

const string ClaimState::Accept("accept_");
const string ClaimState::AcceptAll("accept_all");

unordered_map<string, shared_ptr<ClaimState> > ClaimState::stateTable;

// private constructors
ClaimState::ClaimState(const string & label)
: label(label) {
    checkRep();
}

// public methods
const string & ClaimState::getLabel() const {
    return label;
}

bool ClaimState::isAccepting() const {
    return label.find(Accept) == 0;
}

bool ClaimState::isFinal() const {
    return label == AcceptAll;
}

bool ClaimState::equals(const ClaimState & state) const {
    return this == &state;
}

unsigned int ClaimState::hashCode() const {
    hash<string> hashFct;
    unsigned int hashValue = 3;
    hashValue = 19 * hashValue + hashFct(label);
    return hashValue;
}

// static factory methods
shared_ptr<ClaimState> ClaimState::makeState(const string & label) {
    assertTrue(label.length() > 0);

    if (stateTable.count(label) <= 0) {
        shared_ptr<ClaimState> state(new ClaimState(label));
        stateTable.insert(pair<string, shared_ptr<ClaimState> >(label, state));
    }
    return stateTable[label];
}

// private methods
void ClaimState::checkRep() const {
    if (doCheckRep) {
        // TODO: Rep invariant incomplete but hard to do without regexp.
        assertTrue(label.length() > 0);
    }
}

} // namespace neverclaim
