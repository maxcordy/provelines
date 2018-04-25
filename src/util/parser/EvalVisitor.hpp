/*
 * EvalVisitor.hpp
 *
 * Created on 2013-08-09
 */

#ifndef UTIL_PARSER_EVALVISITOR_HPP
#define	UTIL_PARSER_EVALVISITOR_HPP

#include "Visitor.hpp"

namespace util {
namespace parser {

/**
 * EvalVisitor is an interface for a visitor that evaluates a Boolean
 * expression.
 *
 * @author mwi
 */
class EvalVisitor : public Visitor {

public:
    /**
     * @modifies this
     * @effects Resets this, i.e., deletes all the previous evaluation results.
     */
    virtual void reset() = 0;

    /**
     * @requires at least one Boolean expression was evaluated
     * @return true iff the last visited Boolean expression was evaluated to
     *          true.
     */
    virtual bool isTrue() const = 0;

    // mandatory virtual destructor
    virtual ~EvalVisitor() {}
};

} // namespace parser
} // namespace util

#endif	/* UTIL_PARSER_EVALVISITOR_HPP */
