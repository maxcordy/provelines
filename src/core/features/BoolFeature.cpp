/*
 * BoolFeature.cpp
 *
 * Created on 2013-07-22
 */

#include "BoolFeature.hpp"

#include "util/Assert.hpp"

using std::string;
using math::BoolVar;

namespace core {
namespace features {

    BoolFeature::BoolFeature(BoolVar variable)
        : variable(variable) {
        checkRep();
    }

    int BoolFeature::getId() const {
        return variable.getId();
    }

    string BoolFeature::getName() const {
        return variable.getName();
    }

    void BoolFeature::checkRep() const {
        if(doCheckRep) {
            // Nothing here
        }
    }

} // namespace features
} // namespace core
