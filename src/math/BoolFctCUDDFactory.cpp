/*
 * BoolFctCUDDFactory.cpp
 *
 * Created on 2013-07-26
 */

#include "BoolFctCUDDFactory.hpp"

#include "BoolFctCUDD.hpp"

using std::string;

namespace math {

BoolFctCUDDFactory::BoolFctCUDDFactory() {}

BoolFct * BoolFctCUDDFactory::create(const BoolVar & var) const {
    return new BoolFctCUDD(var);
}

BoolFct * BoolFctCUDDFactory::create(const string & varName) const {
    return create(BoolVar::makeBoolVar(varName));
}

BoolFct * BoolFctCUDDFactory::getTrue() const {
    return BoolFctCUDD::getTrue();
}

BoolFct * BoolFctCUDDFactory::getFalse() const {
    return BoolFctCUDD::getFalse();
}

} // namespace math
