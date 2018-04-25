/*
 * CounterExample.hpp
 *
 * Created on 2013-08-28
 */

#ifndef CORE_CHECKER_COUNTEREXAMPLE_HPP
#define	CORE_CHECKER_COUNTEREXAMPLE_HPP

#include "core/features/forwards.hpp"
#include "core/fts/forwards.hpp"

#include "util/List.hpp"
#include "util/Generator.hpp"

#include <memory>
#include <tr1/memory>

namespace core {
namespace checker {

/**
 * A CounterExample is an immutable object containing both a feature expression
 * representing a set of products and a path in which these products ar shown to
 * exhibit errors with relation to a specified property.
 *
 * @author bdawagne
 * @author mwi
 */
class CounterExample {

private:
    util::List<core::fts::StatePtr> path;
    std::tr1::shared_ptr<core::features::FeatureExp> featureExp;
    mutable std::string minimizedFeatureExp;

    /*
     * Representation Invariant:
     *   I(c) = c.path.count() > 0
     *     c.features != null
     *     c.features != false
     */

public:
    /**
     * @requires states has at least one element to yield
     * @effects Makes this be a counter for the products 'features' exhibiting
     *           errors while following the path 'path'.
     */
    CounterExample(const core::features::FeatureExp & featureExp,
        const std::auto_ptr<util::Generator<core::fts::StatePtr> > & states);

    // The default destructor is fine.

    /**
     * @return a feature expression (as string) representing the set of
     *          products exhibiting errors.
     */
    std::string getFeatureExp() const;

    /**
     * @return the path in which the products are shown to exhibit errors.
     */
    std::auto_ptr<util::Generator<core::fts::StatePtr> > states() const;

    /**
     * @return a string representation of this.
     */
    std::string toString() const;

private:
    /**
     * @effect Assert the invariant representation holds for this.
     */
    void checkRep() const;
};

} // namespace checker
} // namespace core

#endif	/* CORE_CHECKER_COUNTEREXAMPLE_HPP */
