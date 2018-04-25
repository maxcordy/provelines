/*
 * forwards.hpp
 *
 * Created on 2013-08-13
 */

#ifndef UTIL_PARSER_FORWARDS_HPP
#define	UTIL_PARSER_FORWARDS_HPP

#include <tr1/memory>

namespace util {
namespace parser {

// Forward declarations
class AndExp;
class BinExp;
class BoolExp;
class BoolValue;
class EvalVisitor;
class NotExp;
class OrExp;
class Var;
class Visitor;

// Smart pointer typedefs
typedef std::tr1::shared_ptr<BoolExp> BoolExpPtr;
typedef std::tr1::shared_ptr<Visitor> VisitorPtr;

} // namespace parser
} // namespace forward


#endif	/* UTIL_PARSER_FORWARDS_HPP */
