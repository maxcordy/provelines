/*
 * NeverClaimParser.cpp
 *
 * Created on 2013-08-04
 */

#include "NeverClaimParser.hpp"

#include "NeverClaim.hpp"
#include "util/Assert.hpp"
#include "util/parser/AndExp.hpp"
#include "util/parser/BoolExp.hpp"
#include "util/parser/BoolValue.hpp"
#include "util/parser/NotExp.hpp"
#include "util/parser/OrExp.hpp"
#include "util/parser/Var.hpp"

#include <glog/logging.h>

#include <sstream>
#include <stdexcept>
#include <tr1/memory>

using std::logic_error;
using std::ostringstream;
using std::string;
using std::tr1::shared_ptr;
using util::List;
using util::parser::AndExp;
using util::parser::BoolExp;
using util::parser::BoolValue;
using util::parser::NotExp;
using util::parser::OrExp;
using util::parser::Var;

namespace neverclaim {

// constructors
NeverClaimParser::NeverClaimParser(const string & neverClaim)
        : AbstractParser(neverClaim), parsedTransitions(), ltlFormula() {
    checkRep();
}

shared_ptr<NeverClaim> NeverClaimParser::parse() {
    if (!never()) {
        checkRep();
        throw logic_error(errorMessage);
    }

    if (!isEndOfInputReach()) {
        checkRep();
        throw logic_error(
                "The end of the input has not been reached while parsing");
    }

    assertTrue(getInput() == getParsedInput());
    checkRep();

    DLOG(INFO) << "Never claim for LTL formula" << ltlFormula << "parsed";
    return shared_ptr<NeverClaim>(new NeverClaim(parsedTransitions.elements()));
}

// private methods
bool NeverClaimParser::never() {
    if (!expect("never")) {
        return false;
    }
    if (!expect("{")) {
        return false;
    }
    if (!formula()) {
        return false;
    }
    if (!state()) {
        return false;
    }
    while (state()) {}
    if (!expect("}")) {
        return false;
    }
    eatWhitespaces();
    return true;
}

bool NeverClaimParser::formula() {
    if (!expect("/*")) {
        return false;
    }
    if (!ltl()) {
        return false;
    }
    if (!expect("*/")) {
        return false;
    }
    return true;
}

bool NeverClaimParser::state() {
    string sourceState;
    if (!label(sourceState)) {
        return false;
    }
    if (!expect(":")) {
        return false;
    }
    if (!transitions(sourceState)) {
        return false;
    }
    return true;
}

bool NeverClaimParser::transitions(const string & sourceState) {
    if (!expect("if")) {
        if (!expect("skip")) {
            return false;
        }
        return true;
    }
    if (!transition(sourceState)) {
        return false;
    }
    while (transition(sourceState)) {}
    if (!expect("fi")) {
        return false;
    }
    if (!expect(";")) {
        return false;
    }
    return true;
}

bool NeverClaimParser::transition(const string & sourceState) {
    if (!expect("::")) {
        return false;
    }
    shared_ptr<BoolExp> exp;
    if (!boolExp(exp)) {
        return false;
    }
    if (!expect("->")) {
        return false;
    }
    if (!expect("goto")) {
        return false;
    }
    string targetState;
    if (!label(targetState)) {
        return false;
    }
    parsedTransitions.addLast(ClaimTransitionPtr(new ClaimTransition(
            sourceState, targetState, exp)));
    return true;
}

bool NeverClaimParser::boolExp(shared_ptr<BoolExp> & exp) {
    if (!orExp(exp)) {
        return false;
    }
    return true;
}

bool NeverClaimParser::orExp(std::tr1::shared_ptr<BoolExp> & exp) {
    if (!andExp(exp)) {
        return false;
    }
    while (expect("||")) {
        shared_ptr<BoolExp> rhs;
        if (!andExp(rhs)) {
            return false;
        }
        exp = shared_ptr<BoolExp>(new OrExp(exp, rhs));
    }
    return true;
}

bool NeverClaimParser::andExp(std::tr1::shared_ptr<BoolExp> & exp) {
    if (!notExp(exp)) {
        return false;
    }
    while (expect("&&")) {
        shared_ptr<BoolExp> rhs;
        if (!notExp(rhs)) {
            return false;
        }
        exp = shared_ptr<BoolExp>(new AndExp(exp, rhs));
    }
    return true;
}

bool NeverClaimParser::notExp(std::tr1::shared_ptr<BoolExp> & exp) {
    if (!expect("!")) {
        if (!atomExp(exp)) {
            return false;
        }
        return true;
    }
    if (!atomExp(exp)) {
        return false;
    }
    exp = shared_ptr<BoolExp>(new NotExp(exp));
    return true;
}

bool NeverClaimParser::atomExp(std::tr1::shared_ptr<BoolExp> & exp) {
    if (!expect("(")) {
        if (!proposition(exp)) {
            return false;
        }
        return true;
    }
    if (!boolExp(exp)) {
        return false;
    }
    if (!expect(")")) {
        return false;
    }
    return true;
}

bool NeverClaimParser::proposition(std::tr1::shared_ptr<BoolExp> & exp) {
    if (!(applyTrue(exp) || applyFalse(exp) || name(exp))) {
        return false;
    }
    return true;
}

bool NeverClaimParser::applyTrue(std::tr1::shared_ptr<BoolExp> & exp) {
    eatWhitespaces();
    if (!(expect("1") || expect("true"))) {
        return false;
    }
    exp = shared_ptr<BoolExp>(new BoolValue(true));
    return true;
}

bool NeverClaimParser::applyFalse(std::tr1::shared_ptr<BoolExp> & exp) {
    eatWhitespaces();
    if (!(expect("0") || expect("false"))) {
        return false;
    }
    exp = shared_ptr<BoolExp>(new BoolValue(false));
    return true;
}

bool NeverClaimParser::name(std::tr1::shared_ptr<BoolExp> & exp) {
    eatWhitespaces();
    string varName;

    if (!isLowercase()) {
        return false;
    }
    varName += getNextSymbol();
    advanceCursor();

    while (isLowercase() || isUppercase() || isDigit() || isUnderscore()) {
        varName += getNextSymbol();
        advanceCursor();
    }

    appendToParsedInput(varName);
    exp = shared_ptr<BoolExp>(new Var(varName));
    return true;
}

bool NeverClaimParser::label(string & stateLabel) {
    eatWhitespaces();
    if (!(isLowercase() || isUppercase())) {
        return false;
    }
    stateLabel += getNextSymbol();
    advanceCursor();

    while (isLowercase() || isUppercase() || isDigit() || isUnderscore()) {
        stateLabel += getNextSymbol();
        advanceCursor();
    }

    appendToParsedInput(stateLabel);
    return true;
}

bool NeverClaimParser::ltl() {
    while (getNextSymbol() != '*') {
        ltlFormula += getNextSymbol();
        advanceCursor();
    }
    if (ltlFormula.length() <= 0) {
        return false;
    }
    appendToParsedInput(ltlFormula);
    return true;
}

void NeverClaimParser::error() {
    ostringstream oss;
    oss << "Syntax Error: unexpected " ;
    if (hasNextSymbol()) {
        oss << "symbol '" << getNextSymbol() << "' ";
    } else {
        oss << "end ";
    }
    oss << "at position " << getPosition()
        << " in the following never claim\n" << getInput();
    errorMessage = oss.str();
}

void NeverClaimParser::checkRep() const {
    if (doCheckRep) {
        AbstractParser::checkRep();
    }
}

} // namespace neverclaim
