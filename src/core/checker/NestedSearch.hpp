/*
 * NestedSearch.hpp
 *
 * Created on August 14, 2013
 */

#ifndef NESTEDSEARCH_HPP
#define	NESTEDSEARCH_HPP

#include "CombinedStateTable.hpp"

#include "CombinedState.hpp"
#include "CounterExample.hpp"

#include "core/fts/FTS.hpp"
#include "core/fts/PropertyAutomaton.hpp"
#include "core/fts/PropertyState.hpp"
#include "core/fts/PropertyTransition.hpp"
#include "core/fts/State.hpp"
#include "core/fts/Transition.hpp"
#include "core/fts/forwards.hpp"

#include "core/features/BoolFeatureExp.hpp"
#include "core/features/BoolFeatureExpFactory.hpp"
#include "core/features/FeatureExp.hpp"

#include "util/List.hpp"
#include "util/Stack.hpp"
#include "util/Uncopyable.hpp"

#include <tr1/memory>

namespace core {
namespace checker {

/**
 * A NestedSearch is a call to a nested depth first search algorithms to model
 * check a given featured transition system against a given property.
 *
 * The arguments of the call must be given when constructing the algorithm.
 * The caller can then use the 'run' method to run the algorithm and retrieve
 * the results afterwards.
 *
 * @author bdawagne
 * @author mcr
 */
class NestedSearch : private util::Uncopyable {

private:

    /* Arguments */

    /** Feature Diagram */
    std::tr1::shared_ptr<core::features::FeatureExp> fd;

    /** Featured Transition System */
    std::tr1::shared_ptr<core::fts::FTS> fts;

    /** Büchi automaton */
    std::tr1::shared_ptr<core::fts::PropertyAutomaton> propertyAutomaton;

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
        std::tr1::shared_ptr<core::features::FeatureExp> innerFeatures;
        std::tr1::shared_ptr<core::features::FeatureExp> outerFeatures;
        TableValue() : flags(0), innerFeatures(), outerFeatures() {}
    };

    /** Enumeration for the current state of the algorithm */
    enum AlgorithmState {
        Ready,
        Running,
        Finished
    };

    /* Flags*/

    /** The state has been visited in the outer search */
    static const unsigned int VISITED_IN_OUTER =        1 << 0;

    /** The state has been visited in the inner search */
    static const unsigned int VISITED_IN_INNER =        1 << 1;

    /** The state is in the current outer search path */
    static const unsigned int IN_OUTER_PATH =           1 << 2;

    /** The state is in the current inner search path */
    static const unsigned int IN_INNER_PATH =           1 << 3;

    /* Global variables */

    /** Current state of the algorithm */
    AlgorithmState algorithmState;

    /** Valid products expression, i.e. the conjunction of the feature
     *  diagram and of the negation of the bad products expression.
     */
    std::auto_ptr<core::features::FeatureExp> validProducts;

    /** True iff the outer search is running */
    bool outerSearch;

    /** True iff the inner search is running */
    bool innerSearch;

    /** True iff the current path has led to highlight an error */
    bool errorFound;

    /** Current state of the FTS */
    core::fts::StatePtr state;

    /** Current state of the Büchi automaton */
    core::fts::PropertyStatePtr propertyState;

    /** Next FTS transition */
    core::fts::TransitionPtr transition;

    /** Next Büchi automaton transition */
    core::fts::PropertyTransitionPtr propertyTransition;

    /** Current feature expression */
    std::tr1::shared_ptr<core::features::FeatureExp> currentFeatures;

    /** Next feature expression, i.e the conjunction of the current features
     *  expression and of the feature expression labeling the next FTS
     *  transition.
     */
    std::tr1::shared_ptr<core::features::FeatureExp> nextFeatures;

    /** Stack containing the previous features expressions */
    util::Stack<std::tr1::shared_ptr<core::features::FeatureExp> > currentFeaturesStack;

    /** Stack containing the previous next feature expression.
     *  Note: The current feature expression can differ from the previous next
     *        feature expression. It can happen when the current state has
     *        already been visited before. In that case, the current features
     *        has been reduced to the non already verified features.
     */
    util::Stack<std::tr1::shared_ptr<core::features::FeatureExp> > nextFeaturesStack;

    /** Table containing the visited states. */
    CombinedStateTable<TableValue> table;

    /** Initial state of the inner search, when running */
    CombinedState innerSearchInitialState;


public:

    /**
     * @requires 'fts' has a property
     * @effets Makes this be a NestedSearch algorithm ready to verify the
     *         property of 'fts' according to the feature diagram 'fd'.
     */
    NestedSearch(
            std::tr1::shared_ptr<core::features::FeatureExp> fd,
            std::tr1::shared_ptr<core::fts::FTS> fts,
            std::tr1::shared_ptr<core::features::BoolFeatureExpFactory> expFactory);

    // Default destructor is fine

    /**
     * @requires this is ready
     * @modifies this
     * @effects Runs this.
     */
    void run();

    /**
     * @requires this has finished
     * @return A feature expression representing the set of products violating
     *         the specified property.
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
    bool isSatisfiable(const std::tr1::shared_ptr<core::features::FeatureExp> & features) const;

    /**
     * @modifies transition, nextFeatures
     * @effects Makes 'transition' be the next transition of the FTS leading
     *          to a satisfiable (wrt the valid products) feature expression,
     *          if such exists.
     *          Makes 'nextFeatures' be this latter feature expression.
     * @return true iff the mentioned transition has been found
     */
    bool findNextTransition(
        const std::tr1::shared_ptr<const core::features::FeatureExp> & features,
        core::fts::TransitionPtr & transition,
        std::tr1::shared_ptr<core::features::FeatureExp> & nextFeatures) const;

    /**
     * @modifies this
     * @effects Apply the next FTS transition.
     */
    void apply();

    /**
     * @modifies this
     * @effects Apply the next Büchi automaton transition.
     */
    void applyProperty();

    /**
     * @modifies this
     * @effects Revert the previous FTS transition.
     */
    void revert();

    /**
     * @modifies this
     * @effects Revert the previous Büchi automaton transition.
     */
    void revertProperty();

    /**
     * @modifies this
     * @effects Revert both the previous FTS transition and Büchi automaton
     *          transition
     */
    void revertAll();

    /**
     * @modifies this
     * @effects Push the current features and the next features into the
     *          stacks and updates the value of the current features, the next
     *          features and the next FTS transition.
     */
    void push(
        const std::tr1::shared_ptr<core::features::FeatureExp> & nextCurrentFeatures,
        const core::fts::TransitionPtr & nextTransition,
        const std::tr1::shared_ptr<core::features::FeatureExp> & nextNextFeatures);

    /**
     * @modifies this
     * @effects Pop the current features and the next features form the
     *          stacks and revert both the previous FTS transition and Büchi
     *          automaton transition.
     */
    void backtrack();

    /**
     * @modifies this
     * @effects Pop the current features and the next features from the
     *          stacks and revert both the previous FTS transition and Büchi
     *          automaton transition if possible. Stop the outer search if not.
     *          In any case, update the table to retrieve the current state from
     *          the path of the outer search.
     */
    void outerBacktrack();

    /**
     * @modifies this
     * @effects Pop the current features and the next features from the
     *          stacks and revert both the previous FTS transition and Büchi
     *          automaton transition, if possible. Stop the inner search if not.
     *          In any case, update the table to retrieve the current state from
     *          the path of the inner search.
     */
    void innerBacktrack();

    /**
     * @modifies this
     * @effects Adds the next features expression to the bad products.
     */
    void addToBadProducts();

    /**
     * @modifies this
     * @effects Signal a deadlock. Currently, only stop the execution asserting
     *          false.
     */
    void deadlock();

    /**
     * @modifies this
     * @effects Starts the outer search.
     */
    void startOuterSearch();

    /**
     * @modifies this
     * @effects Starts the inner search.
     */
    void startInnerSearch();

    /**
     * @effects Asserts the invariant representation holds for this.
     */
    void checkRep() const;
};

} // namespace checker
} // namespace core

#endif	/* NESTEDSEARCH_HPP */

