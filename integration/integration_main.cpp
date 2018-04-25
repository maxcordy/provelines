/*
 * acceptance_main.cpp
 *
 * Created on 2013-09-03
 */

#include "core/checker/Checker.hpp"
#include "ui/CommandLine.hpp"

#include <glog/logging.h>
#include <gtest/gtest.h>

#include <iostream>

using core::checker::Checker;
using std::string;
using ui::CommandLine;

/** The name of the executables. */
const string ExecName("runIntegrationTests");

/** The directory containing the input data used for testing. */
const string InputsDir("../integration/inputs/");

/** The input paths. */
const string MotorManager(InputsDir + "MotorManager.json");
const string MotorManagerWithProp1(InputsDir + "MotorManagerWithProp1.json");
const string MotorManagerWithProp2(InputsDir + "MotorManagerWithProp2.json");
const string MotorManagerWithProp3(InputsDir + "MotorManagerWithProp3.json");
const string MotorManagerWithProp4(InputsDir + "MotorManagerWithProp4.json");
const string MotorManagerWithProp5(InputsDir + "MotorManagerWithProp5.json");

/**
 * Main entry point for the integration tests of the ProVeLines application.
 *
 * @author mwi
 */

/**
 * @return 0 if all the tests are successful, else returns 1.
 */
int main(int argc, char * argv[]) {

    // Log messages to stderr instead of logfiles.
    FLAGS_logtostderr = true;
    // If specified, logfiles are written into this directory instead of the
    // default logging directory.
    FLAGS_log_dir = "";
    // Show all VLOG(m) messages for m less or equal the value of this flag.
    FLAGS_v = 0;

    google::InitGoogleLogging(argv[0]);

    // Since Google Mock depends on Google Test, InitGoogleMock() is
    // also responsible for initializing Google Test.  Therefore there's
    // no need for calling testing::InitGoogleTest() separately.
    testing::InitGoogleTest(&argc, argv);

    const int result = RUN_ALL_TESTS();

    google::ShutdownGoogleLogging();

    return result;
}

/**
 * Tests ProVeLines with MotorManager.json
 */
TEST(ProVeLines, MotorManager) {
    // Setup
    const int nbOfArgs = 2;
    const char * args[nbOfArgs] = {ExecName.c_str(), MotorManager.c_str()};

    // Exercise
    CommandLine commandLine(nbOfArgs, args);
    Checker checker(commandLine.getInputPath());
    checker.run();

    // Verify
    ASSERT_EQ(Checker::Satisfied, checker.getResult());
}

/**
 * Tests ProVeLines with MotorManagerWithProp1.json
 */
TEST(ProVeLines, MotorManagerWithProp1) {
    // Setup
    const int nbOfArgs = 2;
    const char * args[nbOfArgs] = {ExecName.c_str(),
            MotorManagerWithProp1.c_str()};

    // Exercise
    CommandLine commandLine(nbOfArgs, args);
    Checker checker(commandLine.getInputPath());
    checker.run();

    // Verify
    ASSERT_EQ(Checker::Violated, checker.getResult());
}

/**
 * Tests ProVeLines with MotorManagerWithProp2.json
 */
TEST(ProVeLines, MotorManagerWithProp2) {
    // Setup
    const int nbOfArgs = 2;
    const char * args[nbOfArgs] = {ExecName.c_str(),
            MotorManagerWithProp2.c_str()};

    // Exercise
    CommandLine commandLine(nbOfArgs, args);
    Checker checker(commandLine.getInputPath());
    checker.run();

    // Verify
    ASSERT_EQ(Checker::Satisfied, checker.getResult());
}

/**
 * Tests ProVeLines with MotorManagerWithProp3.json
 */
TEST(ProVeLines, MotorManagerWithProp3) {
    // Setup
    const int nbOfArgs = 2;
    const char * args[nbOfArgs] = {ExecName.c_str(),
            MotorManagerWithProp3.c_str()};

    // Exercise
    CommandLine commandLine(nbOfArgs, args);
    Checker checker(commandLine.getInputPath());
    checker.run();

    // Verify
    ASSERT_EQ(Checker::Satisfied, checker.getResult());
}

/**
 * Tests ProVeLines with MotorManagerWithProp4.json
 */
TEST(ProVeLines, MotorManagerWithProp4) {
    // Setup
    const int nbOfArgs = 2;
    const char * args[nbOfArgs] = {ExecName.c_str(),
            MotorManagerWithProp4.c_str()};

    // Exercise
    CommandLine commandLine(nbOfArgs, args);
    Checker checker(commandLine.getInputPath());
    checker.run();

    // Verify
    ASSERT_EQ(Checker::Violated, checker.getResult());
}

/**
 * Tests ProVeLines with MotorManagerWithProp5.json
 */
TEST(ProVeLines, MotorManagerWithProp5) {
    // Setup
    const int nbOfArgs = 2;
    const char * args[nbOfArgs] = {ExecName.c_str(),
            MotorManagerWithProp5.c_str()};

    // Exercise
    CommandLine commandLine(nbOfArgs, args);
    Checker checker(commandLine.getInputPath());
    checker.run();

    // Verify
    ASSERT_EQ(Checker::Satisfied, checker.getResult());
}
