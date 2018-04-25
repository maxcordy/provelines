/*
 * BoolFct.hpp
 *
 * Created on 2013-07-16
 */

#ifndef MATH_BOOLFCT_HPP
#define MATH_BOOLFCT_HPP

#include "forwards.hpp"
#include "util/Cloneable.hpp"

#include <string>

namespace math {

/**
 * BoolFct is an interface for a mutable boolean function.
 *
 * A boolean function can be invalidated. Operations can only be performed
 * on valid boolean function.
 *
 * @author bdawagne
 */
class BoolFct : public util::Cloneable<BoolFct> {

public:
    /**
     * @requires this and expression are valid && expression != this
     * @modifies this, expression
     * @effects Sets this to the conjunction of this and 'expression', and
     *           invalidates 'expression'.
     * @return this
     */
    virtual BoolFct & conjunction(BoolFct & formula) = 0;

    /**
     * @requires this and expression are valid && expression != this
     * @modifies this, expression
     * @effects Sets this to the disjunction of this and 'expression', and
     *           invalidates 'expression'.
     * @return this
     */
    virtual BoolFct & disjunction(BoolFct & formula) = 0;

    /**
     * @requires this is valid
     * @modifies this
     * @effects Negates this, i.e., this_post = !this
     * @return this
     */
    virtual BoolFct & negation() = 0;

    /**
     * @requires this is valid
     * @return true iff this is satisfiable
     */
    virtual bool isSatisfiable() const = 0;

    /**
     * @requires this is valid
     * @return true iff this is a tautology
     */
    virtual bool isTautology() const = 0;

    /**
     * @requires this is valid &&
     *           'formula' is valid
     * @return true if this and 'formula' are logically equivalent
     *         (i.e. iff this <=> 'formula')
     */
    virtual bool isEquivalent(const BoolFct & formula) const = 0;

    /**
     * @requires this is valid &&
     *           'formula' is valid
     * @return true iff this implies 'formula' (i.e. iff this => 'formula')
     */
    virtual bool implies(const BoolFct & formula) const = 0;

    /**
     * @return true iff this is a valid boolean function
     */
    virtual bool isValid() const = 0;

    /**
     * @return a string representation of this.
     */
    virtual std::string toString() const = 0;

    // mandatory virtual destructor
    virtual ~BoolFct() {}
};

} // namespace math

#endif /* MATH_BOOLFCT_HPP */
