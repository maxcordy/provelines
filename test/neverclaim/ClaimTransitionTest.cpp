/*
 * ClaimTransitionTest.cpp
 *
 * Created on 2013-08-02
 */

#include "gtest/gtest.h"

#include "math/BoolVar.hpp"
#include "neverclaim/ClaimState.hpp"
#include "neverclaim/ClaimTransition.hpp"
#include "util/parser/Var.hpp"

#include <tr1/shared_ptr.h>

using std::string;
using std::tr1::dynamic_pointer_cast;
using std::tr1::shared_ptr;
using util::parser::BoolExp;
using util::parser::Var;

namespace neverclaim {

/**
 * Unit tests for the ClaimTransition class.
 *
 * @author mwi
 */
class ClaimTransitionTest : public ::testing::Test {

protected:
    shared_ptr<ClaimTransition> target;
    shared_ptr<ClaimState> expectedSource;
    shared_ptr<ClaimState> expectedTarget;
    shared_ptr<BoolExp> expectedBoolExp;

    void SetUp() {
        const string sourceLabel("T0_init");
        const string targetLabel("accept_S0");

        expectedSource = ClaimState::makeState(sourceLabel);
        expectedTarget = ClaimState::makeState(targetLabel);
        expectedBoolExp = shared_ptr<BoolExp>(new Var("ClaimTransitionTest"));

        target = shared_ptr<ClaimTransition>(new ClaimTransition(sourceLabel,
                targetLabel, expectedBoolExp));
    }

    void TearDown() {
        // Nothing here
    }
};

TEST_F(ClaimTransitionTest, getSource) {
    // Exercise
    shared_ptr<ClaimState> actualSource = target->getSource();

    // Verify
    ASSERT_EQ(expectedSource, actualSource);
}

TEST_F(ClaimTransitionTest, getTarget) {
    // Exercise
    shared_ptr<ClaimState> actualTarget = target->getTarget();

    // Verify
    ASSERT_EQ(expectedTarget, actualTarget);
}

TEST_F(ClaimTransitionTest, getBoolExp) {
    // Setup
    shared_ptr<Var> expectedVar = dynamic_pointer_cast<Var>(expectedBoolExp);

    // Exercise
    shared_ptr<BoolExp> actualBoolExp = target->getBoolExp();

    // Verify
    shared_ptr<Var> actualVar = dynamic_pointer_cast<Var>(actualBoolExp);
    ASSERT_EQ(expectedVar->getName(), actualVar->getName());
}

} // namespace neverclaim
