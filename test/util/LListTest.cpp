/*
 * LListTest.cpp
 *
 *  Created on 2013-07-12
 */

#include "gtest/gtest.h"

#include "util/List.hpp"
#include "util/LList.hpp"

using std::auto_ptr;
using std::string;
using util::LList;
using util::List;

namespace util {

/**
 * Unit tests for the LList class.
 *
 * @author bdawagne
 */
class LListTest : public ::testing::Test {

protected:
    LList<int> llist1;
    LList<int> llist2;
    LList<int> llist3;

    void SetUp() {
        // Initialize llist2
        llist2.add("0", 0);
        llist2.add("1", 1);
        llist2.add("2", 2);
        llist2.add("3", 3);
        llist2.add("4", 4);
        llist2.add("5", 5);
        llist2.add("6", 6);
        llist2.add("7", 7);
        llist2.add("8", 8);
        llist2.add("9", 9);
         // Initialize llist3
        llist3.add("7", 10);
        llist3.add("8", 11);
        llist3.add("9", 12);
        llist3.add("10", 13);
        llist3.add("11", 14);
        llist3.add("12", 15);
        llist3.add("13", 16);
        llist3.add("14", 17);
        llist3.add("15", 18);
        llist3.add("16", 19);
    }

    void TearDown() {
        // Nothing here
    }
};

TEST_F(LListTest, count) {
    ASSERT_EQ(0, llist1.count());
    ASSERT_EQ(10, llist2.count());
}

TEST_F(LListTest, find) {
    ASSERT_EQ(5, llist2.find("5"));
    ASSERT_EQ(14, llist3.find("11"));
}

TEST_F(LListTest, contains) {
    ASSERT_TRUE(llist2.contains("5"));
    ASSERT_FALSE(llist2.contains("11"));
}

TEST_F(LListTest, concat) {
    // Exercise
    llist2.concat(llist3);

    // Verify
    ASSERT_EQ(20, llist2.count());
    ASSERT_EQ(8, llist2.find("8"));
}

TEST_F(LListTest, merge) {
    // Exercise
    llist2.merge(llist3);

    // Verify
    ASSERT_EQ(17, llist2.count());
    ASSERT_EQ(8, llist2.find("8"));
}

TEST_F(LListTest, remove) {
    ASSERT_FALSE(llist1.remove(5));
    ASSERT_TRUE(llist2.remove(5));
    ASSERT_EQ(9, llist2.count());
    ASSERT_FALSE(llist2.contains("5"));
}

TEST_F(LListTest, removeLabel) {
    ASSERT_FALSE(llist1.removeLabel("5"));
    ASSERT_TRUE(llist2.removeLabel("5"));
    ASSERT_EQ(9, llist2.count());
    ASSERT_FALSE(llist2.contains("5"));
}

/**
 * Tests the removeLabel method with a unique element in the list.
 */
TEST_F(LListTest, removeLabel_uniqueElt) {
    // Setup
    const string label("aLabel");
    LList<int> target;
    target.add(label, 3);

    // Exercise
    target.removeLabel(label);

    // Verify
    ASSERT_FALSE(target.contains(label));
    ASSERT_EQ(0, target.count());
}

TEST_F(LListTest, list2llist) {
    // Setup
    List<std::string> list;
    list.addLast("a");
    list.addLast("b");
    list.addLast("c");
    list.addLast("d");

    // Exercise
    auto_ptr<LList<int> > llist(LList<int>::list2llist(list));

    // Verify
    ASSERT_EQ(4, llist->count());
    ASSERT_EQ(0, llist->find("a"));
    ASSERT_EQ(1, llist->find("b"));
    ASSERT_EQ(2, llist->find("c"));
    ASSERT_EQ(3, llist->find("d"));
}

} // namespace util
