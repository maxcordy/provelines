/*
 * ClaimTransition.hpp
 */

#ifndef NEVERCLAIM_CLAIMTRANSITION_HPP
#define NEVERCLAIM_CLAIMTRANSITION_HPP

#include "forwards.hpp"
#include "util/Uncopyable.hpp"
#include "util/parser/forwards.hpp"

namespace neverclaim {

/**
 * ClaimTransition is an immutable transition of a never claim automaton.
 *
 * Specification Fields:
 *   - source : ClaimState      // The source state of the transition.
 *   - target : ClaimState      // The target state of the transition.
 *   - boolExp : BoolExp        // The Boolean expression of the transition.
 *
 * @note The first letter of the variables of the Boolean expression are in
 *        lowercase.
 *
 * @author mwi
 */
class ClaimTransition : private util::Uncopyable {

private:
    std::tr1::shared_ptr<ClaimState> source;
    std::tr1::shared_ptr<ClaimState> target;
    std::tr1::shared_ptr<util::parser::BoolExp> boolExp;

    /*
     * Abstraction Function:
     *   AF(c) = claim transition t such that
     *     t.source = c.source
     *     t.target = c.target
     *     t.boolExp = c.boolExp;
     *
     * Representation Invariant:
     *   I(c) = c.source != null
     *     c.target != null
     *     c.boolExp != null
     */

public:
    /**
     * @requires source and target are valid claim states && boolExp != null
     * @effects Makes this be a new claim transition t with t.source =
     *           ClaimState(source), t.target = ClaimState(target) and
     *           t.boolExp = boolExp.
     */
    ClaimTransition(const std::string & source, const std::string & target,
        const std::tr1::shared_ptr<util::parser::BoolExp> & boolExp);

    // The default destructor is fine.

    /**
     * @return this.source
     */
    std::tr1::shared_ptr<ClaimState> getSource() const;

    /**
     * @return this.target
     */
    std::tr1::shared_ptr<ClaimState> getTarget() const;

    /**
     * @return this.boolExp
     */
    std::tr1::shared_ptr<util::parser::BoolExp> getBoolExp() const;

private:
    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

} // namespace neverclaim

#endif
