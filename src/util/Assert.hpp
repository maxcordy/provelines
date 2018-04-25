/*
 * Assert.hpp
 *
 * Created on 2013-07-08
 */

#ifndef UTIL_ASSERT_HPP
#define UTIL_ASSERT_HPP

#include <cassert>

/**
 * Helper that provides various assertion functions.
 *
 * doCheckRep: if true, enables rep invariant checking
 *
 * @note For the time being assertTrue is implemented with the standard assert.
 *       However in the future we may want to throw an exception instead or
 *       enable assertion checking (at least some of them) in release mode.
 *
 * @author mwi
 */

#ifndef NDEBUG
// DEBUG MODE
#define assertTrue(exp) { assert(exp); }
const bool doCheckRep = true;

#else
// RELEASE MODE
#define assertTrue(exp)
const bool doCheckRep = false;

#endif

#endif /* UTIL_ASSERT_HPP */
