/*
 * MinimizeGenVisitor.hpp
 *
 * Created on 2013-08-29
 */

#ifndef MATH_MINIMIZE_MINIMIZEGENVISITOR_HPP
#define	MATH_MINIMIZE_MINIMIZEGENVISITOR_HPP

#include "forwards.hpp"

#include "util/Stack.hpp"
#include "util/parser/Visitor.hpp"

#include <string>

namespace math {
namespace minimize {

/**
 * MinimizeGenVisitor is an implementation of the Visitor interface. It visits
 * a Boolean expression to generate a Minimize expression out of it.
 *
 * A Minimize expression is a Boolean expression with an appropriate syntax for
 * the Minimize program.
 *
 * @author mwi
 */
class MinimizeGenVisitor : public util::parser::Visitor,
        private util::Uncopyable {

private:
    /* Stores the generated Minimize expressions. */
    util::Stack<std::string> expressions;
    VarTablePtr varTable;
    bool aborted;

    /*
     * Representation Invariant:
     *   I(c) = varTable != null
     */

public:
    /**
     * @effects Makes this be a new MinimizeGenVisitor.
     */
    MinimizeGenVisitor();

    // The default destructor is fine.

    /**
     * @requires a Boolean expression has been visited
     * @return the generated Minimize expression.
     */
    std::string getMinimizeExp() const;

    /**
     * @return the variable table.
     */
    const VarTablePtr getVarTable() const;

    /**
     * @return true iff the visit has been aborted due to too many distinct
     *          variables.
     */
    bool isAborted() const;

    // overridden methods
    void orExp(const util::parser::OrExp & exp);
    void andExp(const util::parser::AndExp & exp);
    void notExp(const util::parser::NotExp & exp);
    void boolValue(const util::parser::BoolValue & val);
    void var(const util::parser::Var & v);

private:
    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

} // namespace minimize
} // namespace math

#endif	/* MATH_MINIMIZE_MINIMIZEGENVISITOR_HPP */
