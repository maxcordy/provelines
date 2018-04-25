/*
 * ListTest.cpp
 *
 * Created on: 2013-07-11
 */

#include "gtest/gtest.h"

#include "util/List.hpp"

using util::Generator;
using util::List;
using std::auto_ptr;
using std::string;

namespace util {

/**
 * Unit tests for the List class.
 *
 * @author bdawagne
 */
class ListTest : public ::testing::Test {

protected:
    List<int> list1;
    List<int> list2;
    List<int> list3;

    void SetUp() {
        // Initialize list2
        for(int i = 0; i < 10 ; i++) {
            list2.addLast(i);
        }
        // Initialize list3
        for (int i = 10; i < 20 ; i++) {
            list3.addLast(i);
        }
    }

    void TearDown() {
        // Nothing here
    }
};

TEST_F(ListTest, count) {
    ASSERT_EQ(0, list1.count());
    ASSERT_EQ(10, list2.count());
}

TEST_F(ListTest, find) {
    ASSERT_TRUE(list2.find(4));
}

TEST_F(ListTest, find_nonExistingElement) {
    ASSERT_FALSE(list1.find(4));
}

TEST_F(ListTest, addFirst) {
    // Setup
    const int element = 25;

    // Exercise
    list2.addFirst(element);

    // Verify
    ASSERT_EQ(11, list2.count());
    ASSERT_TRUE(list2.find(element));
    ASSERT_EQ(element, list2.getFirst());
}

TEST_F(ListTest, addLast) {
    // Setup
    const int element = 25;

    // Exercise
    list2.addLast(element);

    // Verify
    ASSERT_EQ(11, list2.count());
    ASSERT_TRUE(list2.find(element));
    ASSERT_EQ(element, list2.getLast());
}

TEST_F(ListTest, remove) {
    ASSERT_TRUE(list2.removeFirstOccurence(4));
    ASSERT_EQ(9, list2.count());
    ASSERT_FALSE(list2.find(4));
}

TEST_F(ListTest, remove_nonExistingElement) {
    ASSERT_FALSE(list2.removeFirstOccurence(15));
    ASSERT_EQ(10, list2.count());
}

TEST_F(ListTest, concat) {
    // Exercise
    list2.concat(list3);

    // Verify
    ASSERT_EQ(20, list2.count());
    ASSERT_EQ(0, list3.count());
}

TEST_F(ListTest, elements) {
    auto_ptr<Generator<int> > gen(list2.elements());
    for (int i = 0; i < 10; i++) {
        ASSERT_TRUE(gen->hasNext());
        int e = gen->next();
        ASSERT_GE(e, 0);
        ASSERT_LE(e, 10);
    }
    ASSERT_FALSE(gen->hasNext());
}

/**
 * Tests the copy constructor.
 */
TEST_F(ListTest, copy) {
    // Setup
    const string s1("Robb Stark");
    const string s2("Vent Gris");
    List<string> expectedList;
    expectedList.addLast(s1);
    expectedList.addLast(s2);

    // Exercise
    List<string> actualList(expectedList);

    // Verify
    ASSERT_EQ(expectedList.count(), actualList.count());
    auto_ptr<Generator<string> > g = actualList.elements();
    ASSERT_EQ(s1, g->next());
    ASSERT_EQ(s2, g->next());
    ASSERT_FALSE(g->hasNext());
    ASSERT_EQ(s1, actualList.getFirst());
    ASSERT_EQ(s2, actualList.getLast());
}

} // namespace util
