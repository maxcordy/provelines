/*
 * File:   ClaimPropertyAutomaton.hpp
 *
 * Created on 2013-08-13
 */

#ifndef CORE_FTS_FSTM_CLAIMPROPERTYAUTOMATON_HPP
#define	CORE_FTS_FSTM_CLAIMPROPERTYAUTOMATON_HPP

#include "forwards.hpp"
#include "ChangeListener.hpp"
#include "core/fts/PropertyAutomaton.hpp"
#include "neverclaim/forwards.hpp"
#include "util/Stack.hpp"

#include <vector>

namespace core {
namespace fts {
namespace fstm {

/**
 * ClaimPropertyAutomaton is an implementation of the PropertyAutomaton
 * interface that wraps a never claim automaton.
 *
 * @author mwi
 */
class ClaimPropertyAutomaton : public PropertyAutomaton, public ChangeListener {

private:
    typedef std::tr1::shared_ptr<std::vector<ClaimPropertyTransitionPtr> >
        ClaimPropertyTransitionVectorPtr;

    /**
     * Snapshots capture the context of this and allow it to be reverted.
     */
    struct Snapshot {
        ClaimPropertyTransitionPtr applied;
        ClaimPropertyTransitionVectorPtr executables;
        unsigned int nextIndex;

        Snapshot(const ClaimPropertyTransitionPtr & applied,
            const ClaimPropertyTransitionVectorPtr & execs,
            unsigned int nextIndex)
            : applied(applied), executables(execs), nextIndex(nextIndex) {}
    }; // end Snapshot

    /**
     * IndexCache is a cache for an index.
     */
    class IndexCache {
    private:
        enum { UNSET_INDEX = -1 };
        int index;
    public:
        IndexCache() : index(UNSET_INDEX) {}
        void set(unsigned int i) { index = i; }
        void unset() { index = UNSET_INDEX; }
        int get() const { return index; }
        bool isSet() const { return index != UNSET_INDEX; }
    };

    typedef std::tr1::shared_ptr<Snapshot> SnapshotPtr;

    /* The never claim used by this. */
    const neverclaim::NeverClaimPtr neverClaim;
    /* The current state of the coupled FTS. Updated on stateChanged events. */
    std::string currentFtsState;
    /* The event of the last transition fired of the coupled FTS. Updated on
     * stateChanged events. */
    std::string currentFtsEvent;

    /* The current state of this. */
    neverclaim::ClaimStatePtr currentState;
    /* All the executable transitions leaving currentState. */
    ClaimPropertyTransitionVectorPtr executables;
    /* The next index of executables to consider. */
    unsigned int nextIndex;

    /* Fetched index, used to avoid repeating computations. It is unset each
     * time this is mutated. */
    mutable IndexCache fetchedIndex;

    /* Snapshots of the previous internal states (if any). */
    util::Stack<SnapshotPtr> snapshots;

    /*
     * Abstraction Function:
     *   AF(c) = property automaton a such that
     *     a.currentFtsState = c.currentFtsState
     *     a.currentState = c.currentState
     *     a.initialState = (c.snapshots.size == 0)
     *     a.transitions = [ti,...,tn] sublist of c.executables such that
     *       each ti is a PropertyTransition satisfying c.currentFtsState and
     *       i = c.nextIndex and
     *       n = c.executables.size - 1 and
     *       i <= n
     *
     * Representation Invariant:
     *   I(c) = c.neverClaim != null
     *     c.currentFtsState.length > 0
     *     c.currentState != null
     *     c.executables != null
     *     0 <= c.nextIndex <= c.executables.size
     *     (c.snapshots.size = 0) => c.currentState = c.neverClaim.initialState
     *     (c.snapshots.size > 0) =>
     *       c.currentState = c.snapshots.peek.applied.target
     */

public:
    /**
     * @requires ltl is a valid LTL formula using Spin syntax &&
     *            curFtsState.length > 0
     * @effects Makes this be a new claim property automaton for the LTL
     *           property 'ltl' and with the current FTS state 'curFtsState'.
     */
    ClaimPropertyAutomaton(const std::string & ltl,
            const std::string & curFtsState);

    // The default destructor is fine.

    // overridden methods
    void apply(const PropertyTransitionPtr & transition);
    PropertyTransitionPtr revert();
    PropertyStatePtr getCurrentState() const;
    bool isInitialState() const;
    bool hasNextTransition() const;
    PropertyTransitionPtr nextTransition();
    void resetTransitions();
    void stateChanged(const std::string & newState,
        const std::string & newEvent);

private:
    /**
     * @return the executable transitions of the state 'state'.
     */
    std::vector<ClaimPropertyTransitionPtr> * getExecutables(const
        neverclaim::ClaimState & state) const;

    /**
     * @return the index of this.executables.next if this.executables is not
     *          empty, else returns -1.
     */
    int getNextTransitionIndex() const;

    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

} // namespace fstm
} // namespace fts
} // namespace core

#endif	/* CORE_FTS_FSTM_CLAIMPROPERTYAUTOMATON_HPP */
