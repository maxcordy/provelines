/*
 * Uncopyable.hpp
 *
 * Created on 2013-07-08
 */

#ifndef UTIL_UNCOPYABLE_HPP
#define UTIL_UNCOPYABLE_HPP

namespace util {

/**
 * This class prevents objects from being copied. Actually it disallows
 * functionality automatically provided by compilers.
 *
 * Inheritance from Uncopyable does not have to be public, private
 * inheritance is perfectly fine.
 *
 * @note See Item 6 from Effective C++.
 *
 * @author mwi
 */
class Uncopyable {

protected:
    // Allows construction and destruction of derived objects.
    Uncopyable() {}
    ~Uncopyable() {}

private:
    // Prevents copying.
    Uncopyable(const Uncopyable &);
    Uncopyable & operator=(const Uncopyable &);
};

} // namespace util

#endif /* UTIL_UNCOPYABLE_HPP */
