/*
 * NestedSearchTest.cpp
 *
 * Created on 2013-08-19
 */

#include "gtest/gtest.h"

#include "config/Context.hpp"
#include "core/checker/NestedSearch.hpp"
#include "core/fts/fstm/FSTM.hpp"
#include "core/features/BoolFeatureExp.hpp"

#include <glog/logging.h>

using config::Context;
using core::fts::FTS;
using core::fts::fstm::FSTM;
using core::features::BoolFeatureExp;
using core::features::BoolFeatureExpFactory;
using core::features::FeatureExp;
using std::string;
using std::stringstream;
using std::tr1::shared_ptr;

namespace core {
namespace checker {

/**
 * Unit tests for the NestedSearch class.
 *
 * @author bdawagne, mwi
 */
class NestedSearchTest : public ::testing::Test {

protected:
    shared_ptr<NestedSearch> target;
    shared_ptr<BoolFeatureExpFactory> factory;
    shared_ptr<BoolFeatureExp> fd;

    /**
     * Sets up the test fixture.
     */
    void SetUp() {
        DVLOG(1) << "NestedSearchTest";
        Context ctx;
        factory = ctx.getBoolFeatureExpFactory();
        fd.reset(factory->getTrue());
    }

    /**
     * Tears down the test fixture.
     */
    void TearDown() {}

    void init(int test, const string & property) {

        DVLOG(1) << "Test: " << test;
        DVLOG(1) << "Property: " << property;

        stringstream jsonFstm;

        switch(test) {
            case 0:
                jsonFstm
                        << " {"
                        << "   \"FSTM\":{"
                        << "      \"name\":\"Test1\","
                        << "      \"generated\":\"Tue Aug 06 10:31:18 CEST 2013\","
                        << "      \"nbOfLtlProperties\":1,"
                        << "      \"nbOfTransitions\":6"
                        << "   },"
                        << "   \"states\":["
                        << "      \"state1\","
                        << "      \"state2\""
                        << "   ],"
                        << "   \"events\":["
                        << "      \"evt\""
                        << "   ],"
                        << "   \"ltlProperties\":[\"" + property + "\""
                        << "   ],"
                        << "   \"transitions\":[ "
                        << "      {"
                        << "         \"source\":\"state1\","
                        << "         \"target\":\"state1\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":null"
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state1\","
                        << "         \"target\":\"state2\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":\"f\""
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state2\","
                        << "         \"target\":\"state2\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":null"
                        << "      }"
                        << "   ]"
                        << "}";
                break;
            case 1:
                jsonFstm
                        << " {"
                        << "   \"FSTM\":{"
                        << "      \"name\":\"Test1\","
                        << "      \"generated\":\"Tue Aug 06 10:31:18 CEST 2013\","
                        << "      \"nbOfLtlProperties\":1,"
                        << "      \"nbOfTransitions\":6"
                        << "   },"
                        << "   \"states\":["
                        << "      \"state1\","
                        << "      \"state2\","
                        << "      \"state3\","
                        << "      \"state4\","
                        << "      \"state5\""
                        << "   ],"
                        << "   \"events\":["
                        << "      \"evt0\","
                        << "      \"evt1\","
                        << "      \"evt2\","
                        << "      \"evt3\","
                        << "      \"evt4\","
                        << "      \"evt5\","
                        << "      \"evt6\","
                        << "      \"evt7\""
                        << "   ],"
                        << "   \"ltlProperties\":[\"" + property + "\""
                        << "   ],"
                        << "   \"transitions\":[ "
                        << "      {"
                        << "         \"source\":\"state1\","
                        << "         \"target\":\"state2\","
                        << "         \"event\":\"evt1\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":null"
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state2\","
                        << "         \"target\":\"state3\","
                        << "         \"event\":\"evt2\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":null"
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state2\","
                        << "         \"target\":\"state4\","
                        << "         \"event\":\"evt3\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":\"f || g\""
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state3\","
                        << "         \"target\":\"state1\","
                        << "         \"event\":\"evt4\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":null"
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state3\","
                        << "         \"target\":\"state4\","
                        << "         \"event\":\"evt5\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":\"h\""
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state4\","
                        << "         \"target\":\"state3\","
                        << "         \"event\":\"evt6\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":\"f || h\""
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state4\","
                        << "         \"target\":\"state4\","
                        << "         \"event\":\"evt7\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":\"g\""
                        << "      }"
                        << "   ]"
                        << "}";
                break;
            case 2 :
                jsonFstm
                        << "{"
                        << "   \"FSTM\":{"
                        << "      \"name\":\"Test\","
                        << "      \"generated\":\"Tue Aug 06 10:31:18 CEST 2013\","
                        << "      \"nbOfLtlProperties\":1,"
                        << "      \"nbOfTransitions\":12"
                        << "   },"
                        << "   \"states\":["
                        << "      \"state1\","
                        << "      \"state2\","
                        << "      \"state3\","
                        << "      \"state4\","
                        << "      \"state5\","
                        << "      \"state6\","
                        << "      \"state7\","
                        << "      \"state8\","
                        << "      \"state9\","
                        << "      \"state10\""
                        << "   ],"
                        << "   \"events\":["
                        << "      \"evt\""
                        << "   ],"
                        << "   \"ltlProperties\":[\"" + property + "\""
                        << "   ],"
                        << "   \"transitions\":[ "
                        << "      {"
                        << "         \"source\":\"state1\","
                        << "         \"target\":\"state2\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":null"
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state1\","
                        << "         \"target\":\"state3\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":null"
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state2\","
                        << "         \"target\":\"state4\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":null"
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state3\","
                        << "         \"target\":\"state4\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":null"
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state4\","
                        << "         \"target\":\"state6\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":\"!(f)\""
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state4\","
                        << "         \"target\":\"state7\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":null"
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state5\","
                        << "         \"target\":\"state5\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":null"
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state6\","
                        << "         \"target\":\"state8\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":null"
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state7\","
                        << "         \"target\":\"state9\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":null"
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state8\","
                        << "         \"target\":\"state10\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":null"
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state9\","
                        << "         \"target\":\"state10\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":null"
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state9\","
                        << "         \"target\":\"state6\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":\"f\""
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state10\","
                        << "         \"target\":\"state1\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":null"
                        << "      }"
                        << "   ]"
                        << "}";
                break;
            case 3 :
                jsonFstm
                        << "{"
                        << "   \"FSTM\":{"
                        << "      \"name\":\"Test\","
                        << "      \"generated\":\"Tue Aug 06 10:31:18 CEST 2013\","
                        << "      \"nbOfLtlProperties\":1,"
                        << "      \"nbOfTransitions\":9"
                        << "   },"
                        << "   \"states\":["
                        << "      \"state1\","
                        << "      \"state2\","
                        << "      \"state3\","
                        << "      \"state4\","
                        << "      \"state5\","
                        << "      \"state6\""
                        << "   ],"
                        << "   \"events\":["
                        << "      \"evt\""
                        << "   ],"
                        << "   \"ltlProperties\":[\"[] (state5 -> X state3)\""
                        << "   ],"
                        << "   \"transitions\":[ "
                        << "      {"
                        << "         \"source\":\"state1\","
                        << "         \"target\":\"state2\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":\"f\""
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state1\","
                        << "         \"target\":\"state2\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":\"!(f)\""
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state2\","
                        << "         \"target\":\"state3\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":\"g\""
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state2\","
                        << "         \"target\":\"state3\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":\"!(g)\""
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state3\","
                        << "         \"target\":\"state4\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":\"h\""
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state4\","
                        << "         \"target\":\"state5\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":\"h\""
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state5\","
                        << "         \"target\":\"state6\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":\"h\""
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state5\","
                        << "         \"target\":\"state3\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":\"h\""
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state6\","
                        << "         \"target\":\"state3\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":\"h\""
                        << "      }"
                        << "   ]"
                        << "}";
                break;
        }

        shared_ptr<FTS> fts(new FSTM(jsonFstm, *factory));

        target = shared_ptr<NestedSearch>(new NestedSearch(fd, fts, factory));

    }

};

// ____TEST 0____

/*TEST_F(NestedSearchTest, test0_true) {
    // Setup
    init(0, "true");
    shared_ptr<FeatureExp> badProducts(factory->getFalse();

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}*/

TEST_F(NestedSearchTest, test0_false) {
    // Setup
    init(0, "false");
    shared_ptr<FeatureExp> badProducts(factory->getTrue());

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test0_safety1) {
    // Setup
    init(0, "[] ! state2");
    shared_ptr<FeatureExp> badProducts(factory->create("f"));

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test0_liveness1) {
    // Setup
    init(0, "[] state1");
    shared_ptr<FeatureExp> badProducts(factory->create("f"));

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

// ____TEST 1____

/*TEST_F(NestedSearchTest, test1_true) {
    // Setup
    init(1, "true");
    shared_ptr<FeatureExp> badProducts(factory->getFalse());

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}*/

TEST_F(NestedSearchTest, test1_false) {
    // Setup
    init(1, "false");
    shared_ptr<FeatureExp> badProducts(factory->getTrue());

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test1_safety1) {
    // Setup
    init(1, "[] (state1 -> (X state2))");
    shared_ptr<FeatureExp> badProducts(factory->getFalse());

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test1_safety2) {
    // Setup
    init(1, "[] (state2 -> (X state3))");
    shared_ptr<FeatureExp> badProducts(factory->create("f"));
    badProducts->disjunction(*factory->create("g"));

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test1_safety3) {
    // Setup
    init(1, "[] ((X state3) -> state2)");
    shared_ptr<FeatureExp> badProducts(factory->create("f"));
    badProducts->disjunction(*factory->create("h"));

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test1_oneday1) {
    // Setup
    init(1, "<> state2");
    shared_ptr<FeatureExp> badProducts(factory->getFalse());

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test1_oneday2) {
    // Setup
    init(1, "<> state3");
    shared_ptr<FeatureExp> badProducts(factory->create("g"));

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test1_oneday3) {
    // Setup
    init(1, "<> state4");
    shared_ptr<FeatureExp> badProducts(factory->getTrue());

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test1_liveness1) {
    // Setup
    init(1, "[] <> state1");
    shared_ptr<FeatureExp> badProducts(factory->create("g"));
    badProducts->disjunction(*factory->create("h"));

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test1_livenes2) {
    // Setup
    init(1, "[] <> state2");
    shared_ptr<FeatureExp> badProducts(factory->create("g"));
    badProducts->disjunction(*factory->create("h"));

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test1_liveness3) {
    // Setup
    init(1, "[] <> state3");
    shared_ptr<FeatureExp> badProducts(factory->create("g"));

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

// ____TEST 2____

/*TEST_F(NestedSearchTest, test2_true) {
    // Setup
    init(2, "true");
    shared_ptr<FeatureExp> badProducts(factory->getFalse());

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}*/

TEST_F(NestedSearchTest, test2_false) {
    // Setup
    init(2, "false");
    shared_ptr<FeatureExp> badProducts(factory->getTrue());

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test2_safety1) {
    // Setup
    init(2, "[] (state1 -> X X state4)");
    shared_ptr<FeatureExp> badProducts(factory->getFalse());

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test2_safety2) {
    // Setup
    init(2, "[] (state4 -> X X X state10)");
    shared_ptr<FeatureExp> badProducts(factory->create("f"));

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test2_safety3) {
    // Setup
    init(2, "[] (state1 -> X X X X X state10)");
    shared_ptr<FeatureExp> badProducts(factory->create("f"));

    // Exercise
    target->run();


    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test2_safety4) {
    // Setup
    init(2, "[] ((X X state4) -> state1)");
    shared_ptr<FeatureExp> badProducts(factory->getFalse());

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test2_safety5) {
    // Setup
    init(2, "[] ((X X X state10) -> state4)");
    shared_ptr<FeatureExp> badProducts(factory->create("f"));

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test2_safety6) {
    // Setup
    init(2, "[] ((X X X X X state10) -> state1)");
    shared_ptr<FeatureExp> badProducts(factory->create("f"));

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test2_safety7) {
    // Setup
    init(2, "[] (state1 -> X (state2 || state3))");
    shared_ptr<FeatureExp> badProducts(factory->getFalse());

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test2_safety8) {
    // Setup
    init(2, "[] (state4 -> X X (state8 || state9))");
    shared_ptr<FeatureExp> badProducts(factory->getFalse());

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test2_safety9) {
    // Setup
    init(2, "[] ! state5");
    shared_ptr<FeatureExp> badProducts(factory->getFalse());

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test2_liveness1) {
    // Setup
    init(2, "[] <> state1");
    shared_ptr<FeatureExp> badProducts(factory->getFalse());

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test2_liveness2) {
    // Setup
    init(2, "[] <> state4");
    shared_ptr<FeatureExp> badProducts(factory->getFalse());

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test2_liveness3) {
    // Setup
    init(2, "[] <> state10");
    shared_ptr<FeatureExp> badProducts(factory->getFalse());

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test2_liveness4) {
    // Setup
    init(2, "([] <> state1) && ([] <> state4) && ([] <> state10)");
    shared_ptr<FeatureExp> badProducts(factory->getFalse());

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(NestedSearchTest, test2_liveness5) {
    // Setup
    init(2, "<> state5");
    shared_ptr<FeatureExp> badProducts(factory->getTrue());

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

// ____TEST 3____

TEST_F(NestedSearchTest, test3_safety1) {
    // Setup
    init(3, "[] (state5 -> X state3)");
    shared_ptr<FeatureExp> badProducts(factory->create("h"));

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

} // namespace checker
} // namespace core
