#include "core/checker/Checker.hpp"
#include "core/checker/CounterExample.hpp"

#include "ui/CommandLine.hpp"

#include "util/Assert.hpp"
#include "util/logging.hpp"
#include "util/Generator.hpp"
#include "util/NotPossibleException.hpp"

#include <Version.hpp>
#include <glog/logging.h>

#include <cstdlib>
#include <iostream>
#include <memory>

using core::checker::Checker;
using core::checker::CounterExample;

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::auto_ptr;

using ui::CommandLine;

using util::Generator;
using util::NotPossibleException;

/**
 * @mainpage
 *
 * ProVeLines is a <b>Pro</b>duct line of <b>Ve</b>rifiers for software
 * product <b>Lines</b>.
 *
 * See http://www.info.fundp.ac.be/fts/provelines/
 */

/**
 * Main entry point for the ProVeLines application.
 *
 * @author mwi
 */

/**
 * @modifies stdout
 * @effects Prints on stdout the current version of ProVeLines.
 */
void printVersion();

/**
 * @requires gen != null
 * @modifies stdout, gen
 * @effects Prints on stdout the counter examples from the generator 'gen'.
 */
void printCounterExamples(auto_ptr<Generator<CounterExample> > gen);

/**
 * @requires checker.result = satisfied
 * @modifies stdout
 * @effects Writes various information on stdout when the result of the model
 *           checking is satisfied.
 */
void onSatisfied(const Checker & checker);

/**
 * @requires checker.result = violated
 * @modifies stdout
 * @effects Writes various information on stdout when the result of the model
 *           checking is violated. The written information include the counter
 *           examples iff verbose is true.
 */
void onViolated(const Checker & checker, bool verbose);

/**
 * @requires checker.result = error
 * @modifies stdout
 * @effects Writes various information on stdout when the result of the model
 *           checking is error. The written information include the counter
 *           examples iff verbose is true.
 */
void onError(const Checker & checker, bool verbose);

/**
 * @return 0 if the main method was successfull, else returns 1.
 */
int main(int argc, const char * argv[]) {

    util::initLogging(argv[0]);

    printVersion();

    bool validCommandLine = false;

    try {
        CommandLine commandLine(argc, argv);
        validCommandLine = true;

        Checker checker(commandLine.getInputPath());
        checker.run();

        switch (checker.getResult()) {
            case Checker::Satisfied:
                onSatisfied(checker);
                break;
            case Checker::Violated:
                onViolated(checker, commandLine.isVerbose());
                break;
            case Checker::Error:
                onError(checker, commandLine.isVerbose());
                break;
            default:
                assertTrue(false);
                break;
        }
    } catch (NotPossibleException & ex) {
        LOG(ERROR) << ex.getMessage();
        cerr << ex.getMessage() << endl;

        if (!validCommandLine) {
            cout << CommandLine::getUsage() << endl;
        }

        util::terminateLogging();
        return EXIT_FAILURE;
    }

    util::terminateLogging();
    return EXIT_SUCCESS;
}

void printVersion() {
    cout << "ProVeLines " << PVL_VERSION
         << " (revision " << PVL_REVISION << ")" << endl;
}

void printCounterExamples(auto_ptr<Generator<CounterExample> > gen) {
    while(gen->hasNext()) {
        cout << endl << gen->next().toString();
    }
}

void onSatisfied(const Checker & checker) {
    assertTrue(checker.getResult() == Checker::Satisfied);

    cout << "Satisfied :-)" << endl;
}

void onViolated(const Checker & checker, bool verbose) {
    assertTrue(checker.getResult() == Checker::Violated);

    cout << "Violated :-\\" << endl;
    cout << "The following products exhibit errors:" << endl;
    cout << checker.getViolatedProducts() << endl;

    if (verbose) {
        printCounterExamples(checker.getCounterExamples());
    }
}

void onError(const Checker & checker, bool verbose) {
    assertTrue(checker.getResult() == Checker::Error);

    cout << "Error :-\\" << endl;
    cout << "The following products exhibit deadlocks:" << endl;
    cout << checker.getViolatedProducts() << endl;

    if (verbose) {
        printCounterExamples(checker.getCounterExamples());
    }
}
