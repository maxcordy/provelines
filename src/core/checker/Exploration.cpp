/*
 * Exploration.cpp
 *
 * Created on 2013-08-22
 */

#include "Exploration.hpp"

#include "util/Assert.hpp"
#include "util/StdHashTable.hpp"

#include <glog/logging.h>

#include <ctime>
#include <tr1/unordered_map>

using core::features::BoolFeatureExpFactory;
using core::features::FeatureExp;
using core::features::FeatureExpPtr;

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
using std::make_pair;
using std::tr1::shared_ptr;
using std::tr1::unordered_map;

namespace core {
namespace checker {

// constructors
Exploration::Exploration(
        shared_ptr<FeatureExp> fd,
        shared_ptr<FTS> fts,
        shared_ptr<BoolFeatureExpFactory> expFactory) :

        fd(fd), fts(fts), expFactory(expFactory),
        counterExamples(new List<CounterExample>()), algorithmState(Ready) {

    checkRep();
}

// public methods
void Exploration::run() {
    assertTrue(algorithmState == Ready);

    algorithmState = Running;
    clock_t start = clock();

    startExploration();

    clock_t end = clock();
    algorithmState = Finished;

    LOG(INFO) << "CPU time used: "
              << 1000.0 * (end - start) / CLOCKS_PER_SEC
              << " ms\n";

    checkRep();
}

shared_ptr<FeatureExp> Exploration::getBadProducts() const {
    assertTrue(algorithmState == Finished);

    return badProducts;
}

shared_ptr<List<CounterExample> > Exploration::getCounterExamples() const {
    assertTrue(algorithmState == Finished);

    return counterExamples;
}

// private methods
bool Exploration::isSatisfiable(const FeatureExp & features) const {
    return auto_ptr<FeatureExp>(features.clone())->conjunction(
            *auto_ptr<FeatureExp>(validProducts->clone()))->isSatisfiable();
}

void Exploration::backtrack() {
    if (fts->isInitialState()) {
        exploration = false;
    }
    else {
        fts->revert();
        features = featuresStack.pop();
    }
}

void Exploration::checkForDeadlock() {
    auto_ptr<FeatureExp> deadlockFeatures(features->clone());
    while(fts->hasNextTransition()) {
        TransitionPtr transition = fts->nextTransition();
        deadlockFeatures->conjunction(*transition->getFeatureExp()->negation());
    }
    fts->resetTransitions();
    if (isSatisfiable(*deadlockFeatures)) {
        deadlockFound = true;
        counterExamples->addLast(
                CounterExample(*deadlockFeatures, fts->getPath()));
        badProducts->disjunction(*deadlockFeatures->clone());
        validProducts->conjunction(*deadlockFeatures->clone()->negation());
    }
}

void Exploration::startExploration() {

    // Declaring local variables
    TableValue * tableValue;
    shared_ptr<FeatureExp> verifiedFeatures;

    // Initializing exploration
    exploration = true;
    deadlockFound = false;
    validProducts = auto_ptr<FeatureExp>(fd->clone());
    badProducts.reset(expFactory->getFalse());
    state = fts->getCurrentState();
    features.reset(expFactory->getTrue());
    tableValue = &table[state];
    tableValue->flags |= VISITED | IN_PATH;
    tableValue->features = FeatureExpPtr(features->clone());

    DVLOG(1) << "Starting exploration";

    checkForDeadlock();

    int depth = 0;      // The current depth of the exploration.
    int explored = 1;   // The number of explored states.
    int reexplored = 0; // The number of re-explored states.

    while(exploration) {

        DVLOG(2) << "New iteration";

        if (deadlockFound) {
            /* The current path has led to highlight a deadlock.
             * Consequently, the valid products expression has been updated
             * and the current features expression could no longer be
             * satisfiable.
             */
            if (isSatisfiable(*features)) {
                deadlockFound = false;
            }
            else {
                DVLOG(3) << "Current features expression is no longer satisfiable";
                DVLOG(3) << "Backtracking";
                backtrack();
            }
        }
        else {
            DVLOG(4) << "Current state: " << fts->getCurrentState()->getName();
            DVLOG(4) << "Depth: " << depth;
            if (fts->hasNextTransition()) {
                transition = fts->nextTransition();
                // Next transition
                auto_ptr<FeatureExp> nextFeatures(features->clone());
                nextFeatures->conjunction(*transition->getFeatureExp());
                if (isSatisfiable(*nextFeatures)) {
                    // At least one valid product can execute the transition
                    fts->apply(transition);
                    ++depth;
                    state = fts->getCurrentState();
                    DVLOG(3) << "Next state: " << state->getName();
                    tableValue = &table[state];
                    if (tableValue->flags & VISITED) {
                        DVLOG(3) << "Next state has already been visited";
                        if (tableValue->flags & IN_PATH) {
                            DVLOG(3) << "Next state is in the current path";
                            DVLOG(3) << "Reverting";
                            fts->revert();
                            --depth;
                        }
                        else {
                            verifiedFeatures = tableValue->features;
                            nextFeatures->conjunction(*auto_ptr<FeatureExp>(verifiedFeatures->clone())->negation());
                            if (isSatisfiable(*nextFeatures)) {
                                // At least one valid product is still to be verify
                                verifiedFeatures->disjunction(*auto_ptr<FeatureExp>(nextFeatures->clone()));
                                featuresStack.push(features);
                                ++reexplored;
                                features = nextFeatures;
                                checkForDeadlock();
                            }
                            else {
                                DVLOG(3) << "Reverting";
                                fts->revert();
                                --depth;
                            }
                        }
                    }
                    else {
                        tableValue->features = FeatureExpPtr(nextFeatures->clone());
                        tableValue->flags |= VISITED;
                        featuresStack.push(features);
                        ++explored;
                        features = nextFeatures;
                        checkForDeadlock();
                    }
                }
            }
            else {
                DVLOG(3) << "Current state has no outgoing transition left";
                DVLOG(3) << "Backtracking";
                backtrack();
                --depth;
            }
        }
    }

    LOG(INFO) << "Exhaustive exploration complete ["
              << explored << " explored, " << reexplored << " re-explored].";
}

//void Exploration::startLightExploration() {
//    auto_ptr<FeatureExp> curFD(fd->clone());
//
//    StatePtr init = fts->getCurrentState();
//
//    Stack<FeatureExpPtr> featureStack;
//    featureStack.push(expFactory->getTrue());
//
//    unordered_map<unsigned int, FeatureExpPtr> reachables;
//    reachables.insert(make_pair(init->hashCode(), expFactory->getTrue()));
//
//    int explored = 1;
//    int reexplored = 0;
//
//    badProducts = expFactory->getFalse();
//
//    while (!featureStack.empty()) {
//
//        DVLOG(1) << "Current state is " << fts->getCurrentState()->getName();
//
//        FeatureExpPtr currentFeatures = featureStack.peek();
//
//        if (!fts->hasNextTransition()) {
//            // currentState has no new outgoing transition to analyze
//            DVLOG(1) << "Revert (no new outgoing transition to analyze)";
//            featureStack.pop();
//            if (!fts->isInitialState()) {
//                fts->revert();
//            }
//        } else {
//            // currentState has at least one new transition to analyze
//            TransitionPtr newTransition = fts->nextTransition();
//            FeatureExpPtr conjunction = currentFeatures->clone();
//            conjunction->conjunction(*newTransition->getFeatureExp());
//
//            if (isSatisfiableWrtFD(curFD, conjunction)) {
//                // At least one valid product can execute the transition
//                fts->apply(newTransition);
//                StatePtr nextState = fts->getCurrentState();
//
//                bool foundInReachables = reachables.count(nextState->hashCode()) == 1;
//
//                unordered_map<unsigned int, FeatureExpPtr>::const_iterator it;
//                it = reachables.find(nextState->hashCode());
//                if (it != reachables.end()) {
//                    FeatureExpPtr previousFeatures = it->second->clone();
//                    conjunction->conjunction(*previousFeatures->negation());
//                }
//
//                if (!isSatisfiableWrtFD(curFD, conjunction)) {
//                    DVLOG(1) << "Revert (" << nextState->getName()
//                             << " already visited for the current set of products)";
//                    // State was already visited for the current set of products
//                    fts->revert();
//                } else {
//                    // State is new for at least one valid product
//                    if (!fts->hasNextTransition()) {
//                        DVLOG(1) << "Deadlock found for products "
//                                 << conjunction->toString();
//                        // Found deadlock; adding products to bad set
//                        badProducts->disjunction(*conjunction->clone());
//                        FeatureExpPtr constraint = conjunction->clone();
//                        curFD->conjunction(*constraint->negation());
//                    } else {
//                        // No deadlock found; adding on stack
//                        featureStack.push(conjunction);
//                    }
//
//                    // Updating hashtable and statistical data
//                    if (!foundInReachables) {
//                        // State is explored for the first time
//                        reachables.insert(make_pair(nextState->hashCode(), conjunction));
//                        ++explored;
//                        DVLOG(1) << "Explored "
//                                 << fts->getCurrentState()->getName();
//                    } else {
//                        // State is reexplored
//                        reachables[nextState->hashCode()]->disjunction(*conjunction->clone());
//                        ++reexplored;
//                        DVLOG(1) << "Re-explored "
//                                 << fts->getCurrentState()->getName();
//                    }
//                }
//            } else {
//                DVLOG(1) << "Next transition cannot be fired by the current set of products";
//            }
//        }
//    }
//
//    LOG(INFO) << "Exhaustive exploration complete ["
//              << explored << " explored, " << reexplored << " re-explored].";
//}

//bool Exploration::isSatisfiableWrtFD(const FeatureExpPtr& fd,
//        const FeatureExpPtr & featureExp) {
//    FeatureExpPtr tmp = fd->clone();
//    tmp->conjunction(*featureExp->clone());
//    return tmp->isSatisfiable();
//}

void Exploration::checkRep() const {
    if (doCheckRep) {
        // Nothing here
    }
}

} // namespace checker
} // namespace core
