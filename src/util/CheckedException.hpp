/*
 * CheckedException.hpp
 *
 * Created on 2013-08-30
 */

#ifndef UTIL_CHECKEDEXCEPTION_HPP
#define	UTIL_CHECKEDEXCEPTION_HPP

#include <exception>
#include <string>

namespace util {

/**
 * CheckedException is a checked exception with an error message.
 *
 * @author mwi
 */
class CheckedException : public std::exception {

private:
    const std::string message;

public:
    /**
     * @effects Makes this be a new CheckedException with the message msg.
     */
    explicit CheckedException(const std::string & msg);

    /**
     * @return the message of this.
     */
    std::string getMessage() const;

    // overridden methods
    const char * what() const throw();

     // mandatory destructor
    virtual ~CheckedException() throw() {};
};

} // namespace util

#endif	/* UTIL_CHECKEDEXCEPTION_HPP */
