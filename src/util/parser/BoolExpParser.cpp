/*
 * BoolExpParser.cpp
 *
 * Created on 2013-07-23
 */

#include "BoolExpParser.hpp"

#include "AndExp.hpp"
#include "BoolValue.hpp"
#include "NotExp.hpp"
#include "OrExp.hpp"
#include "Var.hpp"
#include "util/Assert.hpp"

#include <sstream>
#include <stdexcept>

using std::logic_error;
using std::ostringstream;
using std::string;

namespace util {
namespace parser {

// terminals
const string BoolExpParser::TerminalAnd("&&");
const string BoolExpParser::TerminalOr("||");
const string BoolExpParser::TerminalNot("!");
const string BoolExpParser::TerminalLeftParen("(");
const string BoolExpParser::TerminalRightParen(")");

BoolExpParser::BoolExpParser(const string & boolExp)
        : AbstractParser(boolExp), errorMessage() {
    checkRep();
}

BoolExpPtr BoolExpParser::parse() {
    if (getInput().length() == 0) {
        checkRep();
        return BoolExpPtr(new BoolValue(true));
    }

    BoolExpPtr result;

    if (!boolExp(result)) {
        checkRep();
        throw logic_error(errorMessage);
    }

    if (!isEndOfInputReach()) {
        checkRep();
        throw logic_error(
                "The end of the input has not been reached while parsing");
    }

    assertTrue(getInput() == getParsedInput());
    assertTrue(result);
    checkRep();
    return result;
}

bool BoolExpParser::boolExp(BoolExpPtr & exp) {
    assertTrue(!exp);
    if (!logicalOr(exp)) {
        return false;
    }
    assertTrue(exp);
    return true;
}

bool BoolExpParser::logicalOr(BoolExpPtr & exp) {
    assertTrue(!exp);
    if (!logicalAnd(exp)) {
        return false;
    }
    while (expect(TerminalOr)) {
        BoolExpPtr rhs;
        if (!logicalAnd(rhs)) {
            return false;
        }
        exp = BoolExpPtr(new OrExp(exp, rhs));
    }
    assertTrue(exp);
    return true;
}

bool BoolExpParser::logicalAnd(BoolExpPtr & exp) {
    assertTrue(!exp);
    if (!prefixOp(exp)) {
        return false;
    }
    while (expect(TerminalAnd)) {
        BoolExpPtr rhs;
        if (!prefixOp(rhs)) {
            return false;
        }
        exp = BoolExpPtr(new AndExp(exp, rhs));
    }
    assertTrue(exp);
    return true;
}

bool BoolExpParser::prefixOp(BoolExpPtr & exp) {
    assertTrue(!exp);
    if (!expect(TerminalNot)) {
        if (!atomic(exp)) {
            return false;
        }
        return true;
    }
    if (!expect(TerminalLeftParen)) {
        return false;
    }
    if (!boolExp(exp)) {
        return false;
    }
    if (!expect(TerminalRightParen)) {
        return false;
    }
    exp = BoolExpPtr(new NotExp(exp));
    assertTrue(exp);
    return true;
}

bool BoolExpParser::atomic(BoolExpPtr & exp) {
    assertTrue(!exp);
    if (!expect(TerminalLeftParen)) {
        if (!value(exp)) {
            if (!variable(exp)) {
                return false;
            }
            return true;
        }
        return true;
    }
    if (!boolExp(exp)) {
        return false;
    }
    if (!expect(TerminalRightParen)) {
        return false;
    }
    assertTrue(exp);
    return true;
}

bool BoolExpParser::value(BoolExpPtr & val) {
    assertTrue(!val);
    bool boolValue = true;
    if (!(expect("true") || expect("1"))) {
        if (!(expect("false") || expect("0"))) {
            return false;
        }
        boolValue = false;
    }
    val = BoolExpPtr(new BoolValue(boolValue));
    return true;
}

bool BoolExpParser::variable(BoolExpPtr & var) {
    assertTrue(!var);
    string featureName;

    while (true) {
        if (!(isLowercase() || isUppercase() || isUnderscore())) {
            if (!isDigit()) {
                break;
            } else if (featureName.length() <= 0) {
                error();
                return false;
            }
        }
        featureName += getNextSymbol();
        advanceCursor();
    }

    if (featureName.length() <= 0) {
        return false;
    }
    var = BoolExpPtr(new Var(featureName));
    appendToParsedInput(featureName);
    return true;
}

void BoolExpParser::error() {
    ostringstream oss;
    oss << "Syntax Error: unexpected " ;
    if (hasNextSymbol()) {
        oss << "symbol '" << getNextSymbol() << "' ";
    } else {
        oss << "end ";
    }
    oss << "in Boolean expression '" << getInput() << "' at position "
        << getPosition() << ".";
    errorMessage = oss.str();
}

void BoolExpParser::checkRep() const {
    if (doCheckRep) {
        AbstractParser::checkRep();
    }
}

} // namespace parser
} // namespace util
