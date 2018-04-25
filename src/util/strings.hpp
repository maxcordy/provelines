/*
 * strings.hpp
 *
 * Created on 2013-08-27
 */

#ifndef UTIL_STRINGS_HPP
#define	UTIL_STRING_HPP

#include <string>

namespace util {

/**
 * @modifies str
 * @effects Transforms the first letter of 'str' to lowercase.
 */
void toLowercaseFirst(std::string & str);

/**
 * @modifies inputStream
 * @effects Reads all the content of the stream 'inputStream'.
 * @return the content of the stream 'inputStream' as a string.
 */
std::string toString(std::istream & inputStream);

} // namespace util

#endif	/* UTIL_FILES_HPP */
