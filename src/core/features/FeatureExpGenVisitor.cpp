/*
 * FeatureExpGenVisitor.cpp
 *
 * Created on 2013-08-28
 */

#include "FeatureExpGenVisitor.hpp"

#include "BoolFeatureExp.hpp"
#include "FeatureExp.hpp"
#include "util/Assert.hpp"
#include "util/Stack.hpp"
#include "util/parser/AndExp.hpp"
#include "util/parser/BoolValue.hpp"
#include "util/parser/NotExp.hpp"
#include "util/parser/OrExp.hpp"
#include "util/parser/Var.hpp"

#pragma GCC diagnostic ignored "-Wunused-parameter"

using util::parser::AndExp;
using util::parser::BoolValue;
using util::parser::NotExp;
using util::parser::OrExp;
using util::parser::Var;

namespace core {
namespace features {

// constructors
FeatureExpGenVisitor::FeatureExpGenVisitor(const BoolFeatureExpFactory &
        featureExpFactory)
        : featureExpFactory(featureExpFactory), featureExpressions() {
    checkRep();
}

// public methods
FeatureExpPtr FeatureExpGenVisitor::getFeatureExp() const {
    assertTrue(!featureExpressions.empty());

    return featureExpressions.peek();
}

void FeatureExpGenVisitor::orExp(const OrExp & exp) {
    assertTrue(featureExpressions.count() >= 2);

    FeatureExpPtr rhs = featureExpressions.pop();
    FeatureExpPtr lhs = featureExpressions.pop();
    lhs->disjunction(*rhs);
    featureExpressions.push(lhs);

    checkRep();
}

void FeatureExpGenVisitor::andExp(const AndExp & exp) {
    assertTrue(featureExpressions.count() >= 2);

    FeatureExpPtr rhs = featureExpressions.pop();
    FeatureExpPtr lhs = featureExpressions.pop();
    lhs->conjunction(*rhs);
    featureExpressions.push(lhs);

    checkRep();
}

void FeatureExpGenVisitor::notExp(const NotExp & exp) {
    assertTrue(featureExpressions.count() >= 1);

    FeatureExpPtr featureExp = featureExpressions.pop();
    featureExp->negation();
    featureExpressions.push(featureExp);

    checkRep();
}

void FeatureExpGenVisitor::boolValue(const BoolValue & val) {
    if (val.getValue()) {
        featureExpressions.push(FeatureExpPtr(featureExpFactory.getTrue()));
    } else {
        featureExpressions.push(FeatureExpPtr(featureExpFactory.getFalse()));
    }

    checkRep();
}

void FeatureExpGenVisitor::var(const Var & v) {
    featureExpressions.push(FeatureExpPtr(
            featureExpFactory.create(v.getName())));
}

// private methods
void FeatureExpGenVisitor::checkRep() const {
    if (doCheckRep) {
        // Nothing to do
    }
}

} // namespace features
} // namespace core
