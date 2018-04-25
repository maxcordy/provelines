/*
 * Context.cpp
 *
 * Created on 2013-08-02
 */

#include "Context.hpp"

#include "Environment.hpp"
#include "core/features/BoolFeatureExp.hpp"
#include "core/features/BoolFeatureExpFactory.hpp"
#include "core/features/FeatureExp.hpp"
#include "core/fts/fstm/FSTM.hpp"
#include "math/BoolFctCUDDFactory.hpp"
#include "util/Assert.hpp"
#include "util/NotPossibleException.hpp"

using core::features::BoolFeatureExpPtr;
using core::features::BoolFeatureExpFactory;
using core::features::BoolFeatureExpFactoryPtr;
using core::features::FeatureExpPtr;
using core::fts::FTSPtr;
using core::fts::fstm::FSTM;
using math::BoolFctCUDDFactory;
using math::BoolFctFactoryPtr;
using std::istream;
using std::string;
using util::NotPossibleException;

namespace config {

// constructors
Context::Context()
        : boolFctFactory(new BoolFctCUDDFactory()),
          boolFeatureExpFactory(new BoolFeatureExpFactory(boolFctFactory)) {

    Environment::checkVariables();

    checkRep();
}

// public methods
BoolFctFactoryPtr Context::getBoolFctFactory() const {
    return boolFctFactory;
}

BoolFeatureExpFactoryPtr Context::getBoolFeatureExpFactory() const {
    return boolFeatureExpFactory;
}

FTSPtr Context::createFSTM(istream & jsonFstm) const {
    return FTSPtr(new FSTM(jsonFstm, *getBoolFeatureExpFactory()));
}

FeatureExpPtr Context::createEmptyFD() const {
    return BoolFeatureExpPtr(getBoolFeatureExpFactory()->getTrue());
}

// private methods
void Context::checkRep() const {
    if (doCheckRep) {
        assertTrue(boolFctFactory);
        assertTrue(boolFeatureExpFactory);
    }
}

} // namespace config
