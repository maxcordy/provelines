/*
 * forwards.hpp
 *
 * Created on 2013-08-06
 */

#ifndef MATH_FORWARDS_HPP
#define	MATH_FORWARDS_HPP

#include <tr1/memory>

namespace math {

// Forward declarations
class BoolFct;
class BoolFctFactory;
class BoolVar;

// Smart pointer typedefs
typedef std::tr1::shared_ptr<BoolFct> BoolFctPtr;
typedef std::tr1::shared_ptr<BoolFctFactory> BoolFctFactoryPtr;

}

#endif	/* MATH_FORWARDS_HPP */
