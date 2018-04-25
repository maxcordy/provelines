/*
 * FstmEvent.cpp
 *
 * Created on 2013-07-26
 */

#include "gtest/gtest.h"

#include "core/fts/fstm/FstmEvent.hpp"

using std::string;
using std::tr1::shared_ptr;

namespace core {
namespace fts {
namespace fstm {

    /**
     * Unit tests for the FstmEvent class.
     *
     * @author mwi
     */
    class FstmEventTest : public ::testing::Test {

    protected:
        /**
         * Sets up the test fixture.
         */
	void SetUp() {}

        /**
         * Tears down the test fixture.
         */
	void TearDown() {}
    };

    TEST_F(FstmEventTest, makeEvent) {
        // Setup
        const string name("Arya Stark");

        // Exercise
        shared_ptr<FstmEvent> state = FstmEvent::makeEvent(name);

        // Verify
        ASSERT_EQ(name, state->getName());
    }

    TEST_F(FstmEventTest, makeEvent_same) {
        // Setup
        const string name("Ghost");
        shared_ptr<FstmEvent> expectedEvent = FstmEvent::makeEvent(name);

        // Exercise
        shared_ptr<FstmEvent> actualEvent = FstmEvent::makeEvent(name);

        // Verify
        ASSERT_EQ(expectedEvent->getName(), actualEvent->getName());
        ASSERT_EQ(expectedEvent.get(), actualEvent.get());
    }

} // namespace fstm
} // namespace fts
} // namespace core
