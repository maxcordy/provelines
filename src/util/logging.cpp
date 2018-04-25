/*
 * logging.cpp
 *
 * Created on 2013-09-02
 */

#include <glog/logging.h>

namespace util {

void initLogging(const char * programName) {
    // The numbers of severity levels INFO, WARNING, ERROR, and FATAL
    // are 0, 1, 2, and 3, respectively.

    // Log messages to stderr instead of logfiles.
    FLAGS_logtostderr = true;

    // Copy log messages at or above this level to stderr in addition to
    // logfiles.
    FLAGS_stderrthreshold = 3;

    // Log messages at or above this level.
    FLAGS_minloglevel = 0;

    // If specified, logfiles are written into this directory instead of the
    // default logging directory.
    FLAGS_log_dir = "";

    // Show all VLOG(m) messages for m less or equal the value of this flag.
    FLAGS_v = 0;

    google::InitGoogleLogging(programName);

    // Dump useful information when the program crashes on certain signals.
    google::InstallFailureSignalHandler();
}

void terminateLogging() {
    // Avoid potential memory leaks.
    google::ShutdownGoogleLogging();
}

} // namespace util
