/*
 * File:   ClaimPropertyTransition.hpp
 *
 * Created on 2013-08-07
 */

#ifndef CORE_FTS_FSTM_CLAIMPROPERTYTRANSITION_HPP
#define	CORE_FTS_FSTM_CLAIMPROPERTYTRANSITION_HPP

#include "core/fts/PropertyTransition.hpp"
#include "neverclaim/forwards.hpp"
#include "util/parser/forwards.hpp"

namespace core {
namespace fts {
namespace fstm {

/**
 * ClaimPropertyTransition is an implementation of the PropertyTransition
 * interface that wraps a never claim transition.
 *
 * Specification Fields:
 *   - source : ClaimState      // The source state of the transition.
 *   - target : ClaimState      // The target state of the transition.
 *   - boolExp : BoolExp        // The boolean expression of the transition.
 *
 * @author mwi
 */
class ClaimPropertyTransition : public PropertyTransition {

private:
    const neverclaim::ClaimTransitionPtr claimTransition;

    /*
     * Representation Invariant:
     *   I(c) = c.claimTransition != null
     */

public:
    /**
     * @requires claimTransition != null
     * @effects Makes this be a new claim property transition with the never
     *           claim transition 'claimTransition'.
     */
    ClaimPropertyTransition(const neverclaim::ClaimTransitionPtr &
        claimTransition);

    // The default destructor is fine.

    /**
     * @return this.source
     */
    neverclaim::ClaimStatePtr getSource() const;

    /**
     * @return this.target
     */
    neverclaim::ClaimStatePtr getTarget() const;

    /**
     * @return this.boolExp
     */
    util::parser::BoolExpPtr getBoolExp() const;

private:
    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

} // namespace fstm
} // namespace fts
} // namespace core

#endif	/* CORE_FTS_FSTM_CLAIMPROPERTYTRANSITION_HPP */
