/*
 * Clonable.hpp
 *
 * Created on 2013-07-26
 */

#ifndef UTIL_CLONEABLE_HPP
#define	UTIL_CLONEABLE_HPP

#include <tr1/memory>

namespace util {

/**
 * A class implements the Cloneable interface to indicate that instances of
 * that class can be cloned.
 *
 * @author mwi
 */
template <typename T>
class Cloneable {

public:
    /**
     * @return a deep copy of this.
     */
    virtual T * clone() const = 0;

    // mandatory virtual destructor
    virtual ~Cloneable() {}
};

} // namespace util

#endif	/* UTIL_CLONEABLE_HPP */
