/*
 * NotPossibleException.cpp
 *
 * Created 2013-08-30
 */

#include "NotPossibleException.hpp"

using std::string;

namespace util {

// constructors
NotPossibleException::NotPossibleException(const std::string & msg)
        : CheckedException(msg) {}

} // namespace util
