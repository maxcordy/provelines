/*
 * Visitor.hpp
 *
 * Created on 2013-08-09
 */

#ifndef UTIL_PARSER_VISITOR_HPP
#define UTIL_PARSER_VISITOR_HPP

#include "forwards.hpp"

namespace util {
namespace parser {

/**
 * Visitor is an interface for a visitor of a Boolean expression, i.e., BoolExp.
 *
 * @see Visitor pattern
 * @author mwi
 */
class Visitor {

public:
    /**
     * @modifies this
     * @effects Anything
     */
    virtual void orExp(const OrExp & exp) = 0;

    /**
     * @modifies this
     * @effects Anything
     */
    virtual void andExp(const AndExp & exp) = 0;

    /**
     * @modifies this
     * @effects Anything
     */
    virtual void notExp(const NotExp & exp) = 0;

    /**
     * @modifies this
     * @effects Anything
     */
    virtual void boolValue(const BoolValue & val) = 0;

    /**
     * @modifies this
     * @effects Anything
     */
    virtual void var(const Var & v) = 0;

    // mandatory virtual destructor
    virtual ~Visitor() {}
};

} // namespace parser
} // namespace util

#endif /* UTIL_PARSER_VISITOR_HPP */
