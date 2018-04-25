/*
 * FSTM.hpp
 *
 * Created on 2013-07-30
 */

#ifndef CORE_FTS_FSTM_FSTM_HPP
#define CORE_FTS_FSTM_FSTM_HPP

#include "forwards.hpp"
#include "core/features/BoolFeatureExpFactory.hpp"
#include "core/fts/FTS.hpp"
#include "util/Stack.hpp"
#include "util/Generator.hpp"

#include <memory>
#include <istream>

namespace core {
namespace fts {
namespace fstm {

/**
 * FSTM is an immutable featured state machine. It provides access to its
 * transitions produced after it has been flattened.
 *
 * Because FSTM implements the FTS interface, it can also be considered as an
 * FTS. Actually FSTM have the semantics of FTS.
 *
 * @author mwi
 */
class FSTM : public FTS {

private:
    class Snapshot;
    typedef std::tr1::shared_ptr<Snapshot> SnapshotPtr;

    /* Boolean feature expression factory. */
    const core::features::BoolFeatureExpFactory & featureExpFactory;

    TransitionTablePtr transitionTable;
    PropertyAutomatonPtr propertyAutomaton;
    ChangeListenerPtr changeListener;

    FstmStatePtr currentState;
    TransitionGeneratorPtr executables;

    /* Snapshots of the previous internal states (if any). */
    util::Stack<SnapshotPtr> snapshots;

    /*
     * Abstraction Function:
     *   AF(c) = FTS fts such that
     *     fts.currentState = c.currentState
     *     fts.initialState = (c.snapshots.size == 0)
     *     fts.executables = c.executables
     *     fts.propertyAutomaton = c.propertyAutomaton
     *
     * Representation Invariant:
     *   I(c) = c.transitionTable != null
     *     c.currentState != null
     *     c.executables != null
     *     (c.snapshots.size = 0) =>
     *       c.currentState = c.transitionTable.initialState
     *     (c.snapshots.size > 0) =>
     *       c.currentState = c.snapshots.peek.applied.target
     *     (propertyAutomaton != null) <=> (changeListener != null)
     */

public:
    /**
     * @requires jsonFstm represents an FSTM that contains at least one
     *            transition.
     * @effects Makes this be a new FSTM from the JSON FSTM 'jsonFSTM' and
     *           with the boolean feature expression factory
     *           'featureExpFactory'.
     * @throws JsonException iff 'jsonFstm' contains some JSON syntax error.
     */
    FSTM(std::istream & jsonFstm, const core::features::BoolFeatureExpFactory &
        featureExpFactory);

    // The default destructor is fine.

    // overridden methods
    void apply(const TransitionPtr & transition);
    TransitionPtr revert();
    StatePtr getCurrentState() const;
    bool isInitialState() const;
    bool hasNextTransition() const;
    TransitionPtr nextTransition();
    void resetTransitions();
    bool hasProperty() const;
    PropertyAutomatonPtr getPropertyAutomaton() const;
    std::auto_ptr<util::Generator<StatePtr> > getPath() const;

private:
    /**
     * @modifies this
     * @effects Fires a state changed event to the registered listener
     *           (if any) with the event named 'eventName'.
     */
    void fireStateChangedEvent(const std::string & eventName);

    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;

    /**
     * Snapshots capture the context of this and allow it to be reverted.
     */
    struct Snapshot {
        FstmTransitionPtr applied;
        TransitionGeneratorPtr executables;

        Snapshot(const FstmTransitionPtr & applied,
            const TransitionGeneratorPtr & execs)
            : applied(applied), executables(execs) {}
    }; // end Snapshot

    /**
     * A PathGenerator is a generator that generates the path (i.e. the sequence
     * of states) which has led from the initial state to the current state.
     */
    class PathGenerator : public util::Generator<StatePtr> {

    private:
        std::auto_ptr<util::Generator<SnapshotPtr> > stackGen;
        StatePtr lastState;
        bool hasNextState;

    public:
        /**
         * @effects Makes this be the the path generator following all the
         *          transition in 'snapshotGen' until reaching the last state
         *          'lastState'. If stackGen is empty, only generate
         *          'lastState'.
         */
        PathGenerator(std::auto_ptr<util::Generator<SnapshotPtr> > snapshotGen,
                StatePtr lastState);

        // The default destructor is fine.

        // overridden methods
        bool hasNext() const;
        StatePtr next();
    }; // end PathGenerator

};

} // namespace fstm
} // namespace fts
} // namespace core

#endif /* CORE_FTS_FSTM_FSTM_HPP */
