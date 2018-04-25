/*
 * FstmEvalVisitorTest.cpp
 *
 * Created on 2013-08-16
 */

#include "gtest/gtest.h"

#include "core/fts/fstm/FstmEvalVisitor.hpp"
#include "util/parser/AndExp.hpp"
#include "util/parser/BoolExp.hpp"
#include "util/parser/BoolValue.hpp"
#include "util/parser/EvalVisitor.hpp"
#include "util/parser/NotExp.hpp"
#include "util/parser/OrExp.hpp"
#include "util/parser/Var.hpp"

#include <tr1/memory>

using namespace util::parser;
using std::string;
using std::tr1::shared_ptr;

namespace core {
namespace fts {
namespace fstm {

/**
 * Unit tests for the FstmEvalVisitor class.
 *
 * @author mwi
 */
class FstmEvalVisitorTest : public ::testing::Test {

protected:
    shared_ptr<EvalVisitor> target;
    string sameState;
    string differentState;
    string sameEvent;
    string differentEvent;

    void SetUp() {
        sameState = "myState";
        differentState = "myState2";
        sameEvent = "myEvent";
        differentEvent = "myEvent2";
        target = shared_ptr<FstmEvalVisitor>(
                new FstmEvalVisitor(sameState, sameEvent));
    }

    void TearDown() {
        // Nothing here
    }
};

TEST_F(FstmEvalVisitorTest, isTrue_sameState) {
    // Setup
    shared_ptr<BoolExp> var = shared_ptr<Var>(new Var(sameState));

    // Exercise
    var->accept(*target);

    // Verify
    ASSERT_TRUE(target->isTrue());
}

TEST_F(FstmEvalVisitorTest, isTrue_differentState) {
    // Setup
    shared_ptr<BoolExp> var = shared_ptr<Var>(new Var(differentState));

    // Exercise
    var->accept(*target);

    // Verify
    ASSERT_FALSE(target->isTrue());
}

TEST_F(FstmEvalVisitorTest, isTrue_sameEvent) {
    // Setup
    shared_ptr<BoolExp> var = shared_ptr<Var>(new Var(sameEvent));

    // Exercise
    var->accept(*target);

    // Verify
    ASSERT_TRUE(target->isTrue());
}

TEST_F(FstmEvalVisitorTest, isTrue_differentEvent) {
    // Setup
    shared_ptr<BoolExp> var = shared_ptr<Var>(new Var(differentEvent));

    // Exercise
    var->accept(*target);

    // Verify
    ASSERT_FALSE(target->isTrue());
}

TEST_F(FstmEvalVisitorTest, isTrue_trueBoolVal) {
    // Setup
    shared_ptr<BoolExp> value = shared_ptr<BoolValue>(new BoolValue(true));

    // Exercise
    value->accept(*target);

    // Verify
    ASSERT_TRUE(target->isTrue());
}

TEST_F(FstmEvalVisitorTest, isTrue_falseBoolVal) {
    // Setup
    shared_ptr<BoolExp> value = shared_ptr<BoolValue>(new BoolValue(false));

    // Exercise
    value->accept(*target);

    // Verify
    ASSERT_FALSE(target->isTrue());
}

TEST_F(FstmEvalVisitorTest, isTrue_notExp_true) {
    // Setup
    shared_ptr<BoolExp> var = shared_ptr<Var>(new Var(differentState));
    shared_ptr<BoolExp> notExp = shared_ptr<NotExp>(new NotExp(var));

    // Exercise
    notExp->accept(*target);

    // Verify
    ASSERT_TRUE(target->isTrue());
}

TEST_F(FstmEvalVisitorTest, isTrue_notExp_false) {
    // Setup
    shared_ptr<BoolExp> var = shared_ptr<Var>(new Var(sameState));
    shared_ptr<BoolExp> notExp = shared_ptr<NotExp>(new NotExp(var));

    // Exercise
    notExp->accept(*target);

    // Verify
    ASSERT_FALSE(target->isTrue());
}

TEST_F(FstmEvalVisitorTest, isTrue_orExp_true) {
    // Setup
    shared_ptr<BoolExp> lhs = shared_ptr<Var>(new Var(sameState));
    shared_ptr<BoolExp> rhs = shared_ptr<BoolValue>(new BoolValue(false));
    shared_ptr<BoolExp> orExp = shared_ptr<OrExp>(new OrExp(lhs, rhs));

    // Exercise
    orExp->accept(*target);

    // Verify
    ASSERT_TRUE(target->isTrue());
}

TEST_F(FstmEvalVisitorTest, isTrue_orExp_false) {
    // Setup
    shared_ptr<BoolExp> lhs = shared_ptr<Var>(new Var(differentState));
    shared_ptr<BoolExp> rhs = shared_ptr<BoolValue>(new BoolValue(false));
    shared_ptr<BoolExp> orExp = shared_ptr<OrExp>(new OrExp(lhs, rhs));

    // Exercise
    orExp->accept(*target);

    // Verify
    ASSERT_FALSE(target->isTrue());
}

TEST_F(FstmEvalVisitorTest, isTrue_andExp_true) {
    // Setup
    shared_ptr<BoolExp> lhs = shared_ptr<Var>(new Var(sameState));
    shared_ptr<BoolExp> rhs = shared_ptr<BoolValue>(new BoolValue(true));
    shared_ptr<BoolExp> andExp = shared_ptr<AndExp>(new AndExp(lhs, rhs));

    // Exercise
    andExp->accept(*target);

    // Verify
    ASSERT_TRUE(target->isTrue());
}

TEST_F(FstmEvalVisitorTest, isTrue_andExp_false) {
    // Setup
    shared_ptr<BoolExp> lhs = shared_ptr<Var>(new Var(sameState));
    shared_ptr<BoolExp> rhs = shared_ptr<BoolValue>(new BoolValue(false));
    shared_ptr<BoolExp> andExp = shared_ptr<AndExp>(new AndExp(lhs, rhs));

    // Exercise
    andExp->accept(*target);

    // Verify
    ASSERT_FALSE(target->isTrue());
}

TEST_F(FstmEvalVisitorTest, reset) {
    // Setup
    shared_ptr<BoolExp> var = shared_ptr<Var>(new Var(sameState));
    var->accept(*target);

    // Exercise
    target->reset();

    // Verify
    shared_ptr<BoolExp> notExp = shared_ptr<NotExp>(new NotExp(var));
    notExp->accept(*target);
    ASSERT_FALSE(target->isTrue());
}

/**
 * Tests the visitor on a Boolean expression with more than one NOT expression
 * (regression test).
 */
TEST_F(FstmEvalVisitorTest, multipleNot) {
    // Setup
    shared_ptr<BoolExp> var1 = shared_ptr<Var>(new Var("state1"));
    shared_ptr<BoolExp> notVar1 = shared_ptr<NotExp>(new NotExp(var1));
    shared_ptr<BoolExp> var2 = shared_ptr<Var>(new Var("state2"));
    shared_ptr<BoolExp> notVar2 = shared_ptr<NotExp>(new NotExp(var2));
    shared_ptr<BoolExp> andExp =
            shared_ptr<AndExp>(new AndExp(notVar1, notVar2));

    // Exercise
    andExp->accept(*target);

    // Verify
    ASSERT_TRUE(target->isTrue());
}

/**
 * Tests the visitor on a Boolean expression with more than one AND expression
 * (regression test).
 */
TEST_F(FstmEvalVisitorTest, multipleAnd) {
    // Setup
    shared_ptr<BoolExp> lhs = shared_ptr<Var>(new Var(sameState));
    shared_ptr<BoolExp> rhs = shared_ptr<BoolValue>(new BoolValue(true));
    shared_ptr<BoolExp> andExp1 = shared_ptr<AndExp>(new AndExp(lhs, rhs));
    shared_ptr<BoolExp> andExp2 = shared_ptr<AndExp>(new AndExp(lhs, rhs));
    shared_ptr<BoolExp> andExp3 =
            shared_ptr<AndExp>(new AndExp(andExp1, andExp2));

    // Exercise
    andExp3->accept(*target);

    // Verify
    ASSERT_TRUE(target->isTrue());
}

/**
 * Tests the visitor on a Boolean expression with more than one OR expression
 * (regression test).
 */
TEST_F(FstmEvalVisitorTest, multipleOr) {
    // Setup
    shared_ptr<BoolExp> lhs = shared_ptr<Var>(new Var(sameState));
    shared_ptr<BoolExp> rhs = shared_ptr<BoolValue>(new BoolValue(false));
    shared_ptr<BoolExp> orExp1 = shared_ptr<OrExp>(new OrExp(lhs, rhs));
    shared_ptr<BoolExp> orExp2 = shared_ptr<OrExp>(new OrExp(lhs, rhs));
    shared_ptr<BoolExp> orExp3 = shared_ptr<OrExp>(new OrExp(orExp1, orExp2));

    // Exercise
    orExp3->accept(*target);

    // Verify
    ASSERT_TRUE(target->isTrue());
}

/**
 * Tests the visitor on a Boolean expression made up of one state and one event.
 */
TEST_F(FstmEvalVisitorTest, stateAndEvent) {
    // Setup
    BoolExpPtr fCPowerUp(new Var("fCPowerUp"));
    BoolExpPtr notFCPowerUp(new NotExp(fCPowerUp));
    BoolExpPtr coastCommand(new Var("coastCommand"));
    BoolExpPtr notFCPowerUpAndCoastCommand(new AndExp(notFCPowerUp,
            coastCommand));
    FstmEvalVisitor visitor("fCMotorStopped", "coastCommand");

    // Exercise
    notFCPowerUpAndCoastCommand->accept(visitor);

    // Verify
    ASSERT_TRUE(visitor.isTrue());
}

} // namespace fstm
} // namespace fts
} // namespace core
