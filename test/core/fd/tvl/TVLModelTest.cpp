/*
 * TVLModelTest.cpp
 *
 *  Created on 2013-07-30
 */

#include "gtest/gtest.h"

#include "config/Context.hpp"
#include "core/fd/tvl/TVLModel.hpp"
#include "core/features/BoolFeatureExp.hpp"
#include "core/features/FeatureExp.hpp"

#include "math/BoolFctFactory.hpp"

#include "math/BoolVar.hpp"

#include <glog/logging.h>

using config::Context;

using core::fd::tvl::TVLModel;

using core::features::FeatureExp;
using core::features::BoolFeatureExp;
using core::features::BoolFeatureExpFactory;

using math::BoolFctFactory;
using math::BoolVar;

using std::stringstream;
using std::tr1::shared_ptr;

namespace core {
namespace fd {
namespace tvl {

/**
 * Unit tests for the TVLModel class.
 *
 * @author bdawagne
 */
class TVLModelTest : public ::testing::Test {

protected:
    /** Context */
    Context ctx;

    /** BoolFeatureExpFactory */
    shared_ptr<BoolFeatureExpFactory> expFactory;

    /** BoolFctFactory */
    shared_ptr<BoolFctFactory> fctFactory;

    /** true */
    shared_ptr<FeatureExp> t;
    /** false */
    shared_ptr<FeatureExp> f;
    /** a */
    shared_ptr<FeatureExp> a;
    /** id of a */
    int ida;
    /** b */
    shared_ptr<FeatureExp> b;
    /** id of b */
    int idb;
    /** Main */
    shared_ptr<FeatureExp> main;
    /** Main && a && b*/
    shared_ptr<FeatureExp> exp1;
    /** Main && (a || b) */
    shared_ptr<FeatureExp> exp2;
    /** a Xor b */
    shared_ptr<FeatureExp> exp3;

    void SetUp() {
        expFactory = ctx.getBoolFeatureExpFactory();
        fctFactory = ctx.getBoolFctFactory();
    }

    void TearDown() {
        // Nothing here
    }

    void createExp() {
        shared_ptr<FeatureExp> tmp;
        // initialize t
        t.reset(expFactory->getTrue());
        // initialize f
        f.reset(expFactory->getFalse());
        // initialize a
        a.reset(expFactory->create("TVLModelTest_a"));
        ida = BoolVar::makeBoolVar("TVLModelTest_a").getId();
        // initialize b
        b.reset(expFactory->create("TVLModelTest_b"));
        idb = BoolVar::makeBoolVar("TVLModelTest_b").getId();
        // initialize Main
        main.reset(expFactory->create("TVLModelTest_Main"));
        // Initialize exp1
        exp1 = shared_ptr<FeatureExp>(a->clone());
        tmp = shared_ptr<FeatureExp>(b->clone());
        exp1->conjunction(*tmp);
        exp1->conjunction(*main->clone());
        // initialize exp2
        exp2 = shared_ptr<FeatureExp>(a->clone());
        tmp = shared_ptr<FeatureExp>(b->clone());
        exp2->disjunction(*tmp);
        exp2->conjunction(*main->clone());
    }
};

/*TEST_F(TVLModelTest, and) {
    // Setup
    stringstream stream;
    stream
            << "root TVLModelTest_Main group allOf {\n"
            << "TVLModelTest_a,\n"
            << "TVLModelTest_b\n"
            << "}";

    TVLModel model(stream, fctFactory, expFactory);

    // Exercise
    shared_ptr<FeatureExp> exp = model.toFeatureExp();
    createExp();

    // Verify
    ASSERT_TRUE(exp->isEquivalent(*exp1));
}*/

} // namespace tvl
} // namespace fd
} // namespace core
