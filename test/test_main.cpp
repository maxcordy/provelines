/*
 * test_main.cpp
 *
 * Created on 2013-08-05
 */

#include <glog/logging.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>

/**
 * Main entry point for the unit tests of the ProVeLines application.
 *
 * @author bdawagne
 * @author mwi
 */

/**
 *
 * @return 0 if all the unit tests are successful, else returns 1.
 */
int main(int argc, char * argv[]) {

    // Log messages to stderr instead of logfiles.
    FLAGS_logtostderr = false;
    // If specified, logfiles are written into this directory instead of the
    // default logging directory.
    FLAGS_log_dir = "";
    // Show all VLOG(m) messages for m less or equal the value of this flag.
    FLAGS_v = 3;

    google::InitGoogleLogging(argv[0]);

    std::cout << "Running main() from test_main.cpp\n";

    // Since Google Mock depends on Google Test, InitGoogleMock() is
    // also responsible for initializing Google Test.  Therefore there's
    // no need for calling testing::InitGoogleTest() separately.
    testing::InitGoogleMock(&argc, argv);

    const int result = RUN_ALL_TESTS();

    google::ShutdownGoogleLogging();

    return result;
}
