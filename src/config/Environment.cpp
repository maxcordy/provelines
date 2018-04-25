/*
 * Environment.cpp
 *
 * Created on 2013-09-02
 */

#include "Environment.hpp"

#include "Messages.hpp"
#include "util/Assert.hpp"
#include "util/NotPossibleException.hpp"

#include <cstdlib>

using std::getenv;
using std::string;
using util::NotPossibleException;

namespace config {

// static variables
const string Environment::MinimizeHomeEnv("MINIMIZE_HOME");

// public static method
string Environment::getMinimizeHome() {
    assertTrue(isSet(MinimizeHomeEnv));

    return string(getenv(MinimizeHomeEnv.c_str()));
}

void Environment::checkVariables() {
    if (!isSet(MinimizeHomeEnv)) {
        throw NotPossibleException(Messages::envVarNotSet(MinimizeHomeEnv));
    }
}

// private static methods
bool Environment::isSet(const string & envVar) {
    const char * value = getenv(envVar.c_str());
    return value;
}

} // namespace config
