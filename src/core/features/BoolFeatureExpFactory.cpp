/*
 * BoolFeatureExpFactory.cpp
 *
 * Created on 2013-07-26
 */

#include "BoolFeatureExpFactory.hpp"

#include "BoolFeatureExp.hpp"

#include "math/BoolFct.hpp"
#include "math/BoolFctFactory.hpp"
#include "math/BoolVar.hpp"

#include "util/Assert.hpp"

using math::BoolFct;
using math::BoolFctFactory;
using math::BoolVar;

using std::string;
using std::tr1::shared_ptr;

namespace core {
namespace features {

BoolFeatureExpFactory::BoolFeatureExpFactory(const shared_ptr<BoolFctFactory>
    & boolFctFactory) : functionFactory(boolFctFactory) {
    assertTrue(functionFactory);
}

BoolFeatureExp * BoolFeatureExpFactory::create(const string & featureName)
        const {
    return new BoolFeatureExp(functionFactory->create(BoolVar::makeBoolVar(
            featureName)));
}

BoolFeatureExp * BoolFeatureExpFactory::create(BoolFct * boolFct) const {
    return new BoolFeatureExp(boolFct);
}

BoolFeatureExp * BoolFeatureExpFactory::getTrue() const {
    return new BoolFeatureExp(functionFactory->getTrue());
}

BoolFeatureExp * BoolFeatureExpFactory::getFalse() const {
    return new BoolFeatureExp(functionFactory->getFalse());
}

} // namespace features
} // namespace core
