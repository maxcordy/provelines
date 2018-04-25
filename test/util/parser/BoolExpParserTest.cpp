/*
 * BoolExpParserTest.cpp
 *
 * Created on 2013-07-24
 */

#include "gtest/gtest.h"

#include "core/features/BoolFeatureExp.hpp"
#include "core/features/BoolFeatureExpFactory.hpp"
#include "core/features/FeatureExp.hpp"
#include "core/features/FeatureExpGenVisitor.hpp"
#include "math/BoolFctCUDDFactory.hpp"
#include "util/parser/BoolExp.hpp"
#include "util/parser/BoolExpParser.hpp"

#include <stdexcept>

using namespace core::features;
using math::BoolFctCUDDFactory;
using math::BoolFctFactory;
using std::logic_error;
using std::string;
using std::tr1::shared_ptr;

namespace util {
namespace parser {

/**
 * Unit tests for the BoolExpParser class.
 *
 * @author mwi
 */
class BoolExpParserTest : public ::testing::Test {

protected:
    shared_ptr<BoolFeatureExpFactory> factory;

    /**
     * Sets up the test fixture.
     */
    void SetUp() {
        shared_ptr<BoolFctFactory> boolFctFactory(new BoolFctCUDDFactory());
        factory = shared_ptr<BoolFeatureExpFactory>(
                new BoolFeatureExpFactory(boolFctFactory));
    }

    /**
     * Tears down the test fixture.
     */
    void TearDown() {}

    /**
     * @return the expected feature expression for the test conjunctionTwo.
     */
    shared_ptr<FeatureExp> createExpectedExp_conjunctionTwo() {
        shared_ptr<BoolFeatureExp> expectedExp(factory->create("A"));
        expectedExp->conjunction(*factory->create("B"));
        expectedExp->conjunction(*factory->create("C"));
        shared_ptr<BoolFeatureExp> rhsOne(factory->create("AA"));
        rhsOne->conjunction(*factory->create("BB"));
        shared_ptr<BoolFeatureExp> rhsTwo(factory->create("A"));
        rhsTwo->conjunction(*factory->create("B"));
        rhsTwo->conjunction(*factory->create("C"));
        rhsOne->conjunction(*rhsTwo);
        expectedExp->conjunction(*rhsOne);
        return expectedExp;
    }

    /**
     * @return the expected feature expression for the test disjunctionTwo.
     */
    shared_ptr<FeatureExp> createExpectedExp_disjunctionTwo() {
        shared_ptr<BoolFeatureExp> expectedExp(factory->create("A"));
        expectedExp->disjunction(*factory->create("A"));
        expectedExp->disjunction(*factory->create("B"));
        shared_ptr<BoolFeatureExp> rhs(factory->create("Z"));
        rhs->disjunction(*factory->create("BB"));
        expectedExp->disjunction(*rhs);
        rhs.reset(factory->create("_Z"));
        rhs->disjunction(*factory->create("C"));
        expectedExp->disjunction(*rhs);
        return expectedExp;
    }

    /**
     * @return the expected feature expression for the test complexeOne.
     */
    shared_ptr<FeatureExp> createExpectedExp_complexeOne() {
        shared_ptr<BoolFeatureExp> expectedExp(factory->create("A"));

        shared_ptr<BoolFeatureExp> rhsOne(factory->create("B"));
        rhsOne->conjunction(*factory->create("C"));

        shared_ptr<BoolFeatureExp> rhsTwo(factory->create("_ZEee3"));
        shared_ptr<BoolFeatureExp> rhsThree(factory->create("_z3"));
        rhsThree->negation();
        rhsTwo->disjunction(*rhsThree);
        rhsTwo->negation();

        rhsThree.reset(factory->create("A"));
        rhsThree->conjunction(*factory->create("B"));
        rhsThree->negation();

        rhsTwo->conjunction(*rhsThree);

        expectedExp->disjunction(*rhsOne);
        expectedExp->disjunction(*rhsTwo);
        return expectedExp;
    }

    /**
     * @return the Boolean expression 'exp' converted to a feature expression.
     */
    FeatureExpPtr toFeatureExp(const BoolExpPtr & exp) const {
        FeatureExpGenVisitorPtr visitor(new FeatureExpGenVisitor(*factory));
        exp->accept(*visitor);
        return visitor->getFeatureExp();
    }
};

TEST_F(BoolExpParserTest, variable) {
    // Setup
    const string varName("trA_3b");
    BoolExpParser target(varName);

    // Exercise
    BoolExpPtr actualExp = target.parse();

    // Verify
    shared_ptr<BoolFeatureExp> expectedExp(factory->create(varName));
    ASSERT_TRUE(expectedExp->isEquivalent(*toFeatureExp(actualExp)));
}

/**
 * Tests the parser with the true Boolean expression represented as 'true'.
 */
TEST_F(BoolExpParserTest, value_true) {
    // Setup
    const string valueName("true");
    BoolExpParser target(valueName);

    // Exercise
    BoolExpPtr actualExp = target.parse();

    // Verify
    ASSERT_TRUE(toFeatureExp(actualExp)->isEquivalent(*factory->getTrue()));
}

/**
 * Tests the parser with the true Boolean expression represented as '1'.
 */
TEST_F(BoolExpParserTest, value_1) {
    // Setup
    const string valueName("1");
    BoolExpParser target(valueName);

    // Exercise
    BoolExpPtr actualExp = target.parse();

    // Verify
    ASSERT_TRUE(toFeatureExp(actualExp)->isEquivalent(*factory->getTrue()));
}

/**
 * Tests the parser with the false Boolean expression represented as 'false'.
 */
TEST_F(BoolExpParserTest, value_false) {
    // Setup
    const string valueName("false");
    BoolExpParser target(valueName);

    // Exercise
    BoolExpPtr actualExp = target.parse();

    // Verify
    ASSERT_TRUE(toFeatureExp(actualExp)->isEquivalent(*factory->getFalse()));
}

/**
 * Tests the parser with the false Boolean expression represented as '0'.
 */
TEST_F(BoolExpParserTest, value_0) {
    // Setup
    const string valueName("0");
    BoolExpParser target(valueName);

    // Exercise
    BoolExpPtr actualExp = target.parse();

    // Verify
    ASSERT_TRUE(toFeatureExp(actualExp)->isEquivalent(*factory->getFalse()));
}

/**
 * Tests the parser with an empty Boolean expression.
 */
TEST_F(BoolExpParserTest, emptyBoolExp) {
    // Setup
    const string emptyBoolExp("");
    BoolExpParser target(emptyBoolExp);

    // Exercise
    BoolExpPtr actualExp = target.parse();

    // Verify
    FeatureExpPtr expectedExp(factory->getTrue());
    ASSERT_TRUE(expectedExp->isEquivalent(*toFeatureExp(actualExp)));
}

TEST_F(BoolExpParserTest, parenthesisOne) {
    // Setup
    BoolExpParser target("(_3Bz )");

    // Exercise
    BoolExpPtr actualExp = target.parse();

    // Verify
    shared_ptr<BoolFeatureExp> expectedExp(factory->create("_3Bz"));
    ASSERT_TRUE(expectedExp->isEquivalent(*toFeatureExp(actualExp)));
}

TEST_F(BoolExpParserTest, parenthesisTwo) {
    // Setup
    BoolExpParser target("((AA))");

    // Exercise
    BoolExpPtr actualExp = target.parse();

    // Verify
    shared_ptr<BoolFeatureExp> expectedExp(factory->create("AA"));
    ASSERT_TRUE(expectedExp->isEquivalent(*toFeatureExp(actualExp)));
}

TEST_F(BoolExpParserTest, negationOne) {
    // Setup
    BoolExpParser target("!( AaaaA)");

    // Exercise
    BoolExpPtr actualExp = target.parse();

    // Verify
    shared_ptr<BoolFeatureExp> expectedExp(factory->create("AaaaA"));
    expectedExp->negation();
    ASSERT_TRUE(expectedExp->isEquivalent(*toFeatureExp(actualExp)));
}

TEST_F(BoolExpParserTest, negationTwo) {
    // Setup
    BoolExpParser target("!(!( (BB_BB) ))");

    // Exercise
    BoolExpPtr actualExp = target.parse();

    // Verify
    shared_ptr<BoolFeatureExp> expectedExp(factory->create("BB_BB"));
    expectedExp->negation();
    expectedExp->negation();
    ASSERT_TRUE(expectedExp->isEquivalent(*toFeatureExp(actualExp)));
}

TEST_F(BoolExpParserTest, conjunctionOne) {
    // Setup
    BoolExpParser target("A && B");

    // Exercise
    BoolExpPtr actualExp = target.parse();

    // Verify
    shared_ptr<BoolFeatureExp> expectedExp(factory->create("A"));
    shared_ptr<BoolFeatureExp> rhs(factory->create("B"));
    expectedExp->conjunction(*rhs);
    ASSERT_TRUE(toFeatureExp(actualExp)->isEquivalent(*expectedExp));
}

TEST_F(BoolExpParserTest, conjunctionTwo) {
    // Setup
    BoolExpParser target("A && B && C && ((AA&&BB) && (A && B && C))");

    // Exercise
    BoolExpPtr actualExp = target.parse();

    // Verify
    shared_ptr<FeatureExp> expectedExp = createExpectedExp_conjunctionTwo();
    ASSERT_TRUE(toFeatureExp(actualExp)->isEquivalent(*expectedExp));
}

TEST_F(BoolExpParserTest, disjunctionOne) {
    // Setup
    BoolExpParser target("_Z || BOOL_A");

    // Exercise
    BoolExpPtr actualExp = target.parse();

    // Verify
    shared_ptr<BoolFeatureExp> expectedExp(factory->create("_Z"));
    shared_ptr<BoolFeatureExp> rhs(factory->create("BOOL_A"));
    expectedExp->disjunction(*rhs);
    ASSERT_TRUE(toFeatureExp(actualExp)->isEquivalent(*expectedExp));
}

TEST_F(BoolExpParserTest, disjunctionTwo) {
    // Setup
    BoolExpParser target("A || A || B || ((Z||BB)|| (_Z || C)  )");

    // Exercise
    BoolExpPtr actualExp = target.parse();

    // Verify
    shared_ptr<FeatureExp> expectedExp = createExpectedExp_disjunctionTwo();
    ASSERT_TRUE(expectedExp->isEquivalent(*toFeatureExp(actualExp)));
}

TEST_F(BoolExpParserTest, complexOne) {
    // Setup
    BoolExpParser target("(A || B && C || !(_ZEee3 || !(_z3)) && !(A&&B))");

    // Exercise
    BoolExpPtr actualExp = target.parse();

    // Verify
    shared_ptr<FeatureExp> expectedExp = createExpectedExp_complexeOne();
    ASSERT_TRUE(toFeatureExp(actualExp)->isEquivalent(*expectedExp));
}

TEST_F(BoolExpParserTest, badAtomicOne) {
    // Setup
    BoolExpParser target("3a");

    // Exercise & Verify
    ASSERT_THROW(target.parse(), logic_error);
}

TEST_F(BoolExpParserTest, badAtomicTwo) {
    // Setup
    BoolExpParser target("(+)");

    // Exercise & Verify
    ASSERT_THROW(target.parse(), logic_error);
}

TEST_F(BoolExpParserTest, badAtomicThree) {
    // Setup
    BoolExpParser target("(A");

    // Exercise & Verify
    ASSERT_THROW(target.parse(), logic_error);
}

TEST_F(BoolExpParserTest, badNegationOne) {
    // Setup
    BoolExpParser target("!A)");

    // Exercise & Verify
    ASSERT_THROW(target.parse(), logic_error);
}

TEST_F(BoolExpParserTest, badNegationTwo) {
    // Setup
    BoolExpParser target("!(/)");

    // Exercise & Verify
    ASSERT_THROW(target.parse(), logic_error);
}

TEST_F(BoolExpParserTest, badNegationThree) {
    // Setup
    BoolExpParser target("!(A3z");

    // Exercise & Verify
    ASSERT_THROW(target.parse(), logic_error);
}

TEST_F(BoolExpParserTest, badConjunction) {
    // Setup
    BoolExpParser target("A && 3");

    // Exercise & Verify
    ASSERT_THROW(target.parse(), logic_error);
}

TEST_F(BoolExpParserTest, badDisjunction) {
    // Setup
    BoolExpParser target("A || *");

    // Exercise & Verify
    ASSERT_THROW(target.parse(), logic_error);
}

/**
 * Tests the parser with a tabulation within a feature expression.
 */
TEST_F(BoolExpParserTest, tabulation) {
    // Setup
    BoolExpParser target("A\t&&B");

    // Exercise
    BoolExpPtr actualExp = target.parse();

    // Verify
    shared_ptr<BoolFeatureExp> expectedExp(factory->create("A"));
    expectedExp->conjunction(*factory->create("B"));
    ASSERT_TRUE(expectedExp->isEquivalent(*toFeatureExp(actualExp)));
}

/**
 * Tests the parser with a carriage return within a feature expression.
 */
TEST_F(BoolExpParserTest, carriageReturn) {
    // Setup
    BoolExpParser target("\r!(B)");

    // Exercise
    BoolExpPtr actualExp = target.parse();

    // Verify
    shared_ptr<BoolFeatureExp> expectedExp(factory->create("B"));
    expectedExp->negation();
    ASSERT_TRUE(expectedExp->isEquivalent(*toFeatureExp(actualExp)));
}

/**
 * Tests the parser with a newline within a feature expression.
 */
TEST_F(BoolExpParserTest, newline) {
    // Setup
    BoolExpParser target("A\n  ||B");

    // Exercise
    BoolExpPtr actualExp = target.parse();

    // Verify
    shared_ptr<BoolFeatureExp> expectedExp(factory->create("A"));
    expectedExp->disjunction(*factory->create("B"));
    ASSERT_TRUE(expectedExp->isEquivalent(*toFeatureExp(actualExp)));
}

/**
 * Tests the parser with trailing white spaces.
 */
TEST_F(BoolExpParserTest, trailingWhiteSpaces) {
    // TODO

    // Setup
    BoolExpParser target("A && BB\n");

    // Exercise
    BoolExpPtr actualExp = target.parse();

    // Verify
    shared_ptr<BoolFeatureExp> expectedExp(factory->create("A"));
    expectedExp->conjunction(*factory->create("BB"));;
    ASSERT_TRUE(expectedExp->isEquivalent(*toFeatureExp(actualExp)));
}

} // namespace parser
} // namespace util
