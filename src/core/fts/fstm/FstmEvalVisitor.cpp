/*
 * FstmEvalVisitor.cpp
 *
 * Created on 2013-08-09
 */

#include "FstmEvalVisitor.hpp"

#include "util/parser/AndExp.hpp"
#include "util/parser/BoolValue.hpp"
#include "util/parser/NotExp.hpp"
#include "util/parser/OrExp.hpp"
#include "util/parser/Var.hpp"
#include "util/Assert.hpp"
#include "util/strings.hpp"

#include <cctype>

#pragma GCC diagnostic ignored "-Wunused-parameter"

using util::parser::AndExp;
using util::parser::BoolValue;
using util::parser::NotExp;
using util::parser::OrExp;
using util::parser::Var;
using std::auto_ptr;
using std::string;
using util::Stack;
using util::toLowercaseFirst;

namespace core {
namespace fts {
namespace fstm {

// constructors
FstmEvalVisitor::FstmEvalVisitor(const string & stateName,
        const string & eventName)
        : stateName(stateName), eventName(eventName), evals(new Stack<bool>()) {
    toLowercaseFirst(this->stateName);
    toLowercaseFirst(this->eventName);

    checkRep();
}

void FstmEvalVisitor::reset() {
    evals = auto_ptr<Stack<bool> >(new Stack<bool>());

    checkRep();
}

bool FstmEvalVisitor::isTrue() const {
    assertTrue(!evals->empty());

    return evals->peek();
}

void FstmEvalVisitor::orExp(const OrExp & exp) {
    assertTrue(evals->count() >= 2);

    bool rhsEval = evals->pop(); // the right-hand side evaluation
    bool lhsEval = evals->pop(); // the left-hand side evaluation
    evals->push(lhsEval || rhsEval);

    checkRep();
}

void FstmEvalVisitor::andExp(const AndExp & exp) {
    assertTrue(evals->count() >= 2);

    bool rhsEval = evals->pop(); // the right-hand side evaluation
    bool lhsEval = evals->pop(); // the left-hand side evaluation
    evals->push(lhsEval && rhsEval);

    checkRep();
}

void FstmEvalVisitor::notExp(const NotExp & exp) {
    assertTrue(evals->count() > 0);

    bool expEval = evals->pop();
    evals->push(!expEval);

    checkRep();
}

void FstmEvalVisitor::boolValue(const BoolValue & val) {
    evals->push(val.getValue());

    checkRep();
}

void FstmEvalVisitor::var(const Var & v) {
    evals->push(v.getName() == stateName || v.getName() == eventName);

    checkRep();
}

// private methods
void FstmEvalVisitor::checkRep() const {
    if (doCheckRep) {
        assertTrue(stateName.length() > 0);
        assertTrue(islower(stateName.at(0)));
        assertTrue(eventName.length() == 0 || islower(eventName.at(0)));
        assertTrue(evals.get());
    }
}

} // namespace fstm
} // namespace fts
} // namespace core
