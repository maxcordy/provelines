/*
 * ClaimPropertyState.hpp
 *
 * Created on 2013-08-13
 */

#ifndef CORE_FTS_FSTM_CLAIMPROPERTYSTATE_HPP
#define	CORE_FTS_FSTM_CLAIMPROPERTYSTATE_HPP

#include "core/fts/PropertyState.hpp"
#include "neverclaim/forwards.hpp"
#include "core/fts/forwards.hpp"

#include <string>
#include <tr1/unordered_map>

namespace core {
namespace fts {
namespace fstm {

/**
 * ClaimPropertyState is an implementation of the PropertyState interface that
 * wraps a never claim state, and for each unique never claim state there is at
 * most one claim property state.
 *
 * @author mwi
 */
class ClaimPropertyState : public PropertyState {

private:
    static std::tr1::unordered_map<unsigned int, core::fts::PropertyStatePtr> stateTable;

    const neverclaim::ClaimStatePtr claimState;

    /*
     * Representation Invariant:
     *   I(c) = c.claimState != null
     */

    /**
     * @requires claimState != null
     * @effects Makes this be a new claim property state with the claim state
     *           'claimState'.
     */
    ClaimPropertyState(const neverclaim::ClaimStatePtr & claimState);

public:
    // The default destructor is fine.

    // overridden methods
    const std::string & getLabel() const;
    bool isAccepting() const;
    bool isFinal() const;
    bool equals(const PropertyState & obj) const;
    unsigned int hashCode() const;

    /**
     * @requires claimState != null
     * @return the property state wrapping the claim state 'claimState'.
     */
    static core::fts::PropertyStatePtr makePropertyState(const neverclaim::ClaimStatePtr
        & claimState);

private:
    /**
     * @effects Asserts the rep invariant holds for this
     */
    void checkRep() const;
};

} // namespace fstm
} // namespace fts
} // namespace core

#endif	/* CORE_FTS_FSTM_CLAIMPROPERTYSTATE_HPP */
