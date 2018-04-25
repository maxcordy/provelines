/*
 * NotPossibleException.hpp
 *
 * Created on 2013-08-30
 */

#ifndef UTIL_NOTPOSSIBLEEXCEPTION_HPP
#define	UTIL_NOTPOSSIBLEEXCEPTION_HPP

#include "CheckedException.hpp"

#include <string>

namespace util {

/**
 * NotPossibleException is a checked exception with a message explaining
 * the problem to the user.
 *
 * @author mwi
 */
class NotPossibleException : public CheckedException {

public:
    /**
     * @effects Makes this be a new NotPossibleException with the message msg.
     */
    explicit NotPossibleException(const std::string & msg);

     // mandatory destructor
    ~NotPossibleException() throw() {};
};

} // namespace util

#endif	/* UTIL_NOTPOSSIBLEEXCEPTION_HPP */
