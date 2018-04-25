/*
 * FTS.hpp
 *
 * Created on 2013-07-22
 */

#ifndef CORE_FTS_FTS_HPP
#define	CORE_FTS_FTS_HPP

#include "forwards.hpp"
#include "util/Uncopyable.hpp"

namespace core {
namespace fts {

/**
 * FTS is an interface for a featured transition system (FTS) modeling a
 * variability-intensive system, e.g., a software product line (SPL).
 *
 * Specification Fields:
 *   - currentState : State          // The current state of the FTS.
 *   - initialState : bool           // Whether the FTS is in its initial state.
 *   - executables : Generator       // The remaining executable transitions
 *                                      leaving currentState.
 *   - propertyAutomaton : Automaton // The property automaton of the FTS, if
 *                                      any.
 *
 * Abstract Invariant:
 *   - initialState => executables is empty
 *
 * @author mwi
 */
class FTS : private util::Uncopyable {

public:
    /**
     * @requires transition != null && transition is a valid transition leaving
     *            this.currentState
     * @modifies this
     * @effects Sets this.currentState to the target state of 'transition',
     *           this.executables to the transitions leaving this.currentState
     *           and this.initialState to false (if was previously true).
     */
    virtual void apply(const TransitionPtr & transition) = 0;

    /**
     * @requires !this.initialState
     * @modifies this
     * @effects Reverts the last transition fired, i.e., sets this.currentState
     *           to the previously current state, this.executables to the
     *           previously remaining transitions and iff this is reverted to
     *           the initial state, sets this.initialState to true.
     * @return the reverted transition
     */
    virtual TransitionPtr revert() = 0;

    /**
     * @return this.currentState
     */
    virtual StatePtr getCurrentState() const = 0;

    /**
     * @return this.initialState
     */
    virtual bool isInitialState() const = 0;

    /**
     * @return true iff this.executables has more transitions to yield.
     */
    virtual bool hasNextTransition() const = 0;

    /**
     * @requires this.executables has more transitions to yield.
     * @modifies this
     * @effects Records the yield in this.executables.
     * @return the next transitions of this.executables.
     */
    virtual TransitionPtr nextTransition() = 0;

    /**
     * @modifies this
     * @effects Resets this.executables, i.e., sets this.executables to all the
     *           leaving transitions of this.currentState.
     */
    virtual void resetTransitions() = 0;

    /**
     * @return true iff this.propertyAutomaton != null
     */
    virtual bool hasProperty() const = 0;

    /**
     * @requires this.propertyAutomaton != null
     * @return this.propertyAutomaton
     */
    virtual PropertyAutomatonPtr getPropertyAutomaton() const = 0;
    
    /**
     * @return a pointer to a generator that will produce all the states
     *         contained in the current path, in the order they were visited
     *         (i.e. from the initial state to the current state).
     * @requires this must not be modified while the generator is in use
     */
    virtual std::auto_ptr<util::Generator<StatePtr> > getPath() const = 0;

    // mandatory virtual destructor
    virtual ~FTS() {}
};

} // namespace core
} // namespace fts

#endif	/* CORE_FTS_FTS_HPP */
