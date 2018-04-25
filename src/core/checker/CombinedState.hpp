/* 
 * CombinedState.hpp
 *
 * Created on August 14, 2013
 */

#ifndef COMBINEDSTATE_HPP
#define	COMBINEDSTATE_HPP

#include "core/fts/State.hpp"
#include "core/fts/PropertyState.hpp"
#include "core/fts/forwards.hpp"

#include "util/Hashable.hpp"

#include <string>

namespace core {
namespace checker {

/**
 * A CombinedState is an immutable object representing a state of the cross
 * product between a Featured Transition System (FTS) and a Büchi automaton.
 * 
 * It is formally composed of one state of each of the two mentioned structure.
 * 
 * @bdawagne
 */    
class CombinedState : public util::Hashable<CombinedState> {
    
private:
    core::fts::StatePtr state;
    core::fts::PropertyStatePtr propertyState;
    
    /*
     * Invariant Representation :
     *      I(c) = true;
     */
    
public:
    /**
     * @effects Make this be an empty CombinedState. 
     */
    CombinedState();
    
    /**
     * @effects Make this be the CombinedState composed of 'state' and
     *          'propertyState'. 
     */
    CombinedState(const core::fts::StatePtr state,
            const core::fts::PropertyStatePtr propertyState);
    
    // Default destructor is fine
    
    /**
     * @return a string representation of this
     */
    std::string toString() const;
    
    // Overridden methods
    unsigned int hashCode() const;
    bool equals(const CombinedState & other) const;
    
private:
    /**
     * @effects Assert the invariant representation holds for this.
     */
    void checkRep() const;

};

} // namespace checker
} // namespace core

#endif	/* COMBINEDSTATE_HPP */

