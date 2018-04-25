/*
 * files.hpp
 *
 * Created on 2013-07-26
 */

#ifndef UTIL_FILES_HPP
#define	UTIL_FILES_HPP

#include <string>

namespace util {

/**
 * @requires a file exists at the relative path 'src'
 * @modifies the file at relative path 'dest'
 * @effects Copies the file at relative path 'src' into the file at relative
 *          path 'dest'.
 */
void copyFile(const std::string & src, const std::string & dest);

} // namespace util

#endif	/* UTIL_FILES_HPP */
