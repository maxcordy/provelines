/*
 * ClaimPropertyStateTest.cpp
 *
 * Created on 2013-08-14
 */

#include "gtest/gtest.h"

#include "neverclaim/ClaimState.hpp"
#include "core/fts/fstm/ClaimPropertyState.hpp"

using neverclaim::ClaimState;
using neverclaim::ClaimStatePtr;
using std::string;

namespace core {
namespace fts {
namespace fstm {

/**
 * Unit tests for the ClaimPropertyState class.
 *
 * @author mwi
 */
class ClaimPropertyStateTest : public ::testing::Test {

protected:
    ClaimStatePtr claimStateOne;
    ClaimStatePtr claimStateTwo;

    string labelOne;
    string labelTwo;

    void SetUp() {
        labelOne = string("T0_init");
        labelTwo = string("accept_S2");

        claimStateOne = ClaimState::makeState(labelOne);
        claimStateTwo = ClaimState::makeState(labelTwo);
    }

    void TearDown() {
        // Nothing here
    }
};

TEST_F(ClaimPropertyStateTest, getLabel) {
    // Setup
    PropertyStatePtr target =
            ClaimPropertyState::makePropertyState(claimStateOne);

    // Exercise & Verify
    ASSERT_EQ(labelOne, target->getLabel());
}

TEST_F(ClaimPropertyStateTest, isAccepting) {
    // Setup
    PropertyStatePtr target =
            ClaimPropertyState::makePropertyState(claimStateTwo);

    // Exercise & Verify
    ASSERT_TRUE(target->isAccepting());
}

TEST_F(ClaimPropertyStateTest, isFinal) {
    // Setup
    PropertyStatePtr target =
            ClaimPropertyState::makePropertyState(claimStateTwo);

    // Exercise & Verify
    ASSERT_FALSE(target->isFinal());
}

TEST_F(ClaimPropertyStateTest, hashCode) {
    // Setup
    PropertyStatePtr target =
            ClaimPropertyState::makePropertyState(claimStateOne);

    // Exercise & Verify
    ASSERT_EQ(claimStateOne->hashCode(), target->hashCode());
}

TEST_F(ClaimPropertyStateTest, equals_true) {
    // Setup
    PropertyStatePtr target =
            ClaimPropertyState::makePropertyState(claimStateOne);
    PropertyStatePtr same =
            ClaimPropertyState::makePropertyState(claimStateOne);

    // Exercise & Verify
    ASSERT_TRUE(target->equals(*same));
}

TEST_F(ClaimPropertyStateTest, equals_false) {
    // Setup
    PropertyStatePtr target =
            ClaimPropertyState::makePropertyState(claimStateOne);
    PropertyStatePtr different =
            ClaimPropertyState::makePropertyState(claimStateTwo);

    // Exercise & Verify
    ASSERT_FALSE(target->equals(*different));
}

/**
 * Tests the makePropertyState method returns the same state for the same claim
 * state.
 */
TEST_F(ClaimPropertyStateTest, makePropertyState_sameClaimState) {
    // Setup
    PropertyStatePtr expected =
            ClaimPropertyState::makePropertyState(claimStateOne);

    // Exercise
    PropertyStatePtr actual =
            ClaimPropertyState::makePropertyState(claimStateOne);

    // Verify
    ASSERT_EQ(expected, actual);
}

/**
 * Tests the makePropertyState method returns different states for different
 * claim states.
 */
TEST_F(ClaimPropertyStateTest, makePropertyState_differentClaimState) {
    // Setup
    PropertyStatePtr expected =
            ClaimPropertyState::makePropertyState(claimStateTwo);

    // Exercise
    PropertyStatePtr actual =
            ClaimPropertyState::makePropertyState(claimStateOne);

    // Verify
    ASSERT_NE(expected, actual);
}

} // namespace fstm
} // namespace fts
} // namespace core
