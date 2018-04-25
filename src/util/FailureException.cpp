/*
 * FailureException.cpp
 *
 * Created 2013-08-30
 */

#include "FailureException.hpp"

using std::string;

namespace util {

// constructors
FailureException::FailureException(const std::string & msg)
        : message(msg) {}

// public methods
string FailureException::getMessage() const {
    return message;
}

const char * FailureException::what() const throw() {
    return message.c_str();
}

} // namespace util
