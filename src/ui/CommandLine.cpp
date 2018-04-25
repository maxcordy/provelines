/*
 * CommandLine.cpp
 *
 * Created on 2013-09-02
 */

#include "CommandLine.hpp"

#include "config/Messages.hpp"
#include "util/Assert.hpp"
#include "util/NotPossibleException.hpp"

#include <sstream>

using config::Messages;
using std::endl;
using std::string;
using std::ostringstream;
using util::NotPossibleException;

namespace ui {

// constructors
CommandLine::CommandLine(int argc, const char * argv[])
        : verbose(false), inputPath() {
    if (argc < 2 || argc > 3) {
        throw NotPossibleException(Messages::wrongNumberOfArgs());
    }

    inputPath = string(argv[argc - 1]);

    if (argc == 3) {
        string flag(argv[1]);
        if (flag == "-v") {
            verbose = true;
        } else {
            throw NotPossibleException(Messages::invalidArg(flag));
        }
    }

    checkRep();
}

bool CommandLine::isVerbose() const {
    return verbose;
}

string CommandLine::getInputPath() const {
    return inputPath;
}

// static public methods
string CommandLine::getUsage() {
    ostringstream oss;
    oss << "Usage: provelines [-options] input" << endl
        << "where options include:" << endl
        << "    -v            "
        << "enable verbose output, i.e., print counterexamples" << endl
        << "See http://www.info.fundp.ac.be/fts/provelines/ for more details.";
    return oss.str();
}

// private methods
void CommandLine::checkRep() const {
    if (doCheckRep) {
        // Nothing to do
    }
}

} // namespace ui
