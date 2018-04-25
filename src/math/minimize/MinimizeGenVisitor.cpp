/*
 * MinimizeGenVisitor.cpp
 *
 * Created on 2013-08-29
 */

#include "MinimizeGenVisitor.hpp"

#include "VarTable.hpp"
#include "util/Assert.hpp"
#include "util/Stack.hpp"
#include "util/parser/AndExp.hpp"
#include "util/parser/BoolValue.hpp"
#include "util/parser/NotExp.hpp"
#include "util/parser/OrExp.hpp"
#include "util/parser/Var.hpp"

#pragma GCC diagnostic ignored "-Wunused-parameter"

using std::string;
using util::parser::AndExp;
using util::parser::BoolValue;
using util::parser::NotExp;
using util::parser::OrExp;
using util::parser::Var;

namespace math {
namespace minimize {

// constructors
MinimizeGenVisitor::MinimizeGenVisitor()
        : expressions(), varTable(new VarTable()), aborted(false) {
    checkRep();
}

// public methods
string MinimizeGenVisitor::getMinimizeExp() const {
    assertTrue(!expressions.empty());

    return expressions.peek();
}

const VarTablePtr MinimizeGenVisitor::getVarTable() const {
    return varTable;
}

bool MinimizeGenVisitor::isAborted() const {
    return aborted;
}

void MinimizeGenVisitor::orExp(const OrExp & exp) {
    assertTrue(expressions.count() >= 2);

    string rhs = expressions.pop();
    string lhs = expressions.pop();
    expressions.push("(" + rhs + ")+(" + lhs + ")");

    checkRep();
}

void MinimizeGenVisitor::andExp(const AndExp & exp) {
    assertTrue(expressions.count() >= 2);

    string rhs = expressions.pop();
    string lhs = expressions.pop();
    expressions.push("(" + rhs + ")(" + lhs + ")");

    checkRep();
}

void MinimizeGenVisitor::notExp(const NotExp & exp) {
    assertTrue(expressions.count() >= 1);

    string expression = expressions.pop();
    expressions.push("(" + expression + ")'");

    checkRep();
}

void MinimizeGenVisitor::boolValue(const BoolValue & val) {
    if (val.getValue()) {
        expressions.push("1");
    } else {
        expressions.push("0");
    }

    checkRep();
}

void MinimizeGenVisitor::var(const Var & v) {
    string variable;
    if (varTable->isFull()) {
        aborted = true;
        // A dummy name is used so the visit can continue without failing.
        variable = "dummy";
    } else {
        if (!varTable->isIn(v.getName())) {
            varTable->add(v.getName());
        }
        variable += varTable->get(v.getName());
    }
    expressions.push(variable);
}

// private methods
void MinimizeGenVisitor::checkRep() const {
    if (doCheckRep) {
        assertTrue(varTable);
    }
}

} // namespace minimize
} // namespace math
