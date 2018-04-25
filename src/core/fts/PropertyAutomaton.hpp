/*
 * PropertyAutomaton.hpp
 *
 * Created on 2013-08-13
 */

#ifndef CORE_FTS_PROPERTYAUTOMATON_HPP
#define	CORE_FTS_PROPERTYAUTOMATON_HPP

#include "forwards.hpp"
#include "util/Uncopyable.hpp"

namespace core {
namespace fts {

/**
 * PropertyAutomaton is an interface for an automaton used to verify a safety
 * or liveness temporal property. Such automata are usually BÃ¼chi automata.
 *
 * A property automaton is coupled to an FTS which is responsible to keep the
 * property automaton up-to-date regarding its internal changes.
 *
 * Specification Fields:
 *   - currentFtsState : State       // The current state of the associated FTS.
 *   - currentState : PropertyState  // The current state of the property
 *                                      automaton.
 *   - initialState : bool           // Whether the automaton is in its initial
 *                                      state.
 *   - transitions : Generator       // The remaining transitions leaving
 *                                      currentState and satisfying
 *                                      currentFtsState.
 *
 * Abstract Invariant:
 *   - initialState => transitions is empty
 *
 * @author mwi
 */
class PropertyAutomaton : private util::Uncopyable {

public:
    /**
     * @requires transition != null && transition is a valid transition leaving
     *            this.currentState
     * @modifies this
     * @effects Sets this.currentState to the target state of 'transition',
     *           this.transitions to the transitions leaving this.currentState
     *           and this.initialState to false (if was previously true).
     */
    virtual void apply(const PropertyTransitionPtr & transition) = 0;

    /**
     * @requires !this.initialState
     * @modifies this
     * @effects Reverts the last transition fired, i.e., sets this.currentState
     *           to the previously current state, this.transitions to the
     *           previously remaining transitions and iff this is reverted to
     *           the initial state, sets this.initialState to true.
     * @return the reverted transition
     */
    virtual PropertyTransitionPtr revert() = 0;

    /**
     * @return this.currentState
     */
    virtual PropertyStatePtr getCurrentState() const = 0;

    /**
     * @return this.initialState
     */
    virtual bool isInitialState() const = 0;

    /**
     * @return true iff this.transitions has more transitions to yield.
     */
    virtual bool hasNextTransition() const = 0;

    /**
     * @requires this.transitions has more transitions to yield.
     * @modifies this
     * @effects Records the yield in this.transitions.
     * @return the next transitions of this.transitions.
     */
    virtual PropertyTransitionPtr nextTransition() = 0;

    /**
     * @modifies this
     * @effects Resets this.transitions, i.e., sets this.transitions to all the
     *           leaving transitions of this.currentState.
     */
    virtual void resetTransitions() = 0;

    // mandatory virtual destructor
    virtual ~PropertyAutomaton() {}
};

} // namespace core
} // namespace fts

#endif	/* CORE_FTS_PROPERTYAUTOMATON_HPP */
