/*
 * Hashable.hpp
 *
 * Created on 2013-08-03
 */

#ifndef UTIL_HASHABLE_HPP
#define	UTIL_HASHABLE_HPP

namespace util {

/**
 * A class implementing the Hashable interface must provide the equals and
 * hashCode methods.
 *
 * Two objects being equals must have the same hash code value and vice versa.
 *
 * The equals method must be reflexive, symmetric, transitive and consistent.
 *
 * @author mwi
 */
template<typename T>
class Hashable {

public:
    /**
     * @return true iff 't' is equals to this.
     */
    virtual bool equals(const T & t) const = 0;

    /**
     * @return a hash code value for this.
     */
    virtual unsigned int hashCode() const = 0;

    // mandatory virtual destructor
    virtual ~Hashable() {}
};

} // namespace util

#endif	/* UTIL_HASHABLE_HPP */
