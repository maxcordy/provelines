/*
 * File:   FailureException.hpp
 * Author: marco
 *
 * Created on 30 août 2013, 15:03
 */

#ifndef UTIL_FAILUREEXCEPTION_HPP
#define	UTIL_FAILUREEXCEPTION_HPP

#include "Uncopyable.hpp"

#include <exception>
#include <string>

namespace util {

/**
 * FailureException is a runtime exception that should be thrown when a
 * contract or an invariant is broken.
 *
 * @author mwi
 */
class FailureException : public std::exception, private Uncopyable {

private:
    const std::string message;

public:
    /**
     * @effects Makes this be a new failure exception with the message msg.
     */
    explicit FailureException(const std::string & msg);

    /**
     * @return the message of this.
     */
    std::string getMessage() const;

    // overridden methods
    const char * what() const throw();

    // mandatory destructor
    ~FailureException() throw() {}
};

} // namespace util

#endif	/* UTIL_FAILUREEXCEPTION_HPP */
