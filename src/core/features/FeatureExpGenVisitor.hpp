/*
 * FeatureExpGenVisitor.hpp
 *
 * Created on 2013-08-28
 */

#ifndef CORE_FEATURES_FEATUREEXPGENVISITOR_HPP
#define	CORE_FEATURES_FEATUREEXPGENVISITOR_HPP

#include "forwards.hpp"
#include "BoolFeatureExpFactory.hpp"

#include "util/Stack.hpp"
#include "util/parser/Visitor.hpp"

namespace core {
namespace features {

/**
 * FeatureExpGenVisitor is an implementation of the Visitor interface. It visits
 * a Boolean expression to generate a feature expression out of it.
 *
 * @author mwi
 */
class FeatureExpGenVisitor : public util::parser::Visitor,
        private util::Uncopyable {

private:
    /* The feature expression factory used to produce the result. */
    const BoolFeatureExpFactory & featureExpFactory;
    /* Stores the generated feature expressions. */
    util::Stack<FeatureExpPtr> featureExpressions;

    /*
     * Representation Invariant:
     *   I(c) = true
     */

public:
    /**
     * @effects Makes this be a new FeatureExpGenVisitor with the boolean
     *           feature expression factory 'featureExpFactory'.
     */
    explicit FeatureExpGenVisitor(const BoolFeatureExpFactory &
        featureExpFactory);

    // The default destructor is fine.

    /**
     * @requires a Boolean expression has been visited
     * @return the generated feature expression.
     */
    FeatureExpPtr getFeatureExp() const;

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

} // namespace features
} // namespace core

#endif	/* CORE_FEATURES_FEATUREEXPGENVISITOR_HPP */
