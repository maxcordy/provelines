/*
 * BoolFeatureExpTest.cpp
 *
 * Created on 2013-07-26
 */

#include "gtest/gtest.h"

#include "config/Context.hpp"

#include "core/features/BoolFeatureExp.hpp"
#include "core/features/BoolFeatureExpFactory.hpp"

using config::Context;

using math::BoolFctFactory;

using std::string;
using std::tr1::shared_ptr;

namespace core {
namespace features {

/**
 * Unit tests for the BoolFeatureExp class.
 *
 * @author mwi
 */
class BoolFeatureExpTest : public ::testing::Test {

protected:
    /** true */
    shared_ptr<FeatureExp> t;
    /** false */
    shared_ptr<FeatureExp> f;
    /** a */
    shared_ptr<FeatureExp> a;
    /** b */
    shared_ptr<FeatureExp> b;
    /** a && !a */
    shared_ptr<FeatureExp> fct1;
    /** a || !a */
    shared_ptr<FeatureExp> fct2;
    /** a && b */
    shared_ptr<FeatureExp> fct3;
    /** a || b */
    shared_ptr<FeatureExp> fct4;
    /** !(!a && !b) */
    shared_ptr<FeatureExp> fct5;
    /** !(!a || !b) */
    shared_ptr<FeatureExp> fct6;

    /**
     * Sets up the test fixture.
     */
    void SetUp() {
        Context ctx;
        shared_ptr<BoolFeatureExpFactory> factory =
                ctx.getBoolFeatureExpFactory();
        shared_ptr<FeatureExp> tmp;
        // initialize t
        t.reset(factory->getTrue());
        // initialize f
        f.reset(factory->getFalse());
        // initialize a
        a.reset(factory->create("FeatureA"));
        // initialize b
        b.reset(factory->create("FeatureB"));
        // initialize fct1
        fct1 = shared_ptr<FeatureExp>(a->clone());
        tmp = shared_ptr<FeatureExp>(a->clone());
        fct1->negation();
        fct1->conjunction(*tmp);
        // initialize fct2
        fct2 = shared_ptr<FeatureExp>(a->clone());
        tmp = shared_ptr<FeatureExp>(a->clone());
        fct2->negation();
        fct2->disjunction(*tmp);
        // initialize fct3
        fct3 = shared_ptr<FeatureExp>(a->clone());
        tmp = shared_ptr<FeatureExp>(b->clone());
        fct3->conjunction(*tmp);
        // initialize fct4
        fct4 = shared_ptr<FeatureExp>(a->clone());
        tmp = shared_ptr<FeatureExp>(b->clone());
        fct4->disjunction(*tmp);
        // initialize fct5
        fct5 = shared_ptr<FeatureExp>(a->clone());
        tmp = shared_ptr<FeatureExp>(b->clone());
        fct5->negation();
        tmp->negation();
        fct5->conjunction(*tmp);
        fct5->negation();
        // initialize fct6
        fct6 = shared_ptr<FeatureExp>(a->clone());
        tmp = shared_ptr<FeatureExp>(b->clone());
        fct6->negation();
        tmp->negation();
        fct6->disjunction(*tmp);
        fct6->negation();
    }

    /**
     * Tears down the test fixture.
     */
    void TearDown() {}
};

TEST_F(BoolFeatureExpTest, isSatisfiable) {
    // Verify
    EXPECT_FALSE(f->isSatisfiable());
    EXPECT_TRUE(t->isSatisfiable());
    EXPECT_FALSE(fct1->isSatisfiable());
    EXPECT_TRUE(fct2->isSatisfiable());
    EXPECT_TRUE(fct3->isSatisfiable());
    EXPECT_TRUE(fct4->isSatisfiable());
}

TEST_F(BoolFeatureExpTest, isTautology) {
    // Verify
    EXPECT_FALSE(f->isTautology());
    EXPECT_TRUE(t->isTautology());
    EXPECT_FALSE(fct1->isTautology());
    EXPECT_TRUE(fct2->isTautology());
    EXPECT_FALSE(fct3->isTautology());
    EXPECT_FALSE(fct4->isTautology());
}

TEST_F(BoolFeatureExpTest, isEquivalent) {
    // Verify
    EXPECT_TRUE(fct3->isEquivalent(*fct6));
    EXPECT_TRUE(fct4->isEquivalent(*fct5));
    EXPECT_FALSE(fct3->isEquivalent(*fct4));
}

TEST_F(BoolFeatureExpTest, isEquivalent_aliasing) {
    // Verify
    EXPECT_TRUE(fct3->isEquivalent(*fct3));
}

TEST_F(BoolFeatureExpTest, implies) {
    // Verify
    EXPECT_FALSE(a->implies(*fct3));
    EXPECT_TRUE(a->implies(*fct4));
    EXPECT_TRUE(fct3->implies(*b));
    EXPECT_FALSE(fct4->implies(*b));
}

TEST_F(BoolFeatureExpTest, implies_aliasing) {
    // Verify
    EXPECT_TRUE(a->implies(*a));
}

} // namespace features
} // namespace core
