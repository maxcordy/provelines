/*
 * NeverClaimParserTest.cpp
 *
 * Created on 2013-08-02
 */

#include "gtest/gtest.h"

#include "util/parser/BoolExp.hpp"
#include "util/parser/BoolValue.hpp"
#include "util/parser/Var.hpp"
#include "util/parser/NotExp.hpp"
#include "util/parser/OrExp.hpp"
#include "util/parser/AndExp.hpp"
#include "neverclaim/NeverClaimParser.hpp"
#include "neverclaim/NeverClaim.hpp"

#include <stdexcept>
#include <tr1/memory>

using namespace util::parser;
using std::endl;
using std::logic_error;
using std::string;
using std::stringstream;
using std::tr1::shared_ptr;

namespace neverclaim {

/**
 * Unit tests for the NeverClaimParser class.
 *
 * @author mwi
 */
class NeverClaimParserTest : public ::testing::Test {

protected:
    ClaimTransitionPtr claimOneTrans1;
    ClaimTransitionPtr claimOneTrans2;

    ClaimTransitionPtr claimTwoTrans1;

    ClaimTransitionPtr claimThreeTrans1;
    ClaimTransitionPtr claimThreeTrans2;
    ClaimTransitionPtr claimThreeTrans3;

    ClaimTransitionPtr claimFourTrans1;
    ClaimTransitionPtr claimFourTrans2;

    void SetUp() {
        shared_ptr<BoolExp> trueVal(new BoolValue(true));
        shared_ptr<BoolExp> p1(new Var("p1"));
        shared_ptr<BoolExp> notP1(new NotExp(p1));
        shared_ptr<BoolExp> t2(new Var("t2"));
        shared_ptr<BoolExp> notT2(new NotExp(t2));
        shared_ptr<BoolExp> p4(new Var("p4"));
        shared_ptr<BoolExp> notP4(new NotExp(p4));
        shared_ptr<BoolExp> f3(new Var("f3"));
        shared_ptr<BoolExp> notF3(new NotExp(f3));
        shared_ptr<BoolExp> notP4OrNotF3(new OrExp(notP4, notF3));
        shared_ptr<BoolExp> notF3AndP4(new AndExp(notF3, p4));
        BoolExpPtr fCPowerUp(new Var("fCPowerUp"));
        BoolExpPtr notFCPowerUp(new NotExp(fCPowerUp));
        BoolExpPtr coastCommand(new Var("coastCommand"));
        BoolExpPtr notFCPowerUpAndCoastCommand(new AndExp(notFCPowerUp,
                coastCommand));

        claimOneTrans1 = ClaimTransitionPtr(new ClaimTransition("T0_init",
                "T0_init", trueVal));
        claimOneTrans2 = ClaimTransitionPtr(new ClaimTransition("T0_init",
                "accept_all", notP1));
        claimTwoTrans1 = ClaimTransitionPtr(new ClaimTransition("accept_init",
                "accept_init", notT2));
        claimThreeTrans1 = ClaimTransitionPtr(new ClaimTransition("T0_init",
                "T0_init", trueVal));
        claimThreeTrans2 = ClaimTransitionPtr(new ClaimTransition("T0_init",
                "accept_S2", notP4OrNotF3));
        claimThreeTrans3 = ClaimTransitionPtr(new ClaimTransition("accept_S2",
                "accept_S2", notF3AndP4));
        claimFourTrans1 = ClaimTransitionPtr(new ClaimTransition("T0_init",
                "T0_init", trueVal));
        claimFourTrans2 = ClaimTransitionPtr(new ClaimTransition("T0_init",
                "accept_all", notFCPowerUpAndCoastCommand));
    }

    void TearDown() {
        // Nothing here
    }

    string getNeverClaimOne() {
        stringstream claim;
        claim << "never { /* !([]p1) */"             << endl
              << "T0_init:"                          << endl
              << "	if"                          << endl
              << "	:: (1) -> goto T0_init"      << endl
              << "	:: (!p1) -> goto accept_all" << endl
              << "	fi;"                         << endl
              << "accept_all:"                       << endl
              << "	skip"                        << endl
              << "}"                                 << endl;
        return claim.str();
    }

    string getNeverClaimTwo() {
        stringstream claim;
        claim << "never { /* !(<>t2) */"              << endl
              << "accept_init:"                       << endl
              << "	if"                           << endl
              << "	:: (!t2) -> goto accept_init" << endl
              << "	fi;"                          << endl
              << "}";
        return claim.str();
    }

    string getNeverClaimThree() {
        stringstream claim;
        claim << "never { /* !([]<>(f3 && p4)) */"           << endl
              << "T0_init:"                                  << endl
              << "	if"                                  << endl
              << "	:: (1) -> goto T0_init"              << endl
              << "	:: (!p4) || (!f3) -> goto accept_S2" << endl
              << "	fi;"                                 << endl
              << "accept_S2:"                                << endl
              << "	if"                                  << endl
              << "	:: (!f3) && p4 -> goto accept_S2"    << endl
              << "	fi;"                                 << endl
              << "}"                                         << endl;
        return claim.str();
    }


    string getNeverClaimFour() {
        stringstream claim;
        claim << "never { /* !([] (coastCommand -> fCPowerUp)) */" << endl
              << "T0_init:"                                        << endl
              << "        if"                                      << endl
              << "        :: (1) -> goto T0_init"                  << endl
              << "        :: (!fCPowerUp && coastCommand) -> goto accept_all"
              << endl
              << "        fi;"                                     << endl
              << "accept_all:"                                     << endl
              << "        skip"                                    << endl
              << "}"                                               << endl;
        return claim.str();
    }

    string getNeverClaimUppercase() {
        stringstream claim;
        claim << "never { /* !(State) */"               << endl
              << "accept_init:"                         << endl
              << "	if"                             << endl
              << "	:: (!State) -> goto accept_all" << endl
              << "	fi;"                            << endl
              << "accept_all:"                          << endl
              << "	skip"                           << endl
              << "}"                                    << endl;
        return claim.str();
    }

    string getNeverClaimEmptyLtlFormula() {
        stringstream claim;
        claim << "never { /**/"                         << endl
              << "accept_init:"                         << endl
              << "	if"                             << endl
              << "	:: (!state) -> goto accept_all" << endl
              << "	fi;"                            << endl
              << "accept_all:"                          << endl
              << "	skip"                           << endl
              << "}"                                    << endl;
        return claim.str();
    }
};

TEST_F(NeverClaimParserTest, parse_claimOne) {
    // Setup
    NeverClaimParser parser(getNeverClaimOne());

    // Exercise
    shared_ptr<NeverClaim> neverClaim = parser.parse();

    // Verify
    ASSERT_EQ(claimOneTrans1->getSource(), neverClaim->getInitialState());
    ClaimTransitionGeneratorPtr g = neverClaim->executables(
            *claimOneTrans1->getSource());
    ASSERT_EQ(claimOneTrans1->getTarget(), g->next()->getTarget());
    ASSERT_EQ(claimOneTrans2->getTarget(), g->next()->getTarget());
    ASSERT_FALSE(g->hasNext());
}

TEST_F(NeverClaimParserTest, parse_claimTwo) {
    // Setup
    NeverClaimParser parser(getNeverClaimTwo());

    // Exercise
    shared_ptr<NeverClaim> neverClaim = parser.parse();

    // Verify
    ASSERT_EQ(claimTwoTrans1->getSource(), neverClaim->getInitialState());
    ClaimTransitionGeneratorPtr g = neverClaim->executables(
            *claimTwoTrans1->getSource());
    ASSERT_EQ(claimTwoTrans1->getTarget(), g->next()->getTarget());
    ASSERT_FALSE(g->hasNext());
}

TEST_F(NeverClaimParserTest, parse_claimThree) {
    // Setup
    NeverClaimParser parser(getNeverClaimThree());

    // Exercise
    shared_ptr<NeverClaim> neverClaim = parser.parse();

    // Verify
    ASSERT_EQ(claimThreeTrans1->getSource(), neverClaim->getInitialState());
    ClaimTransitionGeneratorPtr g = neverClaim->executables(
            *claimThreeTrans1->getSource());
    ASSERT_EQ(claimThreeTrans1->getTarget(), g->next()->getTarget());
    ASSERT_EQ(claimThreeTrans2->getTarget(), g->next()->getTarget());
    ASSERT_FALSE(g->hasNext());
    g = neverClaim->executables(*claimThreeTrans3->getSource());
    ASSERT_EQ(claimThreeTrans3->getTarget(), g->next()->getTarget());
    ASSERT_FALSE(g->hasNext());
}

TEST_F(NeverClaimParserTest, parse_claimFour) {
    // Setup
    NeverClaimParser parser(getNeverClaimFour());

    // Exercise
    NeverClaimPtr neverClaim = parser.parse();

    // Verify
    ASSERT_EQ(claimFourTrans1->getSource(), neverClaim->getInitialState());
    ClaimTransitionGeneratorPtr g = neverClaim->executables(
            *claimFourTrans1->getSource());
    ASSERT_TRUE(g->hasNext());
    ASSERT_EQ(claimFourTrans1->getTarget(), g->next()->getTarget());
    ASSERT_TRUE(g->hasNext());
    ASSERT_EQ(claimFourTrans2->getTarget(), g->next()->getTarget());
    ASSERT_FALSE(g->hasNext());
}

/**
 * Tests the parser with a never claim that contains a state starting with an
 * uppercase letter.
 */
TEST_F(NeverClaimParserTest, parse_uppercase) {
    // Setup
    NeverClaimParser parser(getNeverClaimUppercase());

    // Exercise
    ASSERT_THROW(parser.parse(), logic_error);
}

/**
 * Tests the parser with a never claim that contains an empty LTL formula in
 * comments.
 */
TEST_F(NeverClaimParserTest, parse_emptyLtlFormula) {
    // Setup
    NeverClaimParser parser(getNeverClaimEmptyLtlFormula());

    // Exercise
    ASSERT_THROW(parser.parse(), logic_error);
}

} // namespace neverclaim
