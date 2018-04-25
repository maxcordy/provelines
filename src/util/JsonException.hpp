/*
 * JsonException.hpp
 *
 * Created on 2013-09-02
 */

#ifndef UTIL_JSONEXCEPTION_HPP
#define	UTIL_JSONEXCEPTION_HPP

#include "CheckedException.hpp"

#include <string>

namespace util {

/**
 * JsonException is a checked exception indicating some syntax error in some
 * JSON code.
 *
 * @author mwi
 */
class JsonException : public CheckedException {

public:
    /**
     * @effects Makes this be a new JsonException with the message msg.
     */
    explicit JsonException(const std::string & msg);

     // mandatory destructor
    ~JsonException() throw() {};
};

} // namespace util

#endif	/* UTIL_JSONEXCEPTION_HPP */
