/*
 * CounterExample.cpp
 *
 * Created on 2013-08-28
 */

#include "CounterExample.hpp"

#include "core/features/FeatureExp.hpp"
#include "core/fts/State.hpp"

#include "math/minimize/Minimizer.hpp"

#include "util/Assert.hpp"

#include <sstream>

using core::features::FeatureExp;
using core::features::FeatureExpPtr;
using core::features::SimpleFeatureExp;

using core::fts::StatePtr;

using math::minimize::Minimizer;

using std::auto_ptr;
using std::ostringstream;
using std::string;
using std::tr1::shared_ptr;

using util::Generator;

namespace core {
namespace checker {

// constructors
CounterExample::CounterExample(const FeatureExp & featureExp,
        const auto_ptr<Generator<StatePtr> > & states)
        : featureExp(featureExp.clone()), minimizedFeatureExp() {
    assertTrue(states->hasNext());
    assertTrue(featureExp.isSatisfiable());

    while (states->hasNext()) {
        path.addLast(states->next());
    }

    checkRep();
}

// public methods
string CounterExample::getFeatureExp() const {
    if (minimizedFeatureExp.length() <= 0) {
        Minimizer minimizer;
        minimizedFeatureExp = minimizer.minimize(featureExp->toString());
    }
    return minimizedFeatureExp;
}

auto_ptr<Generator<StatePtr> > CounterExample::states() const {
    return path.elements();
}

string CounterExample::toString() const {
    ostringstream oss;
    oss << "Counter example for the products " << getFeatureExp() << " :\n";
    auto_ptr<Generator<StatePtr> > stateGen = states();
    while(stateGen->hasNext()) {
        oss << "\t" << stateGen->next()->getName() << "\n";
    }
    return oss.str();
}

// private methods
void CounterExample::checkRep() const {
    if (doCheckRep) {
        assertTrue(path.count() > 0);
        assertTrue(featureExp);
        assertTrue(featureExp->isSatisfiable());
    }
}

} // namespace checker
} // namespace core
