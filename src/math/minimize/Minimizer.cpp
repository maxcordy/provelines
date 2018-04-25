/*
 * Minimizer.cpp
 *
 * Created on 2013-08-29
 */

#include "Minimizer.hpp"

#include "MinimizeGenVisitor.hpp"
#include "MinimizeParser.hpp"
#include "VarTable.hpp"

#include "config/Environment.hpp"

#include "core/checker/Checker.hpp"

#include "util/strings.hpp"
#include "util/parser/BoolExp.hpp"
#include "util/parser/BoolExpParser.hpp"

#include <glog/logging.h>

#include <cstdlib>
#include <fstream>

using config::Environment;

using std::ifstream;
using std::string;

using util::parser::BoolExpParser;
using util::parser::BoolExpPtr;

namespace math {
namespace minimize {

// static variables
const string Minimizer::MinimizeFilePath = "/tmp/pvl_minimize.tmp";

// constructors
Minimizer::Minimizer() {
    checkRep();
}

// public methods
string Minimizer::minimize(const string & expression) const {
    BoolExpParser boolExpParser(expression);
    BoolExpPtr boolExp = boolExpParser.parse();

    MinimizeGenVisitor visitor;
    boolExp->accept(visitor);

    // Workaround due to a bug of the Minimizer program.
    if (expression == "true" || expression == "false") {
        return expression;
    }

    if (visitor.isAborted()) {
        return expression;
    }

    string command("java -jar ");
    command += Environment::getMinimizeHome();
    command += "/Minimize.jar ";
    command += "\"";
    command += visitor.getMinimizeExp();
    command += "\" > ";
    command += MinimizeFilePath;

    int result = system(command.c_str());
    assertTrue(result == 0);

    ifstream inputFileStream(MinimizeFilePath.c_str());
    assertTrue(inputFileStream.is_open());
    string minimizeExp = util::toString(inputFileStream);

    MinimizeParser minimizeParser(minimizeExp, visitor.getVarTable());
    const string endExpression(minimizeParser.parse());

    LOG(INFO) << expression << " minimized into " << endExpression;
    return endExpression;
}

// private methods
void Minimizer::checkRep() const {
    if (doCheckRep) {
        assertTrue(Minimizer::MinimizeFilePath.length() > 0);
    }
}

} // minimize
} // math
