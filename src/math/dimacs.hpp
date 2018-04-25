/*
 * dimacs.hpp
 *
 * Created on 2013-07-26
 */

#ifndef MATH_DIMACS_HPP
#define	MATH_DIMACS_HPP

#include "forwards.hpp"

#include <fstream>

namespace math {

/**
 * @requires 'stream' is a valid dimacs stream
 * @modifies this
 * @effects Reads the input stream 'stream'.
 * @return the boolean function represented in 'stream'.
 */
BoolFct * parseBoolFct(std::istream & stream,
    const math::BoolFctFactoryPtr & factory);

} // namespace math

#endif	/* MATH_DIMACS_HPP */
