/*
 * VarTableTest.cpp
 *
 *  Created on 2013-08-29
 */

#include "gtest/gtest.h"

#include "math/minimize/VarTable.hpp"

#include <sstream>

using std::string;

namespace math {
namespace minimize {

/**
 * Unit tests for the VarTable class.
 *
 * @author mwi
 */
class VarTableTest : public ::testing::Test {

protected:
    VarTablePtr target;

    void SetUp() {
        target = VarTablePtr(new VarTable());
    }

    void TearDown() {
        // Nothing here
    }
};

TEST_F(VarTableTest, add) {
    // Setup
    const string strName("MyVar");

    // Exercise
    const unsigned char charName = target->add(strName);

    // Verify
    ASSERT_TRUE(target->isIn(strName));
    ASSERT_TRUE(target->isIn(charName));
    ASSERT_EQ(strName, target->get(charName));
    ASSERT_EQ(charName, target->get(strName));
}

TEST_F(VarTableTest, isFull) {
    // Exercise & Verify
    for (int i = 0; i < 52; ++i) {
        ASSERT_FALSE(target->isFull());
        std::ostringstream oss;
        oss << "VarNb_" << i;
        target->add(oss.str());
    }
    ASSERT_TRUE(target->isFull());
}

} // namespace minimize
} // namespace math
