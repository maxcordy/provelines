/*
 * strings.cpp
 *
 * Created on 2013-08-07
 */

#include "strings.hpp"

#include <algorithm>
#include <sstream>

using std::ptr_fun;
using std::istream;
using std::ostringstream;
using std::string;
using std::transform;

namespace util {

void toLowercaseFirst(string & str) {
    transform(str.begin(), str.begin() + 1, str.begin(), ptr_fun(tolower));
}

string toString(istream & inputStream) {
    ostringstream outputStringStream;
    inputStream >> outputStringStream.rdbuf();
    return outputStringStream.str();
}

} // namespace util
