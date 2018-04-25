/*
 * Generator.hpp
 *
 * Created on 2013-07-09
 */

#ifndef UTIL_GENERATOR_HPP
#define UTIL_GENERATOR_HPP

namespace util {

/**
 * Generator is an interface for a generator producing elements of type T.
 *
 * This is an implementation of the Iterator design pattern as described by
 * Barbara Liskov and John Guttag.
 *
 * @author mwi
 */
template <typename T>
class Generator {

public:
    /**
     * @return true if there are more elements to yield else returns false.
     */
    virtual bool hasNext() const = 0;

    /**
     * @requires there are more elements to yield
     * @modifies this
     * @effects Records the yield.
     * @return the next element.
     */
    virtual T next() = 0;

    // mandatory virtual destructor
    virtual ~Generator() {};
};

} // namespace util

#endif /* UTIL_GENERATOR_HPP */
