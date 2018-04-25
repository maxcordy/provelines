/*
 * FstmState.cpp
 *
 * Created on 2013-07-26
 */

#include "gtest/gtest.h"

#include "core/fts/forwards.hpp"
#include "core/fts/fstm/FstmState.hpp"

using core::fts::StatePtr;
using std::string;

namespace core {
namespace fts {
namespace fstm {

/**
 * Unit tests for the FstmState class.
 *
 * @author mwi
 */
class FstmStateTest : public ::testing::Test {

protected:
    /**
     * Sets up the test fixture.
     */
    void SetUp() {}

    /**
     * Tears down the test fixture.
     */
    void TearDown() {}
};

TEST_F(FstmStateTest, makeState) {
   // Setup
    const string name("Arya Stark");

    // Exercise
    StatePtr state = FstmState::makeState(name);

    // Verify
    ASSERT_EQ(name, state->getName());
}

TEST_F(FstmStateTest, makeState_same) {
    // Setup
    const string name("Ghost");
    StatePtr expectedState = FstmState::makeState(name);

    // Exercise
    StatePtr actualState = FstmState::makeState(name);

    // Verify
    ASSERT_EQ(expectedState->getName(), actualState->getName());
    ASSERT_EQ(expectedState.get(), actualState.get());
    ASSERT_TRUE(expectedState->equals(*actualState));
}

TEST_F(FstmStateTest, hashCode_same) {
    // Setup
    const string name("Tyrion");
    StatePtr stateOne = FstmState::makeState(name);
    StatePtr stateTwo = FstmState::makeState(name);

    // Exercise && Verify
    ASSERT_EQ(stateOne->hashCode(), stateTwo->hashCode());
    ASSERT_TRUE(stateOne->equals(*stateTwo));
}

TEST_F(FstmStateTest, hashCode_different) {
    // Setup
    StatePtr stateOne = FstmState::makeState("State1");
    StatePtr stateTwo = FstmState::makeState("State2");

    // Exercise && Verify
    ASSERT_NE(stateOne->hashCode(), stateTwo->hashCode());
    ASSERT_FALSE(stateOne->equals(*stateTwo));
}

} // namespace fstm
} // namespace fts
} // namespace core
