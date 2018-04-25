/*
 * StackTest.cpp
 *
 * Created on 2013-07-15
 */

#include "gtest/gtest.h"

#include "util/Stack.hpp"

using std::auto_ptr;

namespace util {

/**
 * Unit tests for the Stack class.
 *
 * @author bdawagne
 */
class StackTest : public ::testing::Test {

protected:
    Stack<int> stack;
    Stack<int> stack2;

    void SetUp() {
        // Initialize stack2
        for(int i = 0; i < 10 ; i++) {
            stack2.push(i);
        }
    }

    void TearDown() {
        // Nothing here
    }
};

TEST_F(StackTest, push) {
    // Exercise
    stack.push(5);
    stack.push(13);

    // Verify
    ASSERT_EQ(2, stack.count());
    ASSERT_TRUE(stack.find(5));
}

TEST_F(StackTest, pop) {
    // Exercise
    int head = stack2.pop();

    // Verify
    ASSERT_EQ(9, head);
    ASSERT_EQ(9, stack2.count());
}

TEST_F(StackTest, top) {
    // Exercise
    stack.push(5);

    // Verify
    ASSERT_EQ(5, stack.peek());
    ASSERT_EQ(9, stack2.peek());
}

TEST_F(StackTest, empty) {
    ASSERT_TRUE(stack.empty());
    ASSERT_FALSE(stack2.empty());
}

TEST_F(StackTest, count) {
    ASSERT_EQ(0, stack.count());
    ASSERT_EQ(10, stack2.count());
}

TEST_F(StackTest, find) {
    ASSERT_FALSE(stack.find(5));
    ASSERT_TRUE(stack2.find(5));
}

TEST_F(StackTest, elements) {
    // Setup
    const int elementOne = 345;
    const int elementTwo = 355;
    stack.push(elementOne);
    stack.push(elementTwo);

    // Exercise
    auto_ptr<Generator<int> > g = stack.elements();

    // Verify
    ASSERT_TRUE(g->hasNext());
    ASSERT_EQ(elementOne, g->next());
    ASSERT_TRUE(g->hasNext());
    ASSERT_EQ(elementTwo, g->next());
    ASSERT_FALSE(g->hasNext());
}

} // namespace util
