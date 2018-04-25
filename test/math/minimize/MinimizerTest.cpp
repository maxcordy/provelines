/*
 * MinimizerTest.cpp
 *
 *  Created on 2013-08-29
 */

#include "gtest/gtest.h"

#include "math/minimize/Minimizer.hpp"

using std::string;

namespace math {
namespace minimize {

/**
 * Unit tests for the Minimizer class.
 *
 * @author mwi
 */
class MinimizerTest : public ::testing::Test {

protected:
    Minimizer target;

    void SetUp() {

    }

    void TearDown() {
        // Nothing here
    }
};

TEST_F(MinimizerTest, minimize) {
    // Setup
    const string exp("!(VarA) && !(VarB) && VarC || !(VarA) && VarB || VarA");
    const string expectedExp("VarC || VarB || VarA");

    // Exercise
    string actualExp = target.minimize(exp);

    // Verify
    ASSERT_EQ(expectedExp, actualExp);
}

} // namespace minimize
} // namespace math
