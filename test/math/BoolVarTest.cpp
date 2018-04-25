/*
 * BoolVarTest.cpp
 *
 *  Created on 2013-07-17
 */

#include "gtest/gtest.h"

#include "math/BoolVar.hpp"

using std::string;

namespace math {

/**
 * Unit tests for the BoolVar class.
 *
 * @author bdawagne
 * @author mwi
 */
class BoolVarTest : public ::testing::Test {

protected:
    void SetUp() {
        // Nothing here
    }

    void TearDown() {
        // Nothing here
    }
};

TEST_F(BoolVarTest, constructor) {
    // Setup
    const string varName("BoolVarA");
    const int varId = 101;

    // Exercise
    BoolVar var = BoolVar::createBoolVar(varName, varId);

    // Verify
    ASSERT_EQ(varName, var.getName());
    ASSERT_EQ(varId, var.getId());
}

TEST_F(BoolVarTest, makeBoolVar_name) {
    // Setup
    const string varName("BoolVarB");
    BoolVar expectedVar = BoolVar::makeBoolVar(varName);

    // Exercise
    BoolVar actualVar = BoolVar::makeBoolVar(varName);

    // Verify
    ASSERT_EQ(expectedVar.getName(), actualVar.getName());
    ASSERT_EQ(expectedVar.getId(), actualVar.getId());
}

TEST_F(BoolVarTest, makeBoolVar_existingId) {
    // Setup
    const int varId = 102;
    BoolVar expectedVar = BoolVar::makeBoolVar(varId);

    // Exercise
    BoolVar actualVar = BoolVar::makeBoolVar(varId);

    // Verify
    ASSERT_EQ(expectedVar.getName(), actualVar.getName());
    ASSERT_EQ(expectedVar.getId(), actualVar.getId());
}

TEST_F(BoolVarTest, makeBoolVar_newId) {
    // Setup
    const int varId = 1001;

    // Exercise
    BoolVar boolVar = BoolVar::makeBoolVar(varId);

    // Verify
    ASSERT_EQ(varId, boolVar.getId());
    ASSERT_TRUE(boolVar.getName().length() > 0);
}

/**
 * Tests the makeBoolVar method with an id whose generated name already
 * exists.
 */
TEST_F(BoolVarTest, makeBoolVar_newSimilarId) {
    // Setup
    const int varId = 500;
    BoolVar::makeBoolVar("_500");

    // Exercise
    BoolVar boolVar = BoolVar::makeBoolVar(varId);

    // Verify
    ASSERT_EQ(varId, boolVar.getId());
    ASSERT_TRUE(boolVar.getName().length() > 0);
}

TEST_F(BoolVarTest, makeBoolVar) {
    // Setup
    const string varName("BoolVarD");
    const int varId = 106;
    BoolVar::createBoolVar(varName, varId);

    // Exercise
    BoolVar varById = BoolVar::makeBoolVar(varId);
    BoolVar varByName = BoolVar::makeBoolVar(varName);

    // Verify
    ASSERT_EQ(varId, varById.getId());
    ASSERT_EQ(varId, varByName.getId());
    ASSERT_EQ(varName, varById.getName());
    ASSERT_EQ(varName, varByName.getName());
}

} // namespace math
