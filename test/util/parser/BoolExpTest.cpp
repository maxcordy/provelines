/*
 * BoolExpTest.cpp
 *
 * Created on: 2013-08-14
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "util/parser/AndExp.hpp"
#include "util/parser/BoolExp.hpp"
#include "util/parser/BoolValue.hpp"
#include "util/parser/NotExp.hpp"
#include "util/parser/OrExp.hpp"
#include "util/parser/Var.hpp"
#include "util/parser/Visitor.hpp"

using namespace util::parser;
using ::testing::_;
using ::testing::InSequence;
using std::string;
using std::tr1::shared_ptr;

namespace util {
namespace parser {

/**
 * Unit tests for the BoolExp class hierarchy.
 *
 * @author mwi
 */
class BoolExpTest : public ::testing::Test {

protected:

    void SetUp() {}

    void TearDown() {}
};

class MockVisitor : public Visitor {
public:
    MOCK_METHOD1(orExp, void(const OrExp & exp));
    MOCK_METHOD1(andExp, void(const AndExp & exp));
    MOCK_METHOD1(notExp, void(const NotExp & exp));
    MOCK_METHOD1(boolValue, void(const BoolValue & val));
    MOCK_METHOD1(var, void(const Var & v));
};

TEST_F(BoolExpTest, var) {
    // Setup
    const string name("MyVar");

    // Exercise
    Var var(name);

    // Verify
    ASSERT_EQ(name, var.getName());
}

TEST_F(BoolExpTest, notExp) {
    // Setup
    shared_ptr<BoolExp> boolExp(new Var("MyVar"));

    // Exercise
    NotExp notExp(boolExp);

    // Verify
    ASSERT_EQ(boolExp, notExp.getBoolExp());
}

TEST_F(BoolExpTest, value_true) {
    // Setup
    BoolValue value(true);

    // Exercise & Verify
    ASSERT_TRUE(value.getValue());
}

TEST_F(BoolExpTest, value_false) {
    // Setup
    BoolValue value(false);

    // Exercise & Verify
    ASSERT_FALSE(value.getValue());
}

TEST_F(BoolExpTest, orExp) {
    // Setup
    shared_ptr<BoolExp> lhs(new Var("MyVar"));
    shared_ptr<BoolExp> rhs(new BoolValue(false));

    // Exercise
    OrExp orExp(lhs, rhs);

    // Verify
    ASSERT_EQ(lhs, orExp.getLeftExp());
    ASSERT_EQ(rhs, orExp.getRightExp());
}

TEST_F(BoolExpTest, andExp) {
    // Setup
    shared_ptr<BoolExp> var(new Var("MyVar"));
    shared_ptr<BoolExp> trueVal(new BoolValue(true));
    shared_ptr<BoolExp> lhs(new NotExp(var));
    shared_ptr<BoolExp> rhs(new OrExp(var, trueVal));

    // Exercise
    AndExp andExp(lhs, rhs);

    // Verify
    ASSERT_EQ(lhs, andExp.getLeftExp());
    ASSERT_EQ(rhs, andExp.getRightExp());
}

TEST_F(BoolExpTest, var_visitor) {
    // Setup
    Var target("MyVar");
    MockVisitor visitor;

    // Configure
    EXPECT_CALL(visitor, var(_)).Times(1);

    // Exercise
    target.accept(visitor);
}

TEST_F(BoolExpTest, boolValue_visitor) {
    // Setup
    BoolValue target(true);
    MockVisitor visitor;

    // Configure
    EXPECT_CALL(visitor, boolValue(_)).Times(1);

    // Exercise
    target.accept(visitor);
}

TEST_F(BoolExpTest, notExp_visitor) {
    // Setup
    shared_ptr<Var> boolExp(new Var("MyVar"));
    NotExp target(boolExp);
    MockVisitor visitor;

    // Configure
    {
        InSequence sequence;
        EXPECT_CALL(visitor, var(_)).Times(1);
        EXPECT_CALL(visitor, notExp(_)).Times(1);
    }

    // Exercise
    target.accept(visitor);
}

TEST_F(BoolExpTest, orExp_visitor) {
    // Setup
    shared_ptr<Var> lhs(new Var("MyVar"));
    shared_ptr<BoolValue> rhs(new BoolValue(true));
    OrExp target(lhs, rhs);
    MockVisitor visitor;

    // Configure
    {
        InSequence sequence;
        EXPECT_CALL(visitor, var(_)).Times(1);
        EXPECT_CALL(visitor, boolValue(_)).Times(1);
        EXPECT_CALL(visitor, orExp(_)).Times(1);
    }

    // Exercise
    target.accept(visitor);
}

TEST_F(BoolExpTest, andExp_visitor) {
    // Setup
    shared_ptr<BoolExp> var(new Var("MyVar"));
    shared_ptr<BoolExp> lhs(new NotExp(var));
    shared_ptr<BoolExp> rhs(new BoolValue(true));
    AndExp target(lhs, rhs);
    MockVisitor visitor;

    // Configure
    {
        InSequence sequence;
        EXPECT_CALL(visitor, var(_)).Times(1);
        EXPECT_CALL(visitor, notExp(_)).Times(1);
        EXPECT_CALL(visitor, boolValue(_)).Times(1);
        EXPECT_CALL(visitor, andExp(_)).Times(1);
    }

    // Exercise
    target.accept(visitor);
}

} // namespace parser
} // namespace util
