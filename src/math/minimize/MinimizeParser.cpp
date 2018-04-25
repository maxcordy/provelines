/*
 * MinimizeParser.cpp
 *
 * Created on 2013-08-29
 */

#include "MinimizeParser.hpp"

#include "VarTable.hpp"
#include "util/Assert.hpp"

#include <sstream>
#include <stdexcept>

using std::logic_error;
using std::ostringstream;
using std::string;

namespace math {
namespace minimize {

// constructors
MinimizeParser::MinimizeParser(const string & minimize, const VarTablePtr &
        varTable) : AbstractParser(minimize), varTable(varTable) {
    checkRep();
}

// public methods
string MinimizeParser::parse() {
    string result;

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
    checkRep();
    return result;
}

bool MinimizeParser::boolExp(string & exp) {
    assertTrue(exp.length() == 0);
    if (!orExp(exp)) {
        return false;
    }
    assertTrue(exp.length() > 0);
    return true;
}

bool MinimizeParser::orExp(string & exp) {
    assertTrue(exp.length() == 0);
    if (!andExp(exp)) {
        return false;
    }
    while (expect("+")) {
        string rhs;
        if (!andExp(rhs)) {
            return false;
        }
        exp += " || " + rhs;
    }
    assertTrue(exp.length() > 0);
    return true;
}

bool MinimizeParser::andExp(string & exp) {
    assertTrue(exp.length() == 0);
    if (!notExp(exp)) {
        return false;
    }
    while (expect("*") || isVariable()) {
        string rhs;
        if (!notExp(rhs)) {
            return false;
        }
        exp += " && " + rhs;
    }
    assertTrue(exp.length() > 0);
    return true;
}

bool MinimizeParser::notExp(string & exp) {
    assertTrue(exp.length() == 0);
    if (!atomExp(exp)) {
        return false;
    }
    if (expect("'")) {
        exp = "!" + exp;
    }
    assertTrue(exp.length() > 0);
    return true;
}

bool MinimizeParser::atomExp(string & exp) {
    assertTrue(exp.length() == 0);
    if (!expect("(")) {
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
    if (!expect(")")) {
        return false;
    }
    exp = "(" + exp + ")";
    assertTrue(exp.length() > 0);
    return true;
}

bool MinimizeParser::value(string & val) {
    assertTrue(val.length() == 0);
    if (expect("1")) {
        val = "true";
    } else if (expect("0")) {
        val = "false";
    } else {
        return false;
    }
    assertTrue(val.length() > 0);
    return true;
}

bool MinimizeParser::variable(string & var) {
    assertTrue(var.length() == 0);
    if (!(isLowercase() || isUppercase())) {
        return false;
    }
    if (!varTable->isIn(getNextSymbol())) {
        errorVarNotFound(getNextSymbol());
        return false;
    }
    var = varTable->get(getNextSymbol());

    string nextSymbol;
    nextSymbol += getNextSymbol();
    appendToParsedInput(nextSymbol);
    advanceCursor();

    assertTrue(var.length() > 0);
    return true;
}

bool MinimizeParser::isVariable() const {
    return isUppercase() || isLowercase();
}

void MinimizeParser::errorVarNotFound(const char & var) {
    ostringstream oss;
    oss << "Error: variable '" << var
        << "' does not exist in the variable table";
    errorMessage = oss.str();
}

void MinimizeParser::error() {
    ostringstream oss;
    oss << "Syntax Error: unexpected " ;
    if (hasNextSymbol()) {
        oss << "symbol '" << getNextSymbol() << "' ";
    } else {
        oss << "end ";
    }
    oss << "in Minimize expression '" << getInput() << "' at position "
        << getPosition() << ".";
    errorMessage = oss.str();
}

// private methods
void MinimizeParser::checkRep() const {
    if (doCheckRep) {
        AbstractParser::checkRep();
        assertTrue(varTable);
    }
}

} // minimize
} // math
