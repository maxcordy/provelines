/*
 * TransitionTable.hpp
 *
 * Created on 2013-08-14
 */

#ifndef CORE_FTS_FSTM_TRANSITIONTABLE_HPP
#define CORE_FTS_FSTM_TRANSITIONTABLE_HPP

#include "forwards.hpp"
#include "core/fts/State.hpp"
#include "util/Generator.hpp"
#include "util/StdHashTable.hpp"
#include "util/Uncopyable.hpp"

#include <vector>

namespace core {
namespace fts {
namespace fstm {

/**
 * TransitionTable is an immutable table of FSTM transitions.
 *
 * @author mwi
 */
class TransitionTable : private util::Uncopyable {

private:
    /* A pair of indices <i1, i2> such that i1 <= i2 */
    typedef std::pair<unsigned int, unsigned int> IndexPair;

    std::vector<FstmTransitionPtr> transitionTable;
    /* Maps the source states from transitionTable with the indexes of the first
     * and last transitions leaving these source states.*/
    util::StdHashTable<State, IndexPair> indexTable;

    /*
     * Representation Invariant:
     *   I(c) = c.transitionTable.size > 0
     *     all the pointers in c.transitionTable are not null
     *     all distinct source states in c.transitionTable are in c.indexTable
     *       and no other state is in c.indexTable
     *     for all index pairs <i1, i2> in c.indexTable .(i1 <= i2)
     */

public:
    /**
     * @requires gen != null && gen generates at least one transition &&
     *            size > 0
     * @effects Makes this be a new transition table from the transition
     *           generator 'gen' with an initial size of 'size'.
     */
    TransitionTable(FstmTransitionGeneratorPtr gen, unsigned int size);

    // The default destructor is fine.

    /**
     * @return the initial state of this.
     */
    FstmStatePtr getInitialState() const;

    /**
     * @return a generator that will produce all the transitions (as
     *          FstmTransitions) leaving the state 'state' in this, each
     *          exactly once, in the order they appear in the FSTM.
     */
    TransitionGeneratorPtr executables(const FstmState & state) const;

private:
    /**
     * @requires gen != null && gen generates at least one transition
     * @modifies this
     * @effects Initializes the tables of this.
     */
    void initTables(FstmTransitionGeneratorPtr & gen);

    /**
     * @return the index of the first occurence of a transition leaving the
     *          given state in this, or -1 if this does not contain any
     *          transition leaving the given state.
     */
    int firstIndexOf(const FstmState & state) const;

    /**
     * @return the index of the last occurence of a transition leaving the
     *          given state in this, or -1 if this does not contain any
     *          transition leaving the given state.
     */
    int lastIndexOf(const FstmState & state) const;

    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;

    /**
     * Generator for transitions of the FSTM.
     */
    class TransitionGenerator : public util::Generator<TransitionPtr> {

    private:
        const std::vector<FstmTransitionPtr> & transitionTable;
        /* The first index of the transitions to generate. */
        unsigned int firstIndex;
        /* The last index of the transitions to generate. */
        unsigned int lastIndex;
        /* The index of the next transition to generate. */
        unsigned int curIndex;

        /*
         * Representation Invariant:
         *   I(c) = 0 <= c.firstIndex <= c.lastIndex &&
         *     c.firstIndex <= c.curIndex <= c.lastIndex + 1 &&
         *     c.lastIndex < c.transitionTable.size
         */

    public:
        /**
         * @requires 0 <= firstIndex <= lastIndex
         * @effects Makes this be a new transition generator for the
         *           transition table 'transitionTable' from
         *           transitionTable[firstIndex] to
         *           transitionTable[lastIndex].
         */
        TransitionGenerator(const std::vector<FstmTransitionPtr> &
            transitionTable, unsigned int firstIndex, unsigned int lastIndex);

        // The default destructor is fine.

        // overridden methods
        bool hasNext() const;
        TransitionPtr next();

    private:
        /**
         * @effects Asserts the rep invariant holds for this.
         */
        void checkRep() const;
    }; // end TransitionGenerator

    /**
     * Empty generator for transitions of the FSTM.
     */
    class EmptyTransitionGenerator : public util::Generator<TransitionPtr> {
    public:
        bool hasNext() const;
        TransitionPtr next();
    }; // end EmtpyTransitionGenerator

};

} // namespace fstm
} // namespace fts
} // namespace core

#endif /* CORE_FTS_FSTM_TRANSITIONTABLE_HPP */
