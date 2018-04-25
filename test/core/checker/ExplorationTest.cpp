/*
 * ExplorationTest.cpp
 *
 * Created on 2013-08-26
 */

#include "gtest/gtest.h"

#include "config/Context.hpp"
#include "core/checker/Exploration.hpp"
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
 * Unit tests for the Exploration class.
 *
 * @author bdawagne, mwi
 */
class ExplorationTest : public ::testing::Test {

protected:
    shared_ptr<Exploration> target;
    shared_ptr<BoolFeatureExpFactory> factory;
    shared_ptr<BoolFeatureExp> fd;

    /**
     * Sets up the test fixture.
     */
    void SetUp() {
        DVLOG(1) << "ExplorationTest";
        Context ctx;
        factory = ctx.getBoolFeatureExpFactory();
        fd.reset(factory->getTrue());
    }

    /**
     * Tears down the test fixture.
     */
    void TearDown() {}

    void init(int test) {

        DVLOG(1) << "Test: " << test;

        stringstream jsonFstm;

        switch(test) {
            case 0:
                jsonFstm
                        << "{"
                        << "   \"FSTM\":{"
                        << "      \"name\":\"Test\","
                        << "      \"generated\":\"Mon Aug 26 15:54:51 CEST 2013\","
                        << "      \"nbOfLtlProperties\":0,"
                        << "      \"nbOfTransitions\":3"
                        << "   },"
                        << "   \"states\":["
                        << "      \"state1\","
                        << "      \"state2\""
                        << "   ],"
                        << "   \"events\":["
                        << "      \"evt\""
                        << "   ],"
                        << "   \"ltlProperties\":["
                        << "   ],"
                        << "   \"transitions\":["
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
                        << "         \"featureExp\":\"f || g\""
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state2\","
                        << "         \"target\":\"state1\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":\"f && !(g)\""
                        << "      }"
                        << "   ]"
                        << "}";
                break;
            case 1:
                jsonFstm
                        << "{"
                        << "   \"FSTM\":{"
                        << "      \"name\":\"Test\","
                        << "      \"generated\":\"Mon Aug 26 15:54:51 CEST 2013\","
                        << "      \"nbOfLtlProperties\":0,"
                        << "      \"nbOfTransitions\":6"
                        << "   },"
                        << "   \"states\":["
                        << "      \"state1\","
                        << "      \"state2\","
                        << "      \"state3\","
                        << "      \"state4\""
                        << "   ],"
                        << "   \"events\":["
                        << "      \"evt\""
                        << "   ],"
                        << "   \"ltlProperties\":["
                        << "   ],"
                        << "   \"transitions\":["
                        << "      {"
                        << "         \"source\":\"state1\","
                        << "         \"target\":\"state2\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":\"f\""
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state1\","
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
                        << "         \"featureExp\":\"h\""
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
                        << "         \"target\":\"state3\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":\"i\""
                        << "      }"
                        << "   ]"
                        << "}";
                break;
            case 2:
                jsonFstm
                        << "{"
                        << "   \"FSTM\":{"
                        << "      \"name\":\"Test\","
                        << "      \"generated\":\"Mon Aug 26 15:54:51 CEST 2013\","
                        << "      \"nbOfLtlProperties\":0,"
                        << "      \"nbOfTransitions\":5"
                        << "   },"
                        << "   \"states\":["
                        << "      \"state1\","
                        << "      \"state2\","
                        << "      \"state3\""
                        << "   ],"
                        << "   \"events\":["
                        << "      \"evt\""
                        << "   ],"
                        << "   \"ltlProperties\":["
                        << "   ],"
                        << "   \"transitions\":["
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
                        << "         \"featureExp\":\"!(g)\""
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
                        << "         \"featureExp\":\"!(f)\""
                        << "      },"
                        << "      {"
                        << "         \"source\":\"state3\","
                        << "         \"target\":\"state1\","
                        << "         \"event\":\"evt\","
                        << "         \"guard\":null,"
                        << "         \"featureExp\":null"
                        << "      }"
                        << "   ]"
                        << "}";
                break;
        }

        shared_ptr<FTS> fts(new FSTM(jsonFstm, *factory));

        target = shared_ptr<Exploration>(new Exploration(fd, fts, factory));

    }

};

TEST_F(ExplorationTest, test0) {
    // Setup
    init(0);
    shared_ptr<FeatureExp> badProducts(factory->create("g"));

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(ExplorationTest, test1) {
    // Setup
    init(1);
    shared_ptr<FeatureExp> badProducts(factory->create("i"));
    badProducts->negation();
    badProducts->disjunction(
        *factory->create("f")->negation()->conjunction(
        *factory->create("g")->negation()));

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

TEST_F(ExplorationTest, test2) {
    // Setup
    init(2);
    shared_ptr<FeatureExp> badProducts(factory->create("f"));
    badProducts->conjunction(*factory->create("g"));
    badProducts->disjunction(
        *factory->create("f")->negation()->conjunction(
        *factory->create("g")->negation()));
    badProducts->negation();

    // Exercise
    target->run();

    // Verify
    ASSERT_TRUE(badProducts->isEquivalent(*target->getBadProducts()));
}

} // namespace checker
} // namespace core
