/*
 * FSTM.cpp
 *
 * Created on 2013-07-30
 */

#include "FSTM.hpp"

#include "ChangeListener.hpp"
#include "ClaimPropertyAutomaton.hpp"
#include "FstmEvent.hpp"
#include "FstmState.hpp"
#include "FstmTransition.hpp"
#include "JsonParser.hpp"
#include "TransitionTable.hpp"
#include "core/fts/PropertyAutomaton.hpp"
#include "core/fts/State.hpp"
#include "util/Assert.hpp"
#include "core/features/FeatureExp.hpp"

#include <glog/logging.h>

#include <string>

using core::features::BoolFeatureExpFactory;
using std::istream;
using std::string;
using std::auto_ptr;
using std::tr1::dynamic_pointer_cast;
using std::tr1::shared_ptr;
using util::Generator;

namespace core {
namespace fts {
namespace fstm {

// constructors
FSTM::FSTM(istream & jsonFstm, const BoolFeatureExpFactory & featureExpFactory)
        : featureExpFactory(featureExpFactory), transitionTable(),
          propertyAutomaton(), changeListener(), currentState(), executables(),
          snapshots() {

    JsonParser parser(jsonFstm, featureExpFactory);

    transitionTable = TransitionTablePtr(new TransitionTable(
            parser.transitions(), parser.getNbOfTransitions()));

    currentState = transitionTable->getInitialState();
    executables = transitionTable->executables(*currentState);

    if (parser.getNbOfProperties() > 0) {
        // for now only the first property is verified
        shared_ptr<Generator<string> > propGen = parser.properties();
        propertyAutomaton = PropertyAutomatonPtr(new ClaimPropertyAutomaton(
                propGen->next(), currentState->getName()));
        changeListener =
                dynamic_pointer_cast<ChangeListener>(propertyAutomaton);
    }

    checkRep();
}

// public methods
 void FSTM::apply(const TransitionPtr & transition) {
    const FstmTransitionPtr & t =
            dynamic_pointer_cast<FstmTransition>(transition);

    assertTrue(t);
    assertTrue(currentState == t->getSource());

    snapshots.push(SnapshotPtr(new Snapshot(t, executables)));

    currentState = t->getTarget();
    resetTransitions();

    fireStateChangedEvent(t->getEvent()->getName());

    DVLOG(1) << "Apply ("
             << t->getSource()->getName() << ", "
             << t->getEvent()->getName() << ", "
             << t->getTarget()->getName() << ")"
             << " / " << t->getFeatureExp()->toString();

    checkRep();
 }

TransitionPtr FSTM::revert() {
    assertTrue(!isInitialState());

    SnapshotPtr snapshot = snapshots.pop();

    currentState = snapshot->applied->getSource();
    executables = snapshot->executables;

    if (snapshots.empty()) {
        fireStateChangedEvent(string(""));
    } else {
        fireStateChangedEvent(snapshots.peek()->applied->getEvent()->getName());
    }

    checkRep();
    return snapshot->applied;
}

StatePtr FSTM::getCurrentState() const {
    return currentState;
}

bool FSTM::isInitialState() const {
    return snapshots.count() <= 0;
}

bool FSTM::hasNextTransition() const {
    return executables->hasNext();
}

TransitionPtr FSTM::nextTransition() {
    assertTrue(hasNextTransition());

    // no checkRep but that's fine
    return executables->next();
}

void FSTM::resetTransitions() {
    executables = transitionTable->executables(*currentState);

    checkRep();
}

bool FSTM::hasProperty() const {
    return propertyAutomaton;
}

PropertyAutomatonPtr FSTM::getPropertyAutomaton() const {
    assertTrue(hasProperty());
    return propertyAutomaton;
}

auto_ptr<Generator<StatePtr> > FSTM::getPath() const {
    return auto_ptr<Generator<StatePtr> >(
            new PathGenerator(snapshots.elements(), currentState));
}

// private methods
void FSTM::fireStateChangedEvent(const string & eventName) {
    if (changeListener) {
        changeListener->stateChanged(currentState->getName(), eventName);
    }
}

void FSTM::checkRep() const {
    if (doCheckRep) {
        assertTrue(transitionTable);
        assertTrue(currentState);
        assertTrue(executables);
        if (snapshots.count() > 0) {
            assertTrue(currentState == snapshots.peek()->applied->getTarget());
        } else {
            assertTrue(currentState == transitionTable->getInitialState());
        }
        assertTrue((propertyAutomaton && changeListener)
                || (!propertyAutomaton && !changeListener));
    }
}

// PathGenerator
FSTM::PathGenerator::PathGenerator(auto_ptr<Generator<SnapshotPtr> > stackGen,
        StatePtr lastState)
        : stackGen(stackGen), lastState(lastState), hasNextState(true) {}

bool FSTM::PathGenerator::hasNext() const {
    return hasNextState;
}

StatePtr FSTM::PathGenerator::next() {
    assertTrue(hasNext());

    if (stackGen->hasNext()) {
        return stackGen->next()->applied->getSource();
    }
    else {
        hasNextState = false;
        return lastState;
    }
}

} // namespace fstm
} // namespace fts
} // namespace core
