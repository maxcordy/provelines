/*
 * ltl2baTest.cpp
 *
 * Created on 2013-07-30
 */

#include "gtest/gtest.h"

#include "neverclaim/ltl2ba.hpp"

#include <fstream>

using std::ifstream;
using std::string;
using std::stringstream;

namespace neverclaim {

/**
 * Unit tests for the ltl2ba functions.
 *
 * @author mwi
 */
class ltl2baTest : public ::testing::Test {

protected:

    void SetUp() {
        // Nothing here
    }

    void TearDown() {
        // Nothing here
    }
};

TEST_F(ltl2baTest, ltl2ba) {
    // Setup
    const string ltlProperty("[]<>(p1 -> Xp2)");
    const string filePath("__test_ltl2ba.tmp");

    // Exercise
    bool result = ltl2ba(ltlProperty, filePath);

    // Verify
    ASSERT_TRUE(result);
    ifstream inputFileStream(filePath.c_str());
    ASSERT_TRUE(inputFileStream.is_open());
    string line;
    getline(inputFileStream, line);
    ASSERT_NE(string::npos, line.find(ltlProperty));
    ASSERT_EQ(string::npos, line.find("ltl2ba"));
}

/**
 * Tests the ltl2ba function with an invalid LTL property.
 */
TEST_F(ltl2baTest, ltl2ba_invalidLtl) {
    // Setup
    const string ltlProperty("[]<>(p1 -");
    const string filePath("__test_ltl2ba_invalidLtl.tmp");

    // Exercise
    bool result = ltl2ba(ltlProperty, filePath);

    // Verify
    ASSERT_FALSE(result);
    ifstream inputFileStream(filePath.c_str());
    ASSERT_TRUE(inputFileStream.is_open());
    string line;
    getline(inputFileStream, line);
    ASSERT_EQ(string::npos, line.find(ltlProperty));
    ASSERT_NE(string::npos, line.find("ltl2ba"));
}

} // namespace neverclaim
