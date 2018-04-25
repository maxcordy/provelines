/*
 * Exploration.hpp
 *
 * Created on 2013-08-22
 */

#ifndef CORE_CHECKER_EXPLORATION_HPP
#define	CORE_CHECKER_EXPLORATION_HPP

#include "CounterExample.hpp"
#include "StatePtrTable.hpp"

#include "core/fts/FTS.hpp"
#include "core/fts/State.hpp"
#include "core/fts/Transition.hpp"
#include "core/fts/forwards.hpp"

#include "core/features/BoolFeatureExp.hpp"
#include "core/features/BoolFeatureExpFactory.hpp"
#include "core/features/FeatureExp.hpp"

#include "util/Generator.hpp"
#include "util/Stack.hpp"
#include "util/Uncopyable.hpp"

#include <memory>
#include <tr1/memory>

namespace core {
namespace checker {

/**
 * An Exploration is a call to an exploration algorithms to model check a given
 * featured transition system against the presence of deadlocks.
 *
 * The arguments of the call must be given when constructing the algorithm.
 * The caller can then use the 'run' method to run the algorithm and retrieve
 * the results afterwards.
 *
 * @author bdawagne
 * @author mcr
 */
class Exploration : private util::Uncopyable {

private:

    /* Arguments */

    /** Feature Diagram */
    std::tr1::shared_ptr<core::features::FeatureExp> fd;

    /** Featured Transition System */
    std::tr1::shared_ptr<core::fts::FTS> fts;

    /** Factory for the feature expressions */
    std::tr1::shared_ptr<core::features::BoolFeatureExpFactory> expFactory;

    /* Results */

    /** Bad products expression, i.e. conjunction of all the feature
     *  expressions for which an error has been highlighted.
     */
    std::tr1::shared_ptr<core::features::FeatureExp> badProducts;

    /**
     * The set of counter examples for the bad products.
     */
    std::tr1::shared_ptr<util::List<CounterExample> > counterExamples;

    /* Private structures and classes */

    /** Structure for the table values
     *  @see NestedSearch#table
     */
    struct TableValue {
        unsigned int flags;
        std::tr1::shared_ptr<core::features::FeatureExp> features;
        TableValue() : flags(0), features() {};
    };

    /** Enumeration for the current state of the algorithm */
    enum AlgorithmState {
        Ready,
        Running,
        Finished
    };

    /* Flags*/

    /** The state has been visited */
    static const unsigned int VISITED =        1 << 0;

    /** The state is in the current path */
    static const unsigned int IN_PATH =        1 << 1;

    /* Global variables */

    /** Current state of the algorithm */
    AlgorithmState algorithmState;

    /** Valid products expression, i.e. the conjunction of the feature
     *  diagram and of the negation of the bad products expression.
     */
    std::auto_ptr<core::features::FeatureExp> validProducts;

    /** true iff the exploration is curently running */
    bool exploration;

    /** True iff the current path has led to highlight a deadlock */
    bool deadlockFound;

    /** Current state of the FTS */
    core::fts::StatePtr state;

    /** Next FTS transition */
    core::fts::TransitionPtr transition;

    /** Current feature expression */
    std::tr1::shared_ptr<core::features::FeatureExp> features;

    /** Stack containing the previous features expressions */
    util::Stack<std::tr1::shared_ptr<core::features::FeatureExp> > featuresStack;

    /** Table containing the visited states. */
    StatePtrTable<TableValue> table;

public:

    /**
     * @effets Makes this be a Exploration algorithm ready to verify the
     *         absence of deadlock in 'fts' according to the feature diagram
     *         'fd'.
     */
    Exploration(
            std::tr1::shared_ptr<core::features::FeatureExp> fd,
            std::tr1::shared_ptr<core::fts::FTS> fts,
            std::tr1::shared_ptr<core::features::BoolFeatureExpFactory> expFactory);

    // The default destructor is fine.

    /**
     * @requires this is ready
     * @modifies this
     * @effects Runs this.
     */
    void run();

    /**
     * @requires this has finished
     * @return A feature expression representing the set of products exhibiting
     *         deadlocks.
     */
    std::tr1::shared_ptr<core::features::FeatureExp> getBadProducts() const;

    /**
     * @requires this has finished
     * @return The set of counter examples for the bad products.
     */
    std::tr1::shared_ptr<util::List<CounterExample> > getCounterExamples() const;

private:
    /**
     * @return true iff 'features' is satisfiable wrt the valid products, i.e.
     *         iff the conjunction of 'features' and the valid products
     *         expression is satisfiable.
     */
    bool isSatisfiable(const core::features::FeatureExp & features) const;

    /**
     * @modifies this
     * @effects Reverts the previous transition and pop the previous features
     *          from the stack if possible, i.e. if the FTS is not is in initial
     *          state. End the exploration if not.
     */
    void backtrack();

    /**
     * @modifies this
     * @effects Looks for features exhibiting deadlocks in the current state of
     *          the FTS. If found, add them to the bad products and signal that
     *          a deadlock has been found.
     */
    void checkForDeadlock();

    /**
     * @modifies this
     * @effects Starts the exploration with full deadlock detection.
     */
    void startExploration();

//    /**
//     * @modifies this
//     * @effects Starts the exploration with partial deadlock detection.
//     */
//    void startLightExploration();

//    bool isSatisfiableWrtFD(const core::features::FeatureExpPtr & fd, const core::features::FeatureExpPtr & featureExp);

    /**
     * @effects Asserts the invariant representation holds for this.
     */
    void checkRep() const;
};

} // namespace checker
} // namespace core

#endif	/* CORE_CHECKER_EXPLORATION_HPP */
