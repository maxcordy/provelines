/*
 * BoolFctCUDDTest.cpp
 *
 *  Created on 2013-07-17
 */

#include "gtest/gtest.h"

#include "math/BoolFctCUDD.hpp"
#include "math/BoolFctCUDDFactory.hpp"
#include "math/minimize/Minimizer.hpp"

using math::minimize::Minimizer;
using std::string;
using std::tr1::shared_ptr;

namespace math {

/**
 * Unit tests for the BoolFctCUDD class.
 *
 * @author bdawagne
 */
class BoolFctCUDDTest : public ::testing::Test {

protected:
    string varA;
    string varB;

    /** true */
    shared_ptr<BoolFct> t;
    /** false */
    shared_ptr<BoolFct> f;
    /** a */
    shared_ptr<BoolFct> a;
    /** b */
    shared_ptr<BoolFct> b;
    /** a && !a */
    shared_ptr<BoolFct> fct1;
    /** a || !a */
    shared_ptr<BoolFct> fct2;
    /** a && b */
    shared_ptr<BoolFct> fct3;
    /** a || b */
    shared_ptr<BoolFct> fct4;
    /** !(!a && !b) */
    shared_ptr<BoolFct> fct5;
    /** !(!a || !b) */
    shared_ptr<BoolFct> fct6;

    void SetUp() {
        varA = "BoolFctCUDDa";
        varB = "BoolFctCUDDb";

        BoolFctCUDDFactory factory;
        shared_ptr<BoolFct> tmp;
        // initialize t
        t.reset(factory.getTrue());
        // initialize f
        f.reset(factory.getFalse());
        // initialize a
        a.reset(factory.create(BoolVar::makeBoolVar(varA)));
        // initialize b
        b.reset(factory.create(varB));
        // initialize fct1
        fct1 = shared_ptr<BoolFct>(a->clone());
        tmp = shared_ptr<BoolFct>(a->clone());
        fct1->negation();
        fct1->conjunction(*tmp);
        // initialize fct2
        fct2 = shared_ptr<BoolFct>(a->clone());
        tmp = shared_ptr<BoolFct>(a->clone());
        fct2->negation();
        fct2->disjunction(*tmp);
        // initialize fct3
        fct3 = shared_ptr<BoolFct>(a->clone());
        tmp = shared_ptr<BoolFct>(b->clone());
        fct3->conjunction(*tmp);
        // initialize fct4
        fct4 = shared_ptr<BoolFct>(a->clone());
        tmp = shared_ptr<BoolFct>(b->clone());
        fct4->disjunction(*tmp);
        // initialize fct5
        fct5 = shared_ptr<BoolFct>(a->clone());
        tmp = shared_ptr<BoolFct>(b->clone());
        fct5->negation();
        tmp->negation();
        fct5->conjunction(*tmp);
        fct5->negation();
        // initialize fct6
        fct6 = shared_ptr<BoolFct>(a->clone());
        tmp = shared_ptr<BoolFct>(b->clone());
        fct6->negation();
        tmp->negation();
        fct6->disjunction(*tmp);
        fct6->negation();
    }

    void TearDown() {}
};

TEST_F(BoolFctCUDDTest, isSatisfiable) {
    // Verify
    EXPECT_FALSE(f->isSatisfiable());
    EXPECT_TRUE(t->isSatisfiable());
    EXPECT_FALSE(fct1->isSatisfiable());
    EXPECT_TRUE(fct2->isSatisfiable());
    EXPECT_TRUE(fct3->isSatisfiable());
    EXPECT_TRUE(fct4->isSatisfiable());
}

TEST_F(BoolFctCUDDTest, isTautology) {
    // Verify
    EXPECT_FALSE(f->isTautology());
    EXPECT_TRUE(t->isTautology());
    EXPECT_FALSE(fct1->isTautology());
    EXPECT_TRUE(fct2->isTautology());
    EXPECT_FALSE(fct3->isTautology());
    EXPECT_FALSE(fct4->isTautology());
}

TEST_F(BoolFctCUDDTest, isEquivalent) {
    // Verify
    EXPECT_TRUE(fct3->isEquivalent(*fct6));
    EXPECT_TRUE(fct4->isEquivalent(*fct5));
    EXPECT_FALSE(fct3->isEquivalent(*fct4));
}

TEST_F(BoolFctCUDDTest, isEquivalent_aliasing) {
    // Verify
    EXPECT_TRUE(fct3->isEquivalent(*fct3));
}

TEST_F(BoolFctCUDDTest, implies) {
    // Verify
    EXPECT_FALSE(a->implies(*fct3));
    EXPECT_TRUE(a->implies(*fct4));
    EXPECT_TRUE(fct3->implies(*b));
    EXPECT_FALSE(fct4->implies(*b));
}

TEST_F(BoolFctCUDDTest, implies_aliasing) {
    // Verify
    EXPECT_TRUE(a->implies(*a));
}

/**
 * Tests the conjunction method returns this.
 */
TEST_F(BoolFctCUDDTest, conjunction_return) {
    // Exercise
    BoolFct & retValue = a->conjunction(*b);

    // Verify
    ASSERT_EQ(a.get(), &retValue);
}

/**
 * Tests the conjunction method returns this.
 */
TEST_F(BoolFctCUDDTest, disjunction_return) {
    // Exercise
    BoolFct & retValue = a->disjunction(*b);

    // Verify
    ASSERT_EQ(a.get(), &retValue);
}

/**
 * Tests the conjunction method returns this.
 */
TEST_F(BoolFctCUDDTest, negation_return) {
    // Exercise
    BoolFct & retValue = a->negation();

    // Verify
    ASSERT_EQ(a.get(), &retValue);
}

TEST_F(BoolFctCUDDTest, toString_one) {
    // Setup
    const string expectedExp("!" + varA + " || " + varB);
    // fct = a && b || !a
    shared_ptr<BoolFct> fct = shared_ptr<BoolFct>(a->clone());
    fct->conjunction(*b);
    shared_ptr<BoolFct> notA = shared_ptr<BoolFct>(a->clone());
    notA->negation();
    fct->disjunction(*notA);

    // Exercise
    const string str(fct->toString());

    // Verify
    Minimizer minimizer;
    const string actualExp(minimizer.minimize(str));
    ASSERT_EQ(expectedExp, actualExp);
}

TEST_F(BoolFctCUDDTest, toString_two) {
    // FIXME: This test should go through the case '0' lines of code from
    //        BoolFctCUDD::toString
    // Setup
    const string expectedExp(varA + " && " + varB);
    // fct = !(!(a && b) || !a)
    shared_ptr<BoolFct> fct = shared_ptr<BoolFct>(a->clone());
    fct->conjunction(*b);
    fct->negation();
    shared_ptr<BoolFct> notA = shared_ptr<BoolFct>(a->clone());
    notA->negation();
    fct->disjunction(*notA);
    fct->negation();

    // Exercise
    const string str(fct->toString());

    // Verify
    Minimizer minimizer;
    const string actualExp(minimizer.minimize(str));
    ASSERT_EQ(expectedExp, actualExp);
}

/**
 * Tests the toString method for the true Boolean function.
 */
TEST_F(BoolFctCUDDTest, toString_true) {
    // Setup
    const string expectedExp("true");

    // Exercise
    const string actualExp(t->toString());

    // Verify
    ASSERT_EQ(expectedExp, actualExp);
}

/**
 * Tests the toString method for the false Boolean function.
 */
TEST_F(BoolFctCUDDTest, toString_false) {
    // Setup
    const string expectedExp("false");

    // Exercise
    const string actualExp(f->toString());

    // Verify
    ASSERT_EQ(expectedExp, actualExp);
}

} // namespace math
