/*
 * Messages.cpp
 *
 * Created on 2013-09-02
 */

#include "Messages.hpp"

#include "util/Assert.hpp"

#include <sstream>

using std::ostringstream;
using std::string;

namespace config {

string Messages::wrongNumberOfArgs() {
    return "Wrong number of arguments.";
}

string Messages::invalidArg(const string & arg) {
    ostringstream oss;
    oss << "The argument '" << arg << "' is invalid.";
    return oss.str();
}

string Messages::envVarNotSet(const string & envVar) {
    assertTrue(envVar.length() > 0);

    ostringstream oss;
    oss << "The environment variable " << envVar << " is not set.";
    return oss.str();
}

string Messages::emptyFilePath() {
    return "The specified file path is empty.";
}

string Messages::errorOpeningFile(const string & filePath) {
    ostringstream oss;
    oss << "Cannot open file '" << filePath << "'.";
    return oss.str();
}

string Messages::invalidJson(const string & jsonFilePath) {
    assertTrue(jsonFilePath.length() > 0);

    ostringstream oss;
    oss << "The JSON file '" << jsonFilePath
        << "' contains some syntax error.";
    return oss.str();
}

} // namespace config
