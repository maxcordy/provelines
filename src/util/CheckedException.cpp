/*
 * CheckedException.cpp
 *
 * Created 2013-08-30
 */

#include "CheckedException.hpp"

using std::string;

namespace util {

// constructors
CheckedException::CheckedException(const std::string & msg)
        : message(msg) {}

// public methods
string CheckedException::getMessage() const {
    return message;
}

const char * CheckedException::what() const throw() {
    return message.c_str();
}

} // namespace util
