/*
 * stringsTest.cpp
 *
 * Created on 2013-08-07
 */

#include "gtest/gtest.h"

#include "util/strings.hpp"

using std::endl;
using std::string;
using std::stringstream;

namespace util {

/**
 * Unit tests for the strings functions.
 *
 * @author mwi
 */
class stringsTest : public ::testing::Test {

protected:

    void SetUp() {
        // Nothing here
    }

    void TearDown() {
        // Nothing here
    }
};

TEST_F(stringsTest, toLowercase) {
    // Setup
    const string expectedStr("iM_IN_LowerCase");
    string actualStr("IM_IN_LowerCase");

    // Exercise
    toLowercaseFirst(actualStr);

    // Verify
    ASSERT_EQ(expectedStr, actualStr);
}

TEST_F(stringsTest, toString) {
    // Setup
    string expectedStr("Dance, dance, wherever you may be\n");
    expectedStr += "I am the lord of the dance, said he";
    stringstream stream;
    stream << "Dance, dance, wherever you may be" << endl
           << "I am the lord of the dance, said he";

    // Exercise
    string actualStr = toString(stream);

    // Verify
    ASSERT_EQ(expectedStr, actualStr);
}

} // namespace util
