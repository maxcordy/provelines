/*
 * Checker.cpp
 *
 * Created on 2013-08-27
 */

#include "Checker.hpp"

#include "config/Messages.hpp"
#include "core/checker/Exploration.hpp"
#include "core/checker/NestedSearch.hpp"

#include "math/minimize/Minimizer.hpp"

#include "util/Assert.hpp"
#include "util/NotPossibleException.hpp"
#include "util/JsonException.hpp"

#include <fstream>

using config::Context;
using config::Messages;

using core::checker::Exploration;
using core::checker::NestedSearch;

using core::fts::FTS;

using core::features::BoolFeatureExpFactory;
using core::features::SimpleFeatureExp;
using core::features::FeatureExp;

using math::minimize::Minimizer;

using std::string;
using std::ifstream;

using std::auto_ptr;
using std::tr1::shared_ptr;

using util::Generator;
using util::JsonException;
using util::List;
using util::NotPossibleException;

namespace core {
namespace checker {

// constructors
Checker::Checker(const string & fstmFilePath) : ctx(), state(idle) {
    if (fstmFilePath.length() <= 0) {
        throw NotPossibleException(Messages::emptyFilePath());
    }

    factory = ctx.getBoolFeatureExpFactory();
    fd = ctx.createEmptyFD();

    ifstream fstmStream(fstmFilePath.c_str());

    if (!fstmStream.is_open()) {
        throw NotPossibleException(Messages::errorOpeningFile(fstmFilePath));
    }

    try {
        fts = ctx.createFSTM(fstmStream);
    } catch (JsonException & jsonEx) {
        throw NotPossibleException(Messages::invalidJson(fstmFilePath));
    }

    checkRep();
}

// public methods
void Checker::run() {
    state = Running;
    if (fts->hasProperty()) {
        NestedSearch nestedSearch(fd, fts, factory);
        nestedSearch.run();
        violatedProducts = nestedSearch.getBadProducts();
        if (violatedProducts->isSatisfiable()) {
            result = Violated;
            counterExamples = nestedSearch.getCounterExamples();
        }
        else {
            result = Satisfied;
        }
    }
    else {
        Exploration exploration(fd, fts, factory);
        exploration.run();
        violatedProducts = exploration.getBadProducts();
        if (violatedProducts->isSatisfiable()) {
            result = Error;
            counterExamples = exploration.getCounterExamples();
        }
        else {
            result = Satisfied;
        }
    }
    state = Terminated;

    checkRep();
}

bool Checker::isTerminated() const {
    return state == Terminated;
}

Checker::Result Checker::getResult() const {
    assertTrue(state == Terminated);
    return result;
}

string Checker::getViolatedProducts() const {
    assertTrue(state == Terminated);
    Minimizer minimizer;
    return minimizer.minimize(violatedProducts->toString());
}

auto_ptr<Generator<CounterExample> > Checker::getCounterExamples() const {
    assertTrue(state == Terminated);

    return counterExamples->elements();
}

// private methods
void Checker::checkRep() const {
    if (doCheckRep) {
        if (violatedProducts) {
            assertTrue(state == Terminated);
        }
        else {
            assertTrue(state != Terminated);
        }
    }
    assertTrue(factory);
    assertTrue(fd);
    assertTrue(fts);
}

} // namespace checker
} // namespace core
