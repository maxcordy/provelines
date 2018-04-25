/*
 * BoolFeatureExp.cpp
 *
 * Created on 2013-07-22
 */

#include "BoolFeatureExp.hpp"

#include "math/BoolFct.hpp"
#include "math/minimize/Minimizer.hpp"

#include "util/Assert.hpp"

#include <glog/logging.h>

#include <stdexcept>

using math::BoolFct;
using math::minimize::Minimizer;

using std::string;
using std::tr1::shared_ptr;

using util::MemoryManager;

namespace core {
namespace features {

MemoryManager<BoolFeatureExp> BoolFeatureExp::memoryManager;

BoolFeatureExp::BoolFeatureExp(BoolFct * function)
    : function(function) {
    checkRep();
}

FeatureExp * BoolFeatureExp::conjunction(FeatureExp & expression) {
    assertTrue(isValid());
    assertTrue(expression.isValid());
    assertTrue(this != &expression);

    BoolFeatureExp & rhs = static_cast<BoolFeatureExp &>(expression);
    function->conjunction(*rhs.function);

    checkRep();

    return this;
}

FeatureExp * BoolFeatureExp::disjunction(FeatureExp & expression) {
    assertTrue(isValid());
    assertTrue(expression.isValid());
    assertTrue(this != &expression);

    BoolFeatureExp & rhs = static_cast<BoolFeatureExp &>(expression);
    function->disjunction(*rhs.function);

    checkRep();

    return this;
}

FeatureExp * BoolFeatureExp::negation() {
    assertTrue(isValid());

    function->negation();

    checkRep();

    return this;
}

bool BoolFeatureExp::isSatisfiable() const {
    assertTrue(isValid());

    return function->isSatisfiable();
}

bool BoolFeatureExp::isTautology() const{
    assertTrue(isValid());

    return function->isTautology();
}

bool BoolFeatureExp::isEquivalent(const FeatureExp & expression) const {
    assertTrue(isValid());
    assertTrue(expression.isValid());

    const BoolFeatureExp & rhs = static_cast<const BoolFeatureExp &>(
        expression);
    return function->isEquivalent(*rhs.function);
}

bool BoolFeatureExp::implies(const FeatureExp & expression) const {
    assertTrue(isValid());
    assertTrue(expression.isValid());

    const BoolFeatureExp & rhs = static_cast<const BoolFeatureExp &>(
        expression);
    return function->implies(*rhs.function);
}

bool BoolFeatureExp::isValid() const {
    return function->isValid();
}

string BoolFeatureExp::toString() const {
        return function->toString();
}

FeatureExp * BoolFeatureExp::clone() const {
    return new BoolFeatureExp(function->clone());
}

void BoolFeatureExp::checkRep() const {
    if(doCheckRep) {
        assertTrue(function.get());
    }
}

} // namespace features
} // namespace core
