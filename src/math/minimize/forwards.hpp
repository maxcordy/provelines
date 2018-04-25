/*
 * forwards.hpp
 *
 * Created on 2013-08-29
 */

#ifndef MATH_MINIMIZE_FORWARDS_HPP
#define	MATH_MINIMIZE_FORWARDS_HPP

#include <tr1/memory>

namespace math {
namespace minimize {

// Forward declarations
class VarTable;

// Smart pointer typedefs
typedef std::tr1::shared_ptr<VarTable> VarTablePtr;

} // namespace minimize
} // namespace math

#endif	/* MATH_MINIMIZE_FORWARDS_HPP */
