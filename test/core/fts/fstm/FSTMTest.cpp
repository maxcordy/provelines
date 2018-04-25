/*
 * FSTMTest.cpp
 *
 * Created on 2013-07-30
 */

#include "gtest/gtest.h"

#include "config/Context.hpp"
#include "core/features/BoolFeatureExpFactory.hpp"
#include "core/fts/PropertyAutomaton.hpp"
#include "core/fts/State.hpp"
#include "core/fts/fstm/ClaimPropertyTransition.hpp"
#include "core/fts/fstm/FSTM.hpp"
#include "core/fts/fstm/FstmState.hpp"
#include "core/fts/fstm/FstmTransition.hpp"
#include "neverclaim/ClaimState.hpp"
#include "util/Generator.hpp"

#include <glog/logging.h>
#include <sstream>

using namespace neverclaim;
using config::Context;
using core::features::BoolFeatureExpFactory;
using core::fts::State;
using std::stringstream;
using std::tr1::dynamic_pointer_cast;
using std::tr1::shared_ptr;
using util::Generator;

namespace core {
namespace fts {
namespace fstm {

/**
 * Unit tests for the FSTM class.
 *
 * @author mwi
 */
class FSTMTest : public ::testing::Test {

/*
 * never claim of !([]!(state2))
 * T0_init:
 *	if
 *	:: (1) -> goto T0_init
 *	:: (state2) -> goto accept_all
 *	fi;
 * accept_all:
 *	skip
 */

protected:
    shared_ptr<FSTM> target;
    shared_ptr<FSTM> targetWithoutProperty;
    shared_ptr<BoolFeatureExpFactory> featureExpFactory;

    StatePtr state1;
    StatePtr state2;
    StatePtr state3;

    ClaimStatePtr claimState1;
    ClaimStatePtr claimState2;

    /**
     * Sets up the test fixture.
     */
    void SetUp() {
        Context ctx;
        featureExpFactory = ctx.getBoolFeatureExpFactory();

        stringstream jsonFstm;
        jsonFstm << "{\"FSTM\":{"
                 << "\"name\":\"Sample3\","
                 << "\"generated\":\"Wed Jul 31 10:37:49 CEST 2013\","
                 << "\"nbOfLtlProperties\":1,"
                 << "\"nbOfTransitions\":7},"
                 << "\"states\":[\"State1\", \"State2\", \"State3\"],"
                 << "\"events\":[\"evt1\",\"evt2\"],"
                 << "\"ltlProperties\":[\"[]!(state2)\"],"
                 << "\"transitions\":["
                 << "{\"source\":\"State1\","
                 << "\"target\":\"State2\","
                 << "\"event\":\"evt1\","
                 << "\"guard\":\"\","
                 << "\"featureExp\":\"A && B\"},"
                 << "{\"source\":\"State1\","
                 << "\"target\":\"State3\","
                 << "\"event\":\"evt2\","
                 << "\"guard\":\"\","
                 << "\"featureExp\":null},"
                 << "{\"source\":\"State2\","
                 << "\"target\":\"State1\","
                 << "\"event\":\"evt1\","
                 << "\"guard\":\"\","
                 << "\"featureExp\":\"!(A)\"},"
                 << "{\"source\":\"State2\","
                 << "\"target\":\"State3\","
                 << "\"event\":\"evt2\","
                 << "\"guard\":\"\","
                 << "\"featureExp\":null},"
                 << "{\"source\":\"State2\","
                 << "\"target\":\"State2\","
                 << "\"event\":\"evt1\","
                 << "\"guard\":\"\","
                 << "\"featureExp\":\"A\"},"
                 << "{\"source\":\"State3\","
                 << "\"target\":\"State2\","
                 << "\"event\":\"evt1\","
                 << "\"guard\":\"\","
                 << "\"featureExp\":\"A && B\"},"
                 << "{\"source\":\"State3\","
                 << "\"target\":\"State3\","
                 << "\"event\":\"evt2\","
                 << "\"guard\":\"\","
                 << "\"featureExp\":null}"
                 << "]}";

        stringstream jsonFstm2;
        jsonFstm2 << "{\"FSTM\":{"
                 << "\"name\":\"Sample3\","
                 << "\"generated\":\"Wed Jul 31 10:37:49 CEST 2013\","
                 << "\"nbOfLtlProperties\":0,"
                 << "\"nbOfTransitions\":7},"
                 << "\"states\":[\"State1\", \"State2\", \"State3\"],"
                 << "\"events\":[\"evt1\",\"evt2\"],"
                 << "\"ltlProperties\":[],"
                 << "\"transitions\":["
                 << "{\"source\":\"State1\","
                 << "\"target\":\"State2\","
                 << "\"event\":\"evt1\","
                 << "\"guard\":\"\","
                 << "\"featureExp\":\"A && B\"}"
                 << "]}";

        state1 = FstmState::makeState("State1");
        state2 = FstmState::makeState("State2");
        state3 = FstmState::makeState("State3");

        claimState1 = ClaimState::makeState("T0_init");
        claimState2 = ClaimState::makeState("accept_all");

        target = shared_ptr<FSTM>(new FSTM(jsonFstm, *featureExpFactory));
        targetWithoutProperty = shared_ptr<FSTM>(new FSTM(jsonFstm2,
                *featureExpFactory));
                
    }

    /**
     * Tears down the test fixture.
     */
    void TearDown() {}
};

/**
 * Tests the FSTM is in its initial state once created.
 */
TEST_F(FSTMTest, initialState) {
    // Exercise
    StatePtr actualState = target->getCurrentState();

    // Verify
    ASSERT_EQ(state1, actualState);
    ASSERT_TRUE(target->isInitialState());
}

/*
 * Tests the executable transitions of the FSTM.
 */
TEST_F(FSTMTest, executables) {
    // Exercise & Verify
    ASSERT_TRUE(target->hasNextTransition());
    FstmTransitionPtr t1 =
            dynamic_pointer_cast<FstmTransition>(target->nextTransition());
    ASSERT_TRUE(target->hasNextTransition());
    FstmTransitionPtr t2 =
            dynamic_pointer_cast<FstmTransition>(target->nextTransition());
    ASSERT_FALSE(target->hasNextTransition());

    ASSERT_EQ(state1, t1->getSource());
    ASSERT_EQ(state2, t1->getTarget());

    ASSERT_EQ(state1, t2->getSource());
    ASSERT_EQ(state3, t2->getTarget());
}

/**
 * Tests the executable transitions of the FSTM for a state without any leaving
 * transition.
 */
TEST_F(FSTMTest, executables_noTransition) {
    // Setup
    targetWithoutProperty->apply(targetWithoutProperty->nextTransition());

    // Exercise & Verify
    ASSERT_FALSE(targetWithoutProperty->hasNextTransition());
}

/**
 * Tests the resetTransitions method.
 */
TEST_F(FSTMTest, resetTransitions) {
    // Setup
    target->nextTransition();
    target->nextTransition();

    // Exercise
    target->resetTransitions();

    // Verify
    ASSERT_TRUE(target->hasNextTransition());
    FstmTransitionPtr t1 =
            dynamic_pointer_cast<FstmTransition>(target->nextTransition());
    ASSERT_TRUE(target->hasNextTransition());
    FstmTransitionPtr t2 =
            dynamic_pointer_cast<FstmTransition>(target->nextTransition());
    ASSERT_FALSE(target->hasNextTransition());

    ASSERT_EQ(state1, t1->getSource());
    ASSERT_EQ(state2, t1->getTarget());

    ASSERT_EQ(state1, t2->getSource());
    ASSERT_EQ(state3, t2->getTarget());
}

TEST_F(FSTMTest, hasProperty) {
    // Exercise & Verify
    ASSERT_TRUE(target->hasProperty());
}

TEST_F(FSTMTest, hasProperty_false) {
    // Exercise & Verify
    ASSERT_FALSE(targetWithoutProperty->hasProperty());
}

TEST_F(FSTMTest, getPropertyAutomaton) {
    // Exercise
    PropertyAutomatonPtr automaton = target->getPropertyAutomaton();

    // Verify
    ASSERT_TRUE(automaton);
}

TEST_F(FSTMTest, apply) {
    // Setup
    TransitionPtr t = target->nextTransition();

    // Exercise
    target->apply(t);

    // Verify
    ASSERT_EQ(state2, target->getCurrentState());
    ASSERT_FALSE(target->isInitialState());
}

TEST_F(FSTMTest, revert) {
    // Setup
    TransitionPtr expectedTrans = target->nextTransition();
    target->apply(expectedTrans);

    // Exercise
    TransitionPtr actualTrans = target->revert();

    // Verify
    ASSERT_EQ(expectedTrans, actualTrans);
    ASSERT_EQ(state1, target->getCurrentState());
    ASSERT_TRUE(target->isInitialState());
    TransitionPtr t2 = target->nextTransition();
    ASSERT_NE(expectedTrans, t2);
    ASSERT_FALSE(target->hasNextTransition());
}

/**
 * Tests the apply method on an FSTM without any property. This is a regression
 * test to ensure the proper use of ChangeListener.
 */
TEST_F(FSTMTest, applyWithoutProperty) {
    // Setup
    TransitionPtr t = targetWithoutProperty->nextTransition();

    // Exercise
    targetWithoutProperty->apply(t);

    // Verify
    ASSERT_EQ(state2, targetWithoutProperty->getCurrentState());
    ASSERT_FALSE(targetWithoutProperty->isInitialState());
}

TEST_F(FSTMTest, revertWithoutProperty) {
    // Setup
    TransitionPtr expectedTrans = targetWithoutProperty->nextTransition();
    targetWithoutProperty->apply(expectedTrans);

    // Exercise
    TransitionPtr actualTrans = targetWithoutProperty->revert();

    // Verify
    ASSERT_EQ(expectedTrans, actualTrans);
    ASSERT_EQ(state1, targetWithoutProperty->getCurrentState());
    ASSERT_TRUE(targetWithoutProperty->isInitialState());
    ASSERT_FALSE(targetWithoutProperty->hasNextTransition());
}

/**
 * Tests a state changed event is fired to the property automaton on the apply
 * operation.
 */
TEST_F(FSTMTest, fireStateChanged_apply) {
    // Setup
    PropertyAutomatonPtr propAutomaton = target->getPropertyAutomaton();

    // Exercise
    target->apply(target->nextTransition());

    // Verify
    ASSERT_TRUE(propAutomaton->hasNextTransition());
    ClaimPropertyTransitionPtr t1 =
            dynamic_pointer_cast<ClaimPropertyTransition>(
            propAutomaton->nextTransition());
    ASSERT_TRUE(propAutomaton->hasNextTransition());
    ClaimPropertyTransitionPtr t2 =
            dynamic_pointer_cast<ClaimPropertyTransition>(
            propAutomaton->nextTransition());
    ASSERT_FALSE(propAutomaton->hasNextTransition());

    ASSERT_EQ(claimState1, t1->getSource());
    ASSERT_EQ(claimState1, t1->getTarget());

    ASSERT_EQ(claimState1, t2->getSource());
    ASSERT_EQ(claimState2, t2->getTarget());
}

/**
 * Tests a state changed event is fired to the property automaton on the revert
 * operation.
 */
TEST_F(FSTMTest, fireStateChanged_revert) {
    // Setup
    PropertyAutomatonPtr propAutomaton = target->getPropertyAutomaton();
    target->apply(target->nextTransition());

    // Exercise
    target->revert();

    // Verify
    ASSERT_TRUE(propAutomaton->hasNextTransition());
    ClaimPropertyTransitionPtr t =
            dynamic_pointer_cast<ClaimPropertyTransition>(
            propAutomaton->nextTransition());
    ASSERT_FALSE(propAutomaton->hasNextTransition());

    ASSERT_EQ(claimState1, t->getSource());
    ASSERT_EQ(claimState1, t->getTarget());
}

TEST_F(FSTMTest, Cycle) {
    // Setup
    stringstream json;
    json << "{\"FSTM\":{"
            << "\"name\":\"Test3\","
            << "\"generated\":\"Wed Jul 31 10:37:49 CEST 2013\","
            << "\"nbOfLtlProperties\":0,"
            << "\"nbOfTransitions\":1},"
            << "\"states\":[\"State1\"],"
            << "\"events\":[\"evt1\"],"
            << "\"ltlProperties\":[],"
            << "\"transitions\":["
            << "{\"source\":\"State1\","
            << "\"target\":\"State1\","
            << "\"event\":\"evt1\","
            << "\"guard\":\"\","
            << "\"featureExp\":null}"
            << "]}";
    shared_ptr<FTS> fts = shared_ptr<FSTM>(new FSTM(json, *featureExpFactory));

    // Exercise
    ASSERT_TRUE(fts->hasNextTransition());
    TransitionPtr transition = fts->nextTransition();
    fts->apply(transition);

    // Verify
    ASSERT_TRUE(fts->hasNextTransition());
}

} // namespace fstm
} // namespace fts
} // namespace core
