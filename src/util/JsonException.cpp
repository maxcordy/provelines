/*
 * JsonException.cpp
 *
 * Created 2013-09-02
 */

#include "JsonException.hpp"

using std::string;

namespace util {

// constructors
JsonException::JsonException(const std::string & msg)
        : CheckedException(msg) {}

} // namespace util
