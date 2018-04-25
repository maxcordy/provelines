/*
 * MinimizeParserTest.cpp
 *
 * Created on 2013-08-29
 */

#include "gtest/gtest.h"

#include "math/minimize/MinimizeParser.hpp"
#include "math/minimize/VarTable.hpp"
#include "core/checker/Exploration.hpp"

#include <sstream>
#include <stdexcept>

using std::logic_error;
using std::ostringstream;
using std::string;

namespace math {
namespace minimize {

/**
 * Unit tests for the MinimizeParser class.
 *
 * @author mwi
 */
class MinimizeParserTest : public ::testing::Test {

protected:
    string exp;
    string varA;
    string varB;
    string varC;
    string varD;

    VarTablePtr varTable;

    /**
     * Sets up the test fixture.
     */
    void SetUp() {
        varA = string("VarA");
        varB = string("VarB");
        varC = string("VarC");
        varD = string("VarD");

        varTable = VarTablePtr(new VarTable());
        varTable->add(varA);
        varTable->add(varB);
        varTable->add(varC);
        varTable->add(varD);
    }

    /**
     * Tears down the test fixture.
     */
    void TearDown() {}
};

TEST_F(MinimizeParserTest, variable) {
    // Setup
    exp += varTable->get(varA);
    MinimizeParser target(exp, varTable);

    // Exercise
    string actualExp = target.parse();

    // Verify
    ASSERT_EQ(varA, actualExp);
}

TEST_F(MinimizeParserTest, value_0) {
    // Setup
    const string expectedExp("false");
    const string value("0");
    MinimizeParser target(value, varTable);

    // Exercise
    string actualExp = target.parse();

    // Verify
    ASSERT_EQ(expectedExp, actualExp);
}

TEST_F(MinimizeParserTest, value_1) {
    // Setup
    const string expectedExp("true");
    const string value("1");
    MinimizeParser target(value, varTable);

    // Exercise
    string actualExp = target.parse();

    // Verify
    ASSERT_EQ(expectedExp, actualExp);
}

TEST_F(MinimizeParserTest, emptyMinimizeExp) {
    // Setup
    const string emptyMinimizeExp("");
    MinimizeParser target(emptyMinimizeExp, varTable);

    // Exercise & Verify
    ASSERT_THROW(target.parse(), logic_error);
}

TEST_F(MinimizeParserTest, parenthesisOne) {
    // Setup
    const string expectedExp("(" + varB + ")");
    exp += "(";
    exp += varTable->get(varB);
    exp += ")";
    MinimizeParser target(exp, varTable);

    // Exercise
    string actualExp = target.parse();

    // Verify
    ASSERT_EQ(expectedExp, actualExp);
}

TEST_F(MinimizeParserTest, parenthesisTwo) {
    // Setup
    const string expectedExp("((" + varC + "))");
    exp += "( (";
    exp += varTable->get(varC);
    exp += "))";
    MinimizeParser target(exp, varTable);

    // Exercise
    string actualExp = target.parse();

    // Verify
    ASSERT_EQ(expectedExp, actualExp);
}

TEST_F(MinimizeParserTest, negationOne) {
    // Setup
    const string expectedExp("!" + varD);
    exp += " ";
    exp += varTable->get(varD);
    exp += "'";
    MinimizeParser target(exp, varTable);

    // Exercise
    string actualExp = target.parse();

    // Verify
    ASSERT_EQ(expectedExp, actualExp);
}

TEST_F(MinimizeParserTest, negationTwo) {
    // Setup
    const string expectedExp("!(!" + varA + ")");
    exp += "(";
    exp += varTable->get(varA);
    exp += "')'";
    MinimizeParser target(exp, varTable);

    // Exercise
    string actualExp = target.parse();

    // Verify
    ASSERT_EQ(expectedExp, actualExp);
}

TEST_F(MinimizeParserTest, conjunctionOne) {
    // Setup
    const string expectedExp(varA + " && " + varB);
    exp += varTable->get(varA);
    exp += "";
    exp += varTable->get(varB);
    MinimizeParser target(exp, varTable);

    // Exercise
    string actualExp = target.parse();

    // Verify
    ASSERT_EQ(expectedExp, actualExp);
}

TEST_F(MinimizeParserTest, conjunctionTwo) {
    // Setup
    const string expectedExp(varA + " && " + varB);
    exp += varTable->get(varA);
    exp += " * ";
    exp += varTable->get(varB);
    MinimizeParser target(exp, varTable);

    // Exercise
    string actualExp = target.parse();

    // Verify
    ASSERT_EQ(expectedExp, actualExp);
}

TEST_F(MinimizeParserTest, conjunctionThree) {
    // Setup
    const string expectedExp(varA + " && " + varB + " && " + varC);
    exp += varTable->get(varA);
    exp += "*" ;
    exp += varTable->get(varB);
    exp += " ";
    exp += varTable->get(varC);
    MinimizeParser target(exp, varTable);

    // Exercise
    string actualExp = target.parse();

    // Verify
    ASSERT_EQ(expectedExp, actualExp);
}

TEST_F(MinimizeParserTest, disjunctionOne) {
    // Setup
    const string expectedExp(varD + " || " + varB);
    exp += varTable->get(varD);
    exp += "+";
    exp += varTable->get(varB);
    MinimizeParser target(exp, varTable);

    // Exercise
    string actualExp = target.parse();

    // Verify
    ASSERT_EQ(expectedExp, actualExp);
}

TEST_F(MinimizeParserTest, disjunctionTwo) {
    // Setup
    const string expectedExp(varD + " || " + varB + " || " + varA);
    exp += varTable->get(varD);
    exp += "+";
    exp += varTable->get(varB);
    exp += " + ";
    exp += varTable->get(varA);
    MinimizeParser target(exp, varTable);

    // Exercise
    string actualExp = target.parse();

    // Verify
    ASSERT_EQ(expectedExp, actualExp);
}

/**
 * Tests the following complexe expression:
 * (varA || varB && varC || !(varD || !varC) && !(varA && varB))
 */
TEST_F(MinimizeParserTest, complexOne) {
    // Setup
    const string expectedExp("(" + varA + " || " + varB + " && " + varC
            + " || !(" + varD + " || !" + varC + ") && !(" + varA + " && "
            + varB + "))");
    ostringstream oss;
    oss << "("
        << varTable->get(varA)
        << " + "
        << varTable->get(varB)
        << " * "
        << varTable->get(varC)
        << " + ("
        << varTable->get(varD)
        << "+"
        << varTable->get(varC)
        << "') '*("
        << varTable->get(varA)
        << varTable->get(varB)
        << ")')";
    MinimizeParser target(oss.str(), varTable);

    // Exercise
    string actualExp = target.parse();

    // Verify
    ASSERT_EQ(expectedExp, actualExp);
}

TEST_F(MinimizeParserTest, badExpOne) {
    // Setup
    MinimizeParser target("3", varTable);

    // Exercise & Verify
    ASSERT_THROW(target.parse(), logic_error);
}

TEST_F(MinimizeParserTest, badExpTwo) {
    // Setup
    MinimizeParser target("(+)", varTable);

    // Exercise & Verify
    ASSERT_THROW(target.parse(), logic_error);
}

TEST_F(MinimizeParserTest, badExpThree) {
    // Setup
    MinimizeParser target("(A", varTable);

    // Exercise & Verify
    ASSERT_THROW(target.parse(), logic_error);
}

TEST_F(MinimizeParserTest, badNegation) {
    // Setup
    MinimizeParser target("'A", varTable);

    // Exercise & Verify
    ASSERT_THROW(target.parse(), logic_error);
}

TEST_F(MinimizeParserTest, badConjunction) {
    // Setup
    MinimizeParser target("A3", varTable);

    // Exercise & Verify
    ASSERT_THROW(target.parse(), logic_error);
}

TEST_F(MinimizeParserTest, badDisjunction) {
    // Setup
    MinimizeParser target("A + *", varTable);

    // Exercise & Verify
    ASSERT_THROW(target.parse(), logic_error);
}

} // namespace parser
} // namespace util
