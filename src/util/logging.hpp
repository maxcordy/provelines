/*
 * logging.hpp
 *
 * Created on 2013-09-02
 */

#ifndef UTIL_LOGGING_HPP
#define	UTIL_LOGGING_HPP

namespace util {

/**
 * @requires programName != null
 * @modifies this
 * @effects Initializes logging with the program name 'programName'.
 */
void initLogging(const char * programName);

/**
 * @requires logging has been initialized
 * @modifies this
 * @effects Terminates logging.
 */
void terminateLogging();

} // namespace util

#endif	/* UTIL_LOGGING_HPP */
