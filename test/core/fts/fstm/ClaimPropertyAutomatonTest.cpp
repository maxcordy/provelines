/*
 * ClaimPropertyAutomatonTest.cpp
 *
 * Created on 2013-08-14
 */

#include "gtest/gtest.h"

#include "config/Context.hpp"
#include "core/features/BoolFeatureExpFactory.hpp"
#include "core/fts/fstm/FSTM.hpp"
#include "core/fts/State.hpp"
#include "core/fts/fstm/FstmState.hpp"
#include "core/fts/fstm/FstmTransition.hpp"
#include "core/fts/fstm/ClaimPropertyAutomaton.hpp"
#include "core/fts/fstm/ClaimPropertyState.hpp"
#include "core/fts/fstm/ClaimPropertyTransition.hpp"
#include "neverclaim/ClaimState.hpp"
#include "util/Generator.hpp"

#include <memory>
#include <sstream>
#include <tr1/memory>

using namespace core::fts;
using namespace core::fts::fstm;
using namespace neverclaim;
using config::Context;
using core::features::BoolFeatureExpFactory;
using std::stringstream;
using std::tr1::dynamic_pointer_cast;
using std::tr1::shared_ptr;
using util::Generator;

namespace core {
namespace fts {
namespace fstm {

/**
 * Unit tests for the ClaimPropertyAutomaton class.
 *
 * @author mwi
 */
class ClaimPropertyAutomatonTest : public ::testing::Test {

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
    PropertyAutomatonPtr target;
    shared_ptr<FTS> fts;
    shared_ptr<BoolFeatureExpFactory> featureExpFactory;

    StatePtr state1;
    StatePtr state2;
    StatePtr state3;

    ClaimStatePtr claimState1;
    ClaimStatePtr claimState2;

    PropertyStatePtr propState1;
    PropertyStatePtr propState2;

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

        state1 = FstmState::makeState("State1");
        state2 = FstmState::makeState("State2");
        state3 = FstmState::makeState("State3");

        claimState1 = ClaimState::makeState("T0_init");
        claimState2 = ClaimState::makeState("accept_all");

        propState1 = ClaimPropertyState::makePropertyState(claimState1);
        propState2 = ClaimPropertyState::makePropertyState(claimState2);

        fts = shared_ptr<FSTM>(new FSTM(jsonFstm, *featureExpFactory));
        target = fts->getPropertyAutomaton();
    }

    /**
     * Tears down the test fixture.
     */
    void TearDown() {}
};

TEST_F(ClaimPropertyAutomatonTest, initialState) {
    // Exercise & Verify
    ASSERT_TRUE(target->isInitialState());
    ASSERT_EQ(propState1, target->getCurrentState());
}

TEST_F(ClaimPropertyAutomatonTest, initialState_false) {
    // Setup
    target->apply(target->nextTransition());

    // Exercise & Verify
    ASSERT_FALSE(target->isInitialState());
    ASSERT_EQ(propState1, target->getCurrentState());
    ASSERT_TRUE(target->hasNextTransition());
}

TEST_F(ClaimPropertyAutomatonTest, executables) {
    // Exercise & Verify
    ASSERT_TRUE(target->hasNextTransition());
    ClaimPropertyTransitionPtr t = dynamic_pointer_cast<ClaimPropertyTransition>
            (target->nextTransition());
    ASSERT_FALSE(target->hasNextTransition());

    ASSERT_EQ(claimState1, t->getSource());
    ASSERT_EQ(claimState1, t->getTarget());
}

TEST_F(ClaimPropertyAutomatonTest, apply) {
    // Setup
    fts->apply(fts->nextTransition());
    target->nextTransition();
    PropertyTransitionPtr t = target->nextTransition();

    // Exercise
    target->apply(t);

    // Verify
    ASSERT_EQ(propState2, target->getCurrentState());
    ASSERT_FALSE(target->hasNextTransition());
    ASSERT_FALSE(target->isInitialState());
}

TEST_F(ClaimPropertyAutomatonTest, revert) {
    // Setup
    PropertyTransitionPtr expectedTrans = target->nextTransition();
    target->apply(expectedTrans);

    // Exercise
    PropertyTransitionPtr actualTrans = target->revert();

    // Verify
    ASSERT_EQ(expectedTrans, actualTrans);
    ASSERT_EQ(propState1, target->getCurrentState());
    ASSERT_TRUE(target->isInitialState());
    ASSERT_FALSE(target->hasNextTransition());
}

TEST_F(ClaimPropertyAutomatonTest, resetTransitions) {
    // Setup
    PropertyTransitionPtr expected = target->nextTransition();

    // Exercise
    target->resetTransitions();

    // Verify
    ASSERT_TRUE(target->hasNextTransition());
    PropertyTransitionPtr actual = target->nextTransition();
    ASSERT_FALSE(target->hasNextTransition());
    ASSERT_EQ(expected, actual);
}

} // namespace fstm
} // namespace fts
} // namespace core
