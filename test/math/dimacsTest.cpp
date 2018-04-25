/*
 * dimacsTest.cpp
 *
 * Created on 2013-07-30
 */

#include "gtest/gtest.h"

#include "config/Context.hpp"

#include "math/BoolFct.hpp"
#include "math/BoolFctFactory.hpp"
#include "math/BoolVar.hpp"
#include "math/dimacs.hpp"

using config::Context;

using math::BoolFctFactory;

using std::string;
using std::stringstream;
using std::tr1::shared_ptr;

namespace math {

/**
 * Unit tests for the dimacs functions.
 *
 * @author bdawagne
 */
class dimacsTest : public ::testing::Test {

protected:
    /** Context */
    Context ctx;
    /** BoolFctFactory */
    shared_ptr<BoolFctFactory> factory;
    /** true */
    shared_ptr<BoolFct> t;
    /** false */
    shared_ptr<BoolFct> f;
    /** a */
    shared_ptr<BoolFct> a;
    /** id of a */
    int ida;
    /** b */
    shared_ptr<BoolFct> b;
    /** id of b */
    int idb;
    /** a && b */
    shared_ptr<BoolFct> fct1;
    /** a || b */
    shared_ptr<BoolFct> fct2;

    void SetUp() {
        factory = ctx.getBoolFctFactory();
        shared_ptr<BoolFct> tmp;
        // initialize t
        t.reset(factory->getTrue());
        // initialize f
        f.reset(factory->getFalse());
        // initialize a
        BoolVar varA = BoolVar::makeBoolVar("dimacs_a");
        a.reset(factory->create(varA));
        ida = varA.getId();
        // initialize b
        BoolVar varB = BoolVar::makeBoolVar("dimacs_b");
        b.reset(factory->create(varB));
        idb = varB.getId();
        // Initialize fct1
        fct1 = shared_ptr<BoolFct>(a->clone());
        tmp = shared_ptr<BoolFct>(b->clone());
        fct1->conjunction(*tmp);
        // initialize fct2
        fct2 = shared_ptr<BoolFct>(a->clone());
        tmp = shared_ptr<BoolFct>(b->clone());
        fct2->disjunction(*tmp);
    }

    void TearDown() {
        // Nothing here
    }
};

TEST_F(dimacsTest, and) {
    // Setup
    stringstream stream;
    stream << "p cnf 2 2\n";
    stream << ida << " 0\n";
    stream << idb << " 0\n";

    // Exercise
    shared_ptr<BoolFct> result(parseBoolFct(stream, factory));

    // Verify
    ASSERT_TRUE(result->isEquivalent(*fct1));
}

TEST_F(dimacsTest, or) {
    // Setup
    stringstream stream;
    stream << "p cnf 2 1\n";
    stream << ida << " " << idb << " 0\n";

    // Exercise
    shared_ptr<BoolFct> result(parseBoolFct(stream, factory));

    // Verify
    ASSERT_TRUE(result->isEquivalent(*fct2));
}

TEST_F(dimacsTest, true1) {
    // Setup
    stringstream stream;
    stream << "p cnf 0 0\n";
    stream << "";

    // Exercise
    shared_ptr<BoolFct> result(parseBoolFct(stream, factory));

    // Verify
    ASSERT_TRUE(result->isEquivalent(*t));
}

TEST_F(dimacsTest, false1) {
    // Setup
    stringstream stream;
    stream << "p cnf 0 1\n";
    stream << "0\n";

    // Exercise
    shared_ptr<BoolFct> result(parseBoolFct(stream, factory));

    // Verify
    ASSERT_TRUE(result->isEquivalent(*f));
}

TEST_F(dimacsTest, true2) {
    // Setup
    stringstream stream;
    stream << "p cnf 1 1\n";
    stream << ida << " " << (-ida) << " 0\n";

    // Exercise
    shared_ptr<BoolFct> result(parseBoolFct(stream, factory));

    // Verify
    ASSERT_TRUE(result->isEquivalent(*t));
}

TEST_F(dimacsTest, false2) {
    // Setup
    stringstream stream;
    stream << "p cnf 1 2\n";
    stream << ida << " 0\n";
    stream << (-ida) << " 0\n";

    // Exercise
    shared_ptr<BoolFct> result(parseBoolFct(stream, factory));

    // Verify
    ASSERT_TRUE(result->isEquivalent(*f));
}

} // namespace math
