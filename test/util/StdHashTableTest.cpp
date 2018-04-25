/*
 * StdHashTableTest.cpp
 *
 * Created on 2013-07-15
 */

#include "gtest/gtest.h"

#include "core/fts/State.hpp"
#include "core/fts/fstm/FstmState.hpp"
#include "util/StdHashTable.hpp"

#include <tr1/memory>

using core::fts::State;
using core::fts::fstm::FstmState;
using util::Generator;
using util::List;
using std::auto_ptr;
using std::string;
using std::tr1::shared_ptr;

namespace util {

/**
 * Unit tests for the StdHashTable class.
 *
 * @author mwi
 */
class StdHashTableTest : public ::testing::Test {

protected:
    HashTable<State, string> * target;

    /**
     * Sets up the test fixture.
     */
    void SetUp() {
        target = new StdHashTable<State, string>();
    }

    /**
     * Tears down the test fixture.
     */
    void TearDown() {
        delete target;
    }
};

/**
 * Tests the size method for an empty hash table.
 */
TEST_F(StdHashTableTest, size_empty) {
    // Exercise & Verify
    ASSERT_EQ(0, target->count()) << "The hashtable should be empty";
}

/**
 * Tests the find method with a non-existing key.
 */
TEST_F(StdHashTableTest, find_notFound) {
    // Setup
    shared_ptr<FstmState> key = FstmState::makeState("3");

    // Exercise & Verify
    ASSERT_FALSE(target->find(*key, "dummy"))
        << "The key '" << key << "' should not exist";
}

/**
 * Tests the search method on an empty hash table.
 */
TEST_F(StdHashTableTest, search_empty) {
    // Setup
    shared_ptr<FstmState> key = FstmState::makeState("5");

    // Exercise
    auto_ptr<Generator<string> > g = target->search(*key);

    // Verify
    ASSERT_FALSE(g->hasNext())
        << "The generator should not return any element";
}

/**
 * Tests the search method with on an hash table with many entries.
 */
TEST_F(StdHashTableTest, search) {
    // Setup
    shared_ptr<FstmState> key = FstmState::makeState("3");
    const string value1("Jacen Solo");
    const string value2("Jaina Solo");
    target->insert(*key, value1);
    target->insert(*key, value2);

    // Exercise
    auto_ptr<Generator<string> > g = target->search(*key);

    // Verify
    bool hasValue1 = false;
    bool hasValue2 = false;
    int i = 0;
    while (g->hasNext()) {
        const string s(g->next());
        if (s == value1) {
            hasValue1 = true;
        } else if (s == value2) {
            hasValue2 = true;
        }
        i++;
    }
    ASSERT_TRUE(hasValue1);
    ASSERT_TRUE(hasValue2);
    ASSERT_EQ(2, i);
}

/**
 * Tests the insert method.
 */
TEST_F(StdHashTableTest, insert) {
    // Setup
    shared_ptr<FstmState> key = FstmState::makeState("3");
    const string value("Yoda");

    // Exercise
    target->insert(*key, value);

    // Verify
    ASSERT_EQ(1, target->count())
        << "The hash table should contain 1 entry";
    ASSERT_TRUE(target->find(*key, value)) << "<" << key << ", " << value
        << "> should be found in the hash table";
    auto_ptr<Generator<string> > g = target->search(*key);
    ASSERT_TRUE(g->hasNext());
    ASSERT_EQ(value, g->next());
    ASSERT_FALSE(g->hasNext());
}

/**
 * Tests the insert method with duplicated values for the same key.
 */
TEST_F(StdHashTableTest, insert_duplicatedValues) {
    // Setup
    shared_ptr<FstmState> key = FstmState::makeState("10");
    const string value("Muahahaha");

    // Exercise
    target->insert(*key, value);
    target->insert(*key, value);

    // Verify
    ASSERT_EQ(2, target->count());
    ASSERT_TRUE(target->find(*key, value));
    auto_ptr<Generator<string> > g = target->search(*key);
    ASSERT_EQ(value, g->next());
    ASSERT_EQ(value, g->next());
    ASSERT_FALSE(g->hasNext());
}

/**
 * Tests the remove method.
 */
TEST_F(StdHashTableTest, remove) {
    // Setup
    shared_ptr<FstmState> key = FstmState::makeState("35");
    const string value1("Jon Snow");
    const string value2("Arya Stark");
    target->insert(*key, value1);
    target->insert(*key, value2);

    // Exercise
    const bool removed = target->remove(*key, value1);

    // Verify
    ASSERT_TRUE(removed);
    ASSERT_EQ(1, target->count());
    ASSERT_FALSE(target->find(*key, value1));
    ASSERT_TRUE(target->find(*key, value2));
}

/**
 * Tests the remove method with a non-existing key.
 */
TEST_F(StdHashTableTest, remove_nonExistingKey) {
    // Setup
    shared_ptr<FstmState> badKey = FstmState::makeState("30");
    shared_ptr<FstmState> key = FstmState::makeState("100");
    const string value("Mara Jade Skywalker");
    target->insert(*key, value);

    // Exercise
    const bool removed = target->remove(*badKey, value);

    // Verify
    ASSERT_FALSE(removed);
    ASSERT_EQ(1, target->count());
}

/**
 * Tests the remove method with a non-existing value.
 */
TEST_F(StdHashTableTest, remove_nonExistingValue) {
    // Setup
    shared_ptr<FstmState> key = FstmState::makeState("101");
    const string value("Luke Skywalker");
    target->insert(*key, value);
    target->remove(*key, value);

    // Exercise
    const bool removed = target->remove(*key, value);

    // Verify
    ASSERT_FALSE(removed);
    ASSERT_EQ(0, target->count());
}

/**
 * Tests the removeEntry method.
 */
TEST_F(StdHashTableTest, removeEntry) {
    // Setup
    shared_ptr<FstmState> key1 = FstmState::makeState("3");
    shared_ptr<FstmState> key2 = FstmState::makeState("5");
    const string value1("Sansa Stark");
    const string value2("Brienne de Torth");
    target->insert(*key1, value1);
    target->insert(*key1, value2);
    target->insert(*key2, value1);
    target->insert(*key2, value1);

    // Exercise
    const bool removed = target->removeEntry(*key1);

    // Verify
    ASSERT_TRUE(removed);
    ASSERT_EQ(2, target->count());
    ASSERT_FALSE(target->find(*key1, value1));
    ASSERT_FALSE(target->find(*key1, value2));
    ASSERT_TRUE(target->find(*key2, value1));
}

/**
 * Tests the removeEntry method with a non-existing key.
 */
TEST_F(StdHashTableTest, removeEntry_nonExistingKey) {
    // Setup
    shared_ptr<FstmState> key = FstmState::makeState("10");
    const string value("Miranda Lambert");
    target->insert(*key, value);
    target->insert(*key, value);
    target->removeEntry(*key);

    // Exercise
    const bool removed = target->removeEntry(*key);

    // Verify
    ASSERT_FALSE(removed);
    ASSERT_EQ(0, target->count());
}

} // namespace util
