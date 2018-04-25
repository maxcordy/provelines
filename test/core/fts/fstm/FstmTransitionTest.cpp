/*
 * FstmTransitionTest.cpp
 *
 * Created on 2013-07-29
 */

#include "gtest/gtest.h"

#include "core/features/BoolFeatureExp.hpp"
#include "core/features/BoolFeatureExpFactory.hpp"
#include "core/features/FeatureExp.hpp"
#include "core/fts/fstm/FstmTransition.hpp"
#include "core/fts/fstm/FstmEvent.hpp"
#include "core/fts/fstm/FstmState.hpp"
#include "math/BoolFctCUDDFactory.hpp"

using core::features::BoolFeatureExp;
using core::features::BoolFeatureExpFactory;
using core::features::FeatureExp;
using core::features::FeatureExpPtr;
using core::fts::fstm::FstmEvent;
using core::fts::fstm::FstmState;
using math::BoolFctCUDDFactory;
using math::BoolFctFactory;
using std::string;
using std::tr1::shared_ptr;

namespace core {
namespace fts {
namespace fstm {

    /**
     * Unit tests for the FstmTransition class.
     *
     * @author mwi
     */
    class FstmTransitionTest : public ::testing::Test {

    protected:
        shared_ptr<BoolFeatureExpFactory> factory;
        shared_ptr<BoolFeatureExp> expectedExp;
        shared_ptr<FstmState> expectedSource;
        shared_ptr<FstmState> expectedTarget;
        shared_ptr<FstmEvent> expectedEvent;
        shared_ptr<FstmTransition> transition;

        /**
         * Sets up the test fixture.
         */
	void SetUp() {
            shared_ptr<BoolFctFactory> boolFctFactory(new BoolFctCUDDFactory());
            factory = shared_ptr<BoolFeatureExpFactory>(
                    new BoolFeatureExpFactory(boolFctFactory));

            const string sourceName("SourceState");
            const string targetName("TargetState");
            const string eventName("MyEvent");

            expectedSource = FstmState::makeState(sourceName);
            expectedTarget = FstmState::makeState(targetName);
            expectedEvent = FstmEvent::makeEvent(eventName);

            // _A || _B && !(_C)
            expectedExp.reset(factory->create("_C"));
            expectedExp->negation();
            expectedExp->conjunction(*factory->create("_B"));
            expectedExp->disjunction(*factory->create("_A"));

            transition = shared_ptr<FstmTransition>(new FstmTransition(
                    sourceName, targetName, eventName, expectedExp));
        }

        /**
         * Tears down the test fixture.
         */
	void TearDown() {}
    };

    TEST_F(FstmTransitionTest, getSource) {
        // Exercise
        shared_ptr<FstmState> actualSource = transition->getSource();

        // Verify
        ASSERT_EQ(expectedSource->getName(), actualSource->getName());
    }

    TEST_F(FstmTransitionTest, getTarget) {
        // Exercise
        shared_ptr<FstmState> actualTarget = transition->getTarget();

        // Verify
        ASSERT_EQ(expectedTarget->getName(), actualTarget->getName());
    }

    TEST_F(FstmTransitionTest, getEvent) {
        // Exercise
        shared_ptr<FstmEvent> actualEvent = transition->getEvent();

        // Verify
        ASSERT_EQ(expectedEvent->getName(), actualEvent->getName());
    }

    TEST_F(FstmTransitionTest, getFeatureExp) {
        // Exercise
        shared_ptr<FeatureExp> actualExp = transition->getFeatureExp();

        // Verify
        ASSERT_TRUE(actualExp->isEquivalent(*expectedExp));
    }

    /**
     * Tests the getFeatureExp for an empty feature expression.
     */
    TEST_F(FstmTransitionTest, getFeatureExp_empty) {
        // Setup
        FstmTransition t("src", "target", "evt",
                shared_ptr<FeatureExp>(factory->getTrue()));

        // Exercise
        shared_ptr<FeatureExp> actualExp = t.getFeatureExp();

        // Verify
        ASSERT_TRUE(actualExp->isEquivalent(*factory->getTrue()));
    }

    /**
     * Tests the getFeatureExp does not expose the representation.
     */
    TEST_F(FstmTransitionTest, getFeatureExp_exposingRep) {
        // Setup
        FeatureExpPtr actualExp = transition->getFeatureExp();

        // Exercise
        actualExp->negation();

        // Verify
        ASSERT_FALSE(actualExp->isEquivalent(*transition->getFeatureExp()));
    }

} // namespace fstm
} // namespace fts
} // namespace core
