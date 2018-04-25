/*
 * JsonAdapterTest.cpp
 *
 * Created on 2013-07-29
 */

#include "gtest/gtest.h"

#include "config/Context.hpp"
#include "core/features/BoolFeatureExpFactory.hpp"
#include "core/fts/fstm/JsonParser.hpp"
#include "core/fts/fstm/FstmTransition.hpp"
#include "core/fts/fstm/FstmState.hpp"
#include "core/fts/fstm/FstmEvent.hpp"
#include "core/features/BoolFeatureExp.hpp"

#include <sstream>
#include <tr1/memory>

using config::Context;
using core::features::BoolFeatureExpFactory;
using core::features::FeatureExp;
using std::string;
using std::stringstream;
using std::tr1::shared_ptr;
using util::Generator;

namespace core {
namespace fts {
namespace fstm {

/**
 * Unit tests for the JsonParser class.
 *
 * @author mwi
 */
class JsonParserTest : public ::testing::Test {

protected:
    shared_ptr<JsonParser> target;
    int expectedNbOfTransitions;
    int expectedNbOfProperties;
    shared_ptr<BoolFeatureExpFactory> factory;
    string expectedProperty;
    string state1;
    string state2;

    /**
     * Sets up the test fixture.
     */
    void SetUp() {
        expectedNbOfTransitions = 2;
        expectedNbOfProperties = 1;
        expectedProperty = "[](<>A)";
        state1 = "State1";
        state2 = "State2";

        Context ctx;
        factory = ctx.getBoolFeatureExpFactory();

        stringstream jsonFstm;
        jsonFstm << "{"
                << "\"FSTM\":{ "
                << "\"name\":\"Sample2\","
                << "\"generated\":\"Thu Jul 25 11:01:23 CEST 2013\","
                << "\"nbOfLtlProperties\":1, "
                << "\"nbOfTransitions\":2 "
                << "},"
                << "\"states\":[\"State1\", \"State2\" ],"
                << "\"events\":[\"evt1\", \"evt2\", \"evt3\" ],"
                << "\"ltlProperties\":[\"[](<>A)\"],"
                << "\"transitions\":[ "
                << "{ \"source\":\"State1\","
                << "\"target\":\"State2\", "
                << "\"event\":\"evt1\","
                << "\"guard\":\"\", "
                << "\"featureExp\":\"!(A) && (B && B) || !(C && !(B) || "
                << "(((C && B))))\" },"
                << "{\"source\":\"State2\","
                << "\"target\":\"State1\","
                << "\"event\":\"evt3\","
                << "\"guard\":\"\","
                << "\"featureExp\":null }"
                << "] }";

        target = shared_ptr<JsonParser>(new JsonParser(jsonFstm,
                *factory));
    }

    /**
     * Tears down the test fixture.
     */
    void TearDown() {}

    /**
     * Asserts t equals the first transition of the JSON FSTM.
     */
    void assertFirstTransitionEquals(shared_ptr<FstmTransition> t) {
        ASSERT_EQ("State1", t->getSource()->getName());
        ASSERT_EQ("State2", t->getTarget()->getName());
        ASSERT_EQ("evt1", t->getEvent()->getName());
        shared_ptr<FeatureExp> expectedExp(factory->create("A"));
        expectedExp->negation();
        expectedExp->conjunction(*factory->create("B"));
        expectedExp->conjunction(*factory->create("B"));
        shared_ptr<FeatureExp> rhsOne(factory->create("B"));
        rhsOne->negation();
        rhsOne->conjunction(*factory->create("C"));
        shared_ptr<FeatureExp> rhsTwo(factory->create("C"));
        rhsTwo->conjunction(*factory->create("B"));
        rhsOne->disjunction(*rhsTwo);
        rhsOne->negation();
        expectedExp->disjunction(*rhsOne);
        ASSERT_TRUE(t->getFeatureExp()->isEquivalent(*expectedExp));
    }

    /**
     * Asserts t equals the second transition of the JSON FSTM.
     */
    void assertSecondTransitionEquals(shared_ptr<FstmTransition> t) {
        ASSERT_EQ("State2", t->getSource()->getName());
        ASSERT_EQ("State1", t->getTarget()->getName());
        ASSERT_EQ("evt3", t->getEvent()->getName());
        ASSERT_TRUE(factory->getTrue()->isEquivalent(*t->getFeatureExp()));
    }
};

TEST_F(JsonParserTest, getNbOfTransitions) {
    // Exercise
    int actualNbOfTransition = target->getNbOfTransitions();

    // Verify
    ASSERT_EQ(expectedNbOfTransitions, actualNbOfTransition);
}

TEST_F(JsonParserTest, transitions) {
    // Exercise
    FstmTransitionGeneratorPtr g = target->transitions();

    // Verify
    ASSERT_TRUE(g->hasNext());
    shared_ptr<FstmTransition> transition = g->next();
    assertFirstTransitionEquals(transition);
    ASSERT_TRUE(g->hasNext());
    transition = g->next();
    assertSecondTransitionEquals(transition);
    ASSERT_FALSE(g->hasNext());
}

TEST_F(JsonParserTest, getNbOfProperties) {
    // Exercise
    int actualNbOfProperties = target->getNbOfProperties();

    // Verify
    ASSERT_EQ(expectedNbOfProperties, actualNbOfProperties);
}

TEST_F(JsonParserTest, properties) {
    // Exercise
    shared_ptr<Generator<string> > g = target->properties();

    // Verify
    ASSERT_TRUE(g->hasNext());
    string actualProperty = g->next();
    ASSERT_EQ(expectedProperty, actualProperty);
    ASSERT_FALSE(g->hasNext());
}

TEST_F(JsonParserTest, states) {
    // Exercise
    shared_ptr<Generator<string> > g = target->states();

    // Verify
    ASSERT_TRUE(g->hasNext());
    ASSERT_EQ(state1, g->next());
    ASSERT_TRUE(g->hasNext());
    ASSERT_EQ(state2, g->next());
    ASSERT_FALSE(g->hasNext());
}

} // namespace fstm
} // namespace fts
} // namespace core
