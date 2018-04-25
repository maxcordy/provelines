/*
 * NeverClaimTest.cpp
 *
 * Created on 2013-08-04
 */

#include "gtest/gtest.h"

#include "core/fts/fstm/FstmEvalVisitor.hpp"
#include "util/parser/AndExp.hpp"
#include "util/parser/BoolExp.hpp"
#include "util/parser/BoolValue.hpp"
#include "util/parser/NotExp.hpp"
#include "util/parser/Var.hpp"
#include "neverclaim/NeverClaim.hpp"
#include "util/List.hpp"

using namespace util::parser;
using core::fts::fstm::FstmEvalVisitor;
using std::string;
using std::tr1::dynamic_pointer_cast;
using std::tr1::shared_ptr;
using util::List;

namespace neverclaim {

/**
 * Unit tests for the NeverClaim class.
 *
 * @author mwi
 */
class NeverClaimTest : public ::testing::Test {

protected:
    shared_ptr<NeverClaim> target;

    string state0;
    string state1;
    string state2;

    string stateA;
    string stateB;

    shared_ptr<BoolExp> trueExp;
    shared_ptr<BoolExp> aExp;
    shared_ptr<BoolExp> notA;

    void SetUp() {
        state0 = "T0_init";
        state1 = "S0";
        state2 = "accept_S1";

        stateA = "a";
        stateB = "b";

        trueExp = shared_ptr<BoolExp>(new BoolValue(true));
        aExp = shared_ptr<BoolExp>(new Var(stateA));
        notA = shared_ptr<BoolExp>(new NotExp(aExp));

        List<ClaimTransitionPtr> transitions;
        transitions.addLast(ClaimTransitionPtr(new ClaimTransition(state0,
                state1, aExp)));
        transitions.addLast(ClaimTransitionPtr(new ClaimTransition(state1,
                state2, trueExp)));
        transitions.addLast(ClaimTransitionPtr(new ClaimTransition(state1,
                state0, notA)));

        target = shared_ptr<NeverClaim>(new NeverClaim(transitions.elements()));
    }

    void TearDown() {
        // Nothing here
    }
};

TEST_F(NeverClaimTest, getInitialState) {
    // Setup
    shared_ptr<ClaimState> expectedState = ClaimState::makeState(state0);

    // Execise
    shared_ptr<ClaimState> actualState = target->getInitialState();

    // Verify
    ASSERT_EQ(expectedState, actualState);
}

/**
 * Tests the executables method with a state that is not in the never claim.
 */
TEST_F(NeverClaimTest, executables_notExistingState) {
    // Setup
    shared_ptr<ClaimState> state = ClaimState::makeState("UnkownState");

    // Exercise
    ClaimTransitionGeneratorPtr g = target->executables(*state);

    // Verify
    ASSERT_FALSE(g->hasNext());
}

/**
 * Tests the executable method with a generator that produces zero transition.
 */
TEST_F(NeverClaimTest, executables_zeroTransition) {
    // Setup
    shared_ptr<ClaimState> state = ClaimState::makeState(state2);

    // Exercise
    ClaimTransitionGeneratorPtr g = target->executables(*state);

    // Verify
    ASSERT_FALSE(g->hasNext());
}

/**
 * Tests the executable method with a generator that produces one transition.
 */
TEST_F(NeverClaimTest, executables_oneTransition) {
    // Setup
    shared_ptr<ClaimState> state = ClaimState::makeState(state0);

    // Exercise
    ClaimTransitionGeneratorPtr g = target->executables(*state);

    // Verify
    ASSERT_TRUE(g->hasNext());
    ClaimTransitionPtr trans = g->next();
    ASSERT_FALSE(g->hasNext());
    ASSERT_EQ(state, trans->getSource());
    ASSERT_EQ(ClaimState::makeState(state1), trans->getTarget());
    shared_ptr<Var> actualVar = dynamic_pointer_cast<Var>(trans->getBoolExp());
    ASSERT_EQ(stateA, actualVar->getName());
}

/**
 * Tests the executable method with a generator that produces two transition.
 */
TEST_F(NeverClaimTest, executables_twoTransition) {
    // Setup
    shared_ptr<ClaimState> state = ClaimState::makeState(state1);

    // Exercise
    ClaimTransitionGeneratorPtr g = target->executables(*state);

    // Verify
    ASSERT_TRUE(g->hasNext());
    ClaimTransitionPtr trans1 = g->next();
    ASSERT_TRUE(g->hasNext());
    ClaimTransitionPtr trans2 = g->next();
    ASSERT_FALSE(g->hasNext());
    ASSERT_EQ(state, trans1->getSource());
    ASSERT_EQ(state, trans2->getSource());
}

} // namespace neverclaim
