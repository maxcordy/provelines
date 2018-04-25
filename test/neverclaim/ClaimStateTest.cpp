/*
 * ClaimStateTest.cpp
 *
 * Created on 2013-08-02
 */

#include "gtest/gtest.h"

#include "neverclaim/ClaimState.hpp"

#include <tr1/memory>

using std::string;
using std::tr1::shared_ptr;

namespace neverclaim {

/**
 * Unit tests for the ClaimState class.
 *
 * @author mwi
 */
class ClaimStateTest : public ::testing::Test {

protected:

    void SetUp() {
        // Nothing here
    }

    void TearDown() {
        // Nothing here
    }
};

/**
 * Tests the makeState method with a non accepting state.
 */
TEST_F(ClaimStateTest, makeState) {
    // Setup
    const string label("T0_init");

    // Exercise
    shared_ptr<ClaimState> state = ClaimState::makeState(label);

    // Verify
    ASSERT_EQ(label, state->getLabel());
    ASSERT_FALSE(state->isAccepting());
    ASSERT_FALSE(state->isFinal());
}

/**
 * Tests the makeState method with an accepting but non final state.
 */
TEST_F(ClaimStateTest, makeState_accepting) {
    // Setup
    const string label("accept_S2");

    // Exercise
    shared_ptr<ClaimState> state = ClaimState::makeState(label);

    // Verify
    ASSERT_EQ(label, state->getLabel());
    ASSERT_TRUE(state->isAccepting());
    ASSERT_FALSE(state->isFinal());
}

/**
 * Tests the makeState method with an accepting but non final state.
 */
TEST_F(ClaimStateTest, makeState_final) {
    // Setup
    const string label("accept_all");

    // Exercise
    shared_ptr<ClaimState> state = ClaimState::makeState(label);

    // Verify
    ASSERT_EQ(label, state->getLabel());
    ASSERT_TRUE(state->isAccepting());
    ASSERT_TRUE(state->isFinal());
}

/**
 * Tests the makeState method returns the same state for the same label.
 */
TEST_F(ClaimStateTest, makeState_sameLabel) {
    // Setup
    const string label("T0_S2");
    shared_ptr<ClaimState> expectedState = ClaimState::makeState(label);

    // Exercise
    shared_ptr<ClaimState> actualState = ClaimState::makeState(label);

    // Verify
    ASSERT_EQ(expectedState, actualState);
}

TEST_F(ClaimStateTest, hashCode_same) {
    // Setup
    const string label("accept_T1");
    shared_ptr<ClaimState> stateOne = ClaimState::makeState(label);
    shared_ptr<ClaimState> stateTwo = ClaimState::makeState(label);

    // Exercise & Verify
    ASSERT_EQ(stateOne->hashCode(), stateTwo->hashCode());
    ASSERT_TRUE(stateOne->equals(*stateTwo));
}

TEST_F(ClaimStateTest, hashCode_different) {
    // Setup
    shared_ptr<ClaimState> stateOne = ClaimState::makeState("accept_all");
    shared_ptr<ClaimState> stateTwo = ClaimState::makeState("accept_T0");

    // Exercise & Verify
    ASSERT_NE(stateOne->hashCode(), stateTwo->hashCode());
    ASSERT_FALSE(stateOne->equals(*stateTwo));
}

} // namespace neverclaim
