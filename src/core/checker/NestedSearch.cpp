/*
 * NestedSearch.cpp
 *
 * Created on August 14, 2013
 */

#include "NestedSearch.hpp"

#include "util/Assert.hpp"

#include <glog/logging.h>

#include <ctime>

using core::features::BoolFeatureExpFactory;
using core::features::FeatureExp;

using core::fts::FTS;
using core::fts::PropertyStatePtr;
using core::fts::PropertyTransitionPtr;
using core::fts::StatePtr;
using core::fts::TransitionPtr;

using util::Generator;
using util::List;
using util::Stack;

using std::auto_ptr;
using std::clock;
using std::clock_t;
using std::tr1::shared_ptr;

namespace core {
namespace checker {

// constructors
NestedSearch::NestedSearch(
        shared_ptr<FeatureExp> fd,
        shared_ptr<FTS> fts,
        shared_ptr<BoolFeatureExpFactory> expFactory) :

        fd(fd), fts(fts), expFactory(expFactory),
        counterExamples(new List<CounterExample>()), algorithmState(Ready) {

    assertTrue(fts->hasProperty());

    propertyAutomaton = fts->getPropertyAutomaton();

    checkRep();
}

// public methods
void NestedSearch::run() {
    assertTrue(algorithmState == Ready);

    algorithmState = Running;
    clock_t start = clock();

    startOuterSearch();

    clock_t end = clock();
    algorithmState = Finished;

    LOG(INFO) << "CPU time used: "
              << 1000.0 * (end - start) / CLOCKS_PER_SEC
              << " ms\n";

    checkRep();
}

shared_ptr<FeatureExp> NestedSearch::getBadProducts() const {
    assertTrue(algorithmState == Finished);

    return badProducts;
}

shared_ptr<List<CounterExample> > NestedSearch::getCounterExamples() const {
    assertTrue(algorithmState == Finished);

    return counterExamples;
}

// private methods
bool NestedSearch::isSatisfiable(const shared_ptr<FeatureExp> & features) const {
    return auto_ptr<FeatureExp>(features->clone())->conjunction(
            *auto_ptr<FeatureExp>(validProducts->clone()))->isSatisfiable();
}

bool NestedSearch::findNextTransition(
        const shared_ptr<const FeatureExp> & features,
        TransitionPtr & transition,
        shared_ptr<FeatureExp> & nextFeatures) const {

    bool found = false;
    while (fts->hasNextTransition() && !found) {
        transition = fts->nextTransition();
        nextFeatures = shared_ptr<FeatureExp>(features->clone());
        nextFeatures->conjunction(*transition->getFeatureExp());
        if (isSatisfiable(nextFeatures)) {
            found = true;
        }
    }
    return found;
}

void NestedSearch::apply() {
    fts->apply(transition);
    state = fts->getCurrentState();
}

void NestedSearch::applyProperty() {
    propertyAutomaton->apply(propertyTransition);
    propertyState = propertyAutomaton->getCurrentState();
}

void NestedSearch::revert() {
    transition = fts->revert();
    state = fts->getCurrentState();
}

void NestedSearch::revertProperty() {
    propertyTransition = propertyAutomaton->revert();
    propertyState = propertyAutomaton->getCurrentState();
}

void NestedSearch::revertAll() {
    revert();
    revertProperty();
}

void NestedSearch::push(
        const shared_ptr<FeatureExp> & nextCurrentFeatures,
        const TransitionPtr & nextTransition,
        const shared_ptr<FeatureExp> & nextNextFeatures) {

    currentFeaturesStack.push(currentFeatures);
    nextFeaturesStack.push(nextFeatures);
    currentFeatures = nextCurrentFeatures;
    nextFeatures = nextNextFeatures;
    transition = nextTransition;
}

void NestedSearch::backtrack() {
    revertAll();
    currentFeatures = currentFeaturesStack.pop();
    nextFeatures = nextFeaturesStack.pop();
}

void NestedSearch::outerBacktrack() {
    CombinedState combinedState(state, propertyState);
    table[combinedState].flags &= ~IN_OUTER_PATH;
    if (fts->isInitialState()) {
        outerSearch = false;
    }
    else {
        backtrack();
    }
}

void NestedSearch::innerBacktrack() {
    CombinedState combinedState(state, propertyState);
    table[combinedState].flags &= ~IN_INNER_PATH;
    if (combinedState.equals(innerSearchInitialState)) {
        innerSearch = false;
    }
    else {
        backtrack();
    }
}

void NestedSearch::addToBadProducts() {
    counterExamples->addLast(CounterExample(*auto_ptr<FeatureExp>(
            nextFeatures->clone()), fts->getPath()));
    badProducts->disjunction(*auto_ptr<FeatureExp>(nextFeatures->clone()));
    validProducts->conjunction(*auto_ptr<FeatureExp>(nextFeatures->clone()->negation()));
}

void NestedSearch::deadlock() {
    LOG(ERROR) << "A deadlock has been found";
    // TODO
    assertTrue(false);
}

void NestedSearch::startOuterSearch() {

    // Declaring local variables
    bool found;
    shared_ptr<FeatureExp> verifiedFeatures;
    shared_ptr<FeatureExp> toVerifyFeatures;
    TransitionPtr nextTransition;
    shared_ptr<FeatureExp> nextNextFeatures;
    CombinedState combinedState;
    TableValue * tableValue;

    // Initializing results
    badProducts.reset(expFactory->getFalse());

    // Initializing outer search
    outerSearch = true;
    errorFound = false;
    validProducts = auto_ptr<FeatureExp>(fd->clone());
    state = fts->getCurrentState();
    propertyState = propertyAutomaton->getCurrentState();
    currentFeatures.reset(expFactory->getTrue());
    found = findNextTransition(currentFeatures, transition, nextFeatures);
    if (!found) {
        deadlock();
    }
    combinedState = CombinedState(state, propertyState);
    tableValue = &table[combinedState];
    tableValue->flags |= VISITED_IN_OUTER | IN_OUTER_PATH;
    tableValue->outerFeatures = shared_ptr<FeatureExp>(currentFeatures->clone());

    DVLOG(1) << "Starting outer search";

    while (outerSearch) {

        DVLOG(2) << "New iteration";

        if (errorFound) {
            /* The current path has led to highlight an error.
             * Consequently, the valid products expression has been updated,
             * the next features expression is no longer satisfiable with
             * relation it, and the current features expression could no longer
             * be either.
             */
            if (isSatisfiable(currentFeatures)) {
                DVLOG(3) << "Current features expression is still satisfiable";
                errorFound = false;
                found = findNextTransition(currentFeatures, transition, nextFeatures);
                if (found) {
                    propertyAutomaton->resetTransitions();
                }
                else {
                    DVLOG(3) << "Current state has no outgoing transition left";
                    if (propertyState->isAccepting()) {
                        DVLOG(3) << "Current state is an accepting state";
                        startInnerSearch();
                    }
                    DVLOG(3) << "Backtracking";
                    outerBacktrack();
                }
            }
            else {
                DVLOG(3) << "Current features expression is no longer satisfiable";
                DVLOG(3) << "Backtracking";
                outerBacktrack();
            }
        }
        else if (propertyAutomaton->hasNextTransition()) {
            DVLOG(3) << "Taking next transition";
            propertyTransition = propertyAutomaton->nextTransition();
            applyProperty();
            apply();
            DVLOG(3) << "Next state: " << CombinedState(state, propertyState).toString();
            if (propertyState->isFinal()) {
                DVLOG(3) << "Next state is final";
                DVLOG(1) << "Adding " << nextFeatures->toString() << " to bad products";
                addToBadProducts();
                DVLOG(3) << "Reverting";
                revertAll();
                errorFound = true;
            }
            else if (!propertyAutomaton->hasNextTransition()) {
                DVLOG(3) << "Next state has no outgoing transition";
                DVLOG(3) << "Reverting";
                revertAll();
            }
            else {
                combinedState = CombinedState(state, propertyState);
                tableValue = &table[combinedState];
                if (tableValue->flags & VISITED_IN_OUTER) {
                    DVLOG(3) << "Next state has already been visited";
                    if (tableValue->flags & IN_OUTER_PATH) {
                        DVLOG(3) << "Next state is in the current path";
                        DVLOG(3) << "Reverting";
                        revertAll();
                    }
                    else {
                        verifiedFeatures = tableValue->outerFeatures;
                        toVerifyFeatures = shared_ptr<FeatureExp>(nextFeatures->clone());
                        toVerifyFeatures->conjunction(*auto_ptr<FeatureExp>(verifiedFeatures->clone())->negation());
                        if (isSatisfiable(toVerifyFeatures)) {
                            DVLOG(3) << "At least one valid product is still to be verify";
                            verifiedFeatures->disjunction(*auto_ptr<FeatureExp>(toVerifyFeatures->clone()));
                            found = findNextTransition(toVerifyFeatures, nextTransition, nextNextFeatures);
                            if (found) {
                                push(toVerifyFeatures, nextTransition, nextNextFeatures);
                                tableValue->flags |= IN_OUTER_PATH;
                            }
                            else {
                                deadlock();
                            }
                        }
                        else {
                            DVLOG(3) << "Reverting";
                            revertAll();
                        }
                    }
                }
                else {
                    tableValue->outerFeatures = shared_ptr<FeatureExp>(nextFeatures->clone());
                    tableValue->flags |= VISITED_IN_OUTER;
                    found = findNextTransition(nextFeatures, nextTransition, nextNextFeatures);
                    if (found) {
                        push(nextFeatures, nextTransition, nextNextFeatures);
                        tableValue->flags |= IN_OUTER_PATH;
                    }
                    else {
                        deadlock();
                    }
                }
            }
        }
        else {
            found = findNextTransition(currentFeatures, transition, nextFeatures);
            if (found) {
                propertyAutomaton->resetTransitions();
            }
            else {
                DVLOG(3) << "Current state has no outgoing transition left";
                if (propertyState->isAccepting()) {
                    DVLOG(3) << "Current state is an accepting state";
                    startInnerSearch();
                }
                DVLOG(3) << "Backtracking";
                outerBacktrack();
            }
        }
    }


    DVLOG(1) << "Exiting outer search";
}

void NestedSearch::startInnerSearch() {

    // Declaring local variables
    bool found;
    shared_ptr<FeatureExp> verifiedFeatures;
    shared_ptr<FeatureExp> toVerifyFeatures;
    TransitionPtr nextTransition;
    shared_ptr<FeatureExp> nextNextFeatures;
    CombinedState combinedState;
    TableValue * tableValue;

    // Initializing inner search
    innerSearch = true;
    fts->resetTransitions();
    propertyAutomaton->resetTransitions();
    found = findNextTransition(currentFeatures, transition, nextFeatures);
    if (!found) {
        deadlock();
    }
    combinedState = CombinedState(state, propertyState);
    innerSearchInitialState = combinedState;
    tableValue = &table[combinedState];
    if (tableValue->flags & VISITED_IN_INNER) {
        tableValue->innerFeatures->disjunction(*currentFeatures->clone());
    }
    else {
        tableValue->flags |= VISITED_IN_INNER;
        tableValue->innerFeatures = shared_ptr<FeatureExp>(currentFeatures->clone());
    }
    tableValue->flags |= IN_INNER_PATH;

    DVLOG(1) << "Starting inner search";

    while (innerSearch) {

        DVLOG(2) << "New iteration";

        if (errorFound) {
            /* The current path has led to highlight an error.
             * Consequently, the valid products expression has been updated,
             * the next features expression is no longer satisfiable with
             * relation it, and the current features expression could no longer
             * be either.
             */
            if (isSatisfiable(currentFeatures)) {
                DVLOG(3) << "Current features expression is still satisfiable";
                errorFound = false;
                found = findNextTransition(currentFeatures, transition, nextFeatures);
                if (found) {
                    propertyAutomaton->resetTransitions();
                }
                else {
                    DVLOG(3) << "Current state has no outgoing transition left";
                    DVLOG(3) << "Backtracking";
                    innerBacktrack();
                }
            }
            else {
                DVLOG(3) << "Current features expression is no longer satisfiable";
                innerBacktrack();
            }
        }
        else if (propertyAutomaton->hasNextTransition()) {
            DVLOG(3) << "Taking next transition";
            propertyTransition = propertyAutomaton->nextTransition();
            applyProperty();
            apply();
            DVLOG(3) << "Next state: " << CombinedState(state, propertyState).toString();
            // At this stage, next state should not be final
            assertTrue(!propertyState->isFinal());
            if (!propertyAutomaton->hasNextTransition()) {
                DVLOG(3) << "Next state has no outgoing transition";
                revertAll();
            }
            else {
                combinedState = CombinedState(state, propertyState);
                tableValue = &table[combinedState];
                if (tableValue->flags & IN_OUTER_PATH) {
                    DVLOG(3) << "A cycle has been found";
                    DVLOG(1) << "Adding " << nextFeatures->toString() << " to bad products";
                    addToBadProducts();
                    DVLOG(3) << "Reverting";
                    revertAll();
                    errorFound = true;
                }
                else if (tableValue->flags & VISITED_IN_INNER) {
                    DVLOG(3) << "Next state has already been visited";
                    if (tableValue->flags & IN_INNER_PATH) {
                        DVLOG(3) << "Next state is in the current path";
                        DVLOG(3) << "Reverting";
                        revertAll();
                    }
                    else if (propertyState->isAccepting()) {
                        /* If the next state is an accepting state, it has already
                         * been visited for these features.
                         */
                        DVLOG(3) << "Next state is an accepting state";
                        DVLOG(3) << "Reverting";
                        revertAll();
                    }
                    else {
                        verifiedFeatures = tableValue->innerFeatures;
                        toVerifyFeatures = shared_ptr<FeatureExp>(nextFeatures->clone());
                        toVerifyFeatures->conjunction(*verifiedFeatures->clone()->negation());
                        if (isSatisfiable(toVerifyFeatures)) {
                            DVLOG(3) << "At least one valid product is still to be verify";
                            verifiedFeatures->disjunction(*toVerifyFeatures->clone());
                            found = findNextTransition(toVerifyFeatures, nextTransition, nextNextFeatures);
                            if (found) {
                                push(toVerifyFeatures, nextTransition, nextNextFeatures);
                                tableValue->flags |= IN_INNER_PATH;
                            }
                            else {
                                deadlock();
                            }
                        }
                        else {
                            DVLOG(3) << "Reverting";
                            revertAll();
                        }
                    }
                }
                else {
                    // At this stage, we should not encounter a non visited accepting state
                    assertTrue(!propertyState->isAccepting());
                    tableValue->innerFeatures = shared_ptr<FeatureExp>(nextFeatures->clone());
                    tableValue->flags |= VISITED_IN_INNER;
                    found = findNextTransition(nextFeatures, nextTransition, nextNextFeatures);
                    if (found) {
                        push(nextFeatures, nextTransition, nextNextFeatures);
                        tableValue->flags |= IN_INNER_PATH;
                    }
                    else {
                        deadlock();
                    }
                }
            }
        }
        else {
            found = findNextTransition(currentFeatures, transition, nextFeatures);
            if (found) {
                propertyAutomaton->resetTransitions();
            }
            else {
                DVLOG(3) << "Current state has no outgoing transition left";
                DVLOG(3) << "Backtracking";
                innerBacktrack();
            }
        }
    }

    DVLOG(1) << "Exiting inner search";
}

void NestedSearch::checkRep() const {
    if (doCheckRep) {
        // Nothing here
    }
}

} // namespace checker
} // namespace core
