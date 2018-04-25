/*
 * ClaimState.hpp
 *
 * Created on 2013-08-02
 */

#ifndef NEVERCLAIM_CLAIMSTATE_HPP
#define NEVERCLAIM_CLAIMSTATE_HPP

#include "forwards.hpp"
#include "util/Hashable.hpp"
#include "util/Uncopyable.hpp"

#include <string>
#include <tr1/memory>
#include <tr1/unordered_map>

namespace neverclaim {

/**
 * ClaimStates are immutable states of a never claim automaton, a.k.a., never
 * claim or claim automaton. An claim state has a label, and for each unique
 * label there is at most one claim state. A claim state can be accepting, in
 * which case it can also be final. There exists at most one final state. In
 * terms of Buchi acceptance, a final state is interpreted as the implicit
 * acceptance cycle.
 *
 * The label of a claim state is as follows:
 *   LABEL = ('accept_all') | (('accept_')? [A-Za-z0-9_]+)
 *
 * The label 'accept_all' identifies the final state. A label prefixed by
 * 'accept_' identifies an accepting state. Any other label represents a state
 * that is not accepting and thus not final.
 *
 * Specification Fields:
 *   - label : string   // The label of this claim state.
 *   - accepting : bool // Whether this is an accepting state.
 *   - final : bool     // Whether this is an accepting and final state.
 *
 * Abstract Invariant:
 *   label.length > 0 && final => accepting
 *
 * @author mwi
 */
class ClaimState : public util::Hashable<ClaimState>, private util::Uncopyable {

private:
    static const std::string Accept;
    static const std::string AcceptAll;
    static std::tr1::unordered_map<std::string,
        std::tr1::shared_ptr<ClaimState> > stateTable;

    const std::string label;

    /*
     * Abstraction Function:
     *   AF(c) = claim state s such that
     *     s.label = c.label
     *     s.accepting = (c.label starts with 'accept_')
     *     s.final = (c.label == 'accept_all')
     *
     * Representation Invariant:
     *   I(c) = LABEL(c.label)
     */

    /**
     * @requires LABEL(label)
     * @effects Makes this be a new claim state labeled 'label'.
     */
    explicit ClaimState(const std::string & label);

public:
    // The default destructor is fine.

    /**
     * @return this.label
     */
    const std::string & getLabel() const;

    /**
     * @return this.accepting
     */
    bool isAccepting() const;

    /**
     * @return this.final
     */
    bool isFinal() const;

    // overridden methods
    bool equals(const ClaimState & state) const;
    unsigned int hashCode() const;

    /**
     * @requires LABEL(label)
     * @return the claim state labeled 'label'.
     */
    static std::tr1::shared_ptr<ClaimState> makeState(const std::string &
        label);

private:
    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

} // namespace neverclaim

#endif /* NEVERCLAIM_CLAIMSTATE_HPP */
