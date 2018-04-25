/*
 * File:   ClaimPropertyAutomaton.cpp
 *
 * Created on 2013-08-13
 */

#include "ClaimPropertyAutomaton.hpp"

#include "ClaimPropertyState.hpp"
#include "ClaimPropertyTransition.hpp"
#include "FstmEvalVisitor.hpp"

#include "neverclaim/NeverClaim.hpp"

#include "util/Assert.hpp"
#include "util/Stack.hpp"

#include "util/parser/EvalVisitor.hpp"
#include "util/parser/BoolExp.hpp"

#include <glog/logging.h>

#include <utility>

using neverclaim::ClaimState;
using neverclaim::ClaimStatePtr;
using neverclaim::ClaimTransitionGeneratorPtr;
using neverclaim::NeverClaim;

using std::make_pair;
using std::pair;
using std::string;
using std::vector;
using std::tr1::dynamic_pointer_cast;
using std::tr1::shared_ptr;

using util::parser::EvalVisitor;

namespace core {
namespace fts {
namespace fstm {

// constructors
ClaimPropertyAutomaton::ClaimPropertyAutomaton(const string & ltl,
        const string & curFtsState)
        : neverClaim(NeverClaim::create(ltl)), currentFtsState(curFtsState),
          currentFtsEvent(), currentState(neverClaim->getInitialState()),
          executables(), nextIndex(0), fetchedIndex(), snapshots() {

    executables.reset(getExecutables(*currentState));

    checkRep();
}

// public methods
void ClaimPropertyAutomaton::apply(const PropertyTransitionPtr & transition) {
    const ClaimPropertyTransitionPtr & t =
            dynamic_pointer_cast<ClaimPropertyTransition>(transition);

    assertTrue(t);
    assertTrue(currentState == t->getSource());

    snapshots.push(SnapshotPtr(new Snapshot(t, executables, nextIndex)));

    currentState = t->getTarget();
    executables.reset(getExecutables(*currentState));
    nextIndex = 0;
    fetchedIndex.unset();

    DVLOG(5) << "current state changed to " << currentState->getLabel();

    checkRep();
}

PropertyTransitionPtr ClaimPropertyAutomaton::revert() {
    assertTrue(!isInitialState());

    SnapshotPtr snapshot = snapshots.pop();

    currentState = snapshot->applied->getSource();
    executables = snapshot->executables;
    nextIndex = snapshot->nextIndex;
    fetchedIndex.unset();

    checkRep();
    return snapshot->applied;
}

PropertyStatePtr ClaimPropertyAutomaton::getCurrentState() const {
    return ClaimPropertyState::makePropertyState(currentState);
}

bool ClaimPropertyAutomaton::isInitialState() const {
    return snapshots.count() <= 0;
}

bool ClaimPropertyAutomaton::hasNextTransition() const {
    return getNextTransitionIndex() != -1;
}

PropertyTransitionPtr ClaimPropertyAutomaton::nextTransition() {
    int i = getNextTransitionIndex();
    assertTrue(i >= 0);

    nextIndex = i + 1;
    fetchedIndex.unset();

    checkRep();
    return executables->at(i);
}

void ClaimPropertyAutomaton::resetTransitions() {
    nextIndex = 0;
    fetchedIndex.unset();

    checkRep();
}

void ClaimPropertyAutomaton::stateChanged(const string & newState,
        const string & newEvent) {
    assertTrue(newState.length() > 0);

    currentFtsState = newState;
    currentFtsEvent = newEvent;

    fetchedIndex.unset();

    checkRep();
}

// private methods
vector<ClaimPropertyTransitionPtr> *
ClaimPropertyAutomaton::getExecutables(const ClaimState & state) const {
    vector<ClaimPropertyTransitionPtr> * transitions =
            new vector<ClaimPropertyTransitionPtr>();

    ClaimTransitionGeneratorPtr g = neverClaim->executables(state);
    while (g->hasNext()) {
        ClaimPropertyTransitionPtr t(new ClaimPropertyTransition(g->next()));
        transitions->push_back(t);
    }

    return transitions;
}

int ClaimPropertyAutomaton::getNextTransitionIndex() const {
    if (fetchedIndex.isSet()) {
        return fetchedIndex.get();
    }

    FstmEvalVisitor visitor(currentFtsState, currentFtsEvent);
    for (unsigned int i = nextIndex; i < executables->size(); i++) {
        visitor.reset();
        executables->at(i)->getBoolExp()->accept(visitor);
        if (visitor.isTrue()) {
            fetchedIndex.set(i);
            return i;
        }
    }
    return -1;
}

void ClaimPropertyAutomaton::checkRep() const {
    if (doCheckRep) {
        assertTrue(neverClaim);
        assertTrue(currentFtsState.length() > 0);
        assertTrue(currentState);
        assertTrue(executables);
        assertTrue(nextIndex <= executables->size());
        if (snapshots.count() > 0) {
            assertTrue(currentState == snapshots.peek()->applied->getTarget());
        } else {
            assertTrue(currentState == neverClaim->getInitialState());
        }
    }
}

} // namespace fstm
} // namespace fts
} // namespace core
