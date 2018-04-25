/*
 * FeatureExp.hpp
 *
 * Created on 2013-07-22
 */

#ifndef CORE_FEATURES_FEATUREEXP_HPP
#define	CORE_FEATURES_FEATUREEXP_HPP

#include "forwards.hpp"
#include "SimpleFeatureExp.hpp"
#include "util/Cloneable.hpp"

#include <string>

namespace core {
namespace features {

/**
 * FeatureExp is an interface for mutable feature expressions.
 *
 * Feature expressions can be invalidated. Operations can only be performed
 * on valid feature expressions.
 *
 * @author bdawagne
 */
class FeatureExp : public SimpleFeatureExp, public util::Cloneable<FeatureExp> {

public:
    /**
     * @requires this and expression are valid && expression != this
     * @modifies this, expression
     * @effects Sets this to the conjunction of this and 'expression', and
     *           invalidates 'expression'.
     * @return this
     */
    virtual FeatureExp * conjunction(FeatureExp & expression) = 0;

    /**
     * @requires this and expression are valid && expression != this
     * @modifies this, expression
     * @effects Sets this to the disjunction of this and 'expression', and
     *           invalidates 'expression'.
     * @return this
     */
    virtual FeatureExp * disjunction(FeatureExp & expression) = 0;

    /**
     * @requires this is valid
     * @modifies this
     * @effects Negates this, i.e., this_post = !this
     * @return this
     */
    virtual FeatureExp * negation() = 0;

    /**
     * @requires this is valid
     * @return true iff this is satisfiable.
     */
    virtual bool isSatisfiable() const = 0;

    /**
     * @requires this is valid
     * @return true iff this is a tautology.
     */
    virtual bool isTautology() const = 0;

    /**
     * @requires this and expression are valid
     * @return true iff this and 'expression' are logically equivalent,
     *          i.e., this <=> 'expression'.
     */
    virtual bool isEquivalent(const FeatureExp & expression) const = 0;

    /**
     * @requires this and expression are valid
     * @return true iff this implies 'expression', i.e.,
     *          this => 'expression'.
     */
    virtual bool implies(const FeatureExp & expression) const = 0;

    /**
     * @return true iff this is a valid feature expression.
     */
    virtual bool isValid() const = 0;

    // mandatory virtual destructor
    virtual ~FeatureExp() {}
};

} // namespace features
} // namespace core

#endif	/* CORE_FEATURES_FEATUREEXP_HPP */
