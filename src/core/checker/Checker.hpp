/*
 * Checker.hpp
 *
 * Created on 2013-08-27
 */

#ifndef CHECKER_HPP
#define	CHECKER_HPP

#include "core/checker/CounterExample.hpp"

#include "config/Context.hpp"

#include "core/fts/FTS.hpp"

#include "core/features/FeatureExp.hpp"
#include "core/features/SimpleFeatureExp.hpp"
#include "core/features/BoolFeatureExpFactory.hpp"

#include "util/Uncopyable.hpp"
#include "util/List.hpp"
#include "util/Generator.hpp"

#include <string>
#include <memory>
#include <tr1/memory>

namespace core {
namespace checker {

/**
 * A checker is an object able to verify an FTS model against the presence of
 * deadlocks or any LTL property.
 *
 * Currently, a checker only accept an json FSTM model file path as input.
 * If a property is specified within that model, the checker runs a nested
 * depth-first search algorithm in order to verify it. In that case, the model
 * should not contain any deadlock.
 * If no property is specified within the model, the Checker runs an exploration
 * algorithm in order to verify the absence of deadlock.
 *
 * Specification fields:
 * - state  : InternalState            // The current state of the checker.
 * - result : Result                   // The result of the model checking.
 *
 * @author bdawagne
 * @author mwi
 */
class Checker : private util::Uncopyable {

public:
    /**
     * The possible results of the model checking.
     */
    enum Result {
        /**
         * No deadlock has been found. If a property has been specified, it
         * has been successfully verified by all products.
         */
        Satisfied,

        /**
         * No deadlock has been found. A property has been specified and some
         * products exhibit errors.
         */
        Violated,

        /**
         * Some products exhibit deadlocks.
         */
        Error
    };

private:
    /**
     * The possible internal states of the checker.
     */
    enum InternalState {idle, Running, Terminated};

    config::Context ctx;
    std::tr1::shared_ptr<core::features::BoolFeatureExpFactory> factory;
    std::tr1::shared_ptr<core::features::FeatureExp> fd;
    std::tr1::shared_ptr<core::fts::FTS> fts;
    InternalState state;
    Result result;
    std::tr1::shared_ptr<core::features::FeatureExp> violatedProducts;
    std::tr1::shared_ptr<util::List<CounterExample> > counterExamples;

    /**
     * Representation invariant:
     *
     *     I(c) = (
     *         (violatedProducts != NULL) <=> (state = Terminated)
     *         factory != NULL
     *         fd != NULL
     *         fts != NULL
     *     )
     *
     */

public:
    /**
     * @effects Makes this.state = idle
     * @throws NotPossibleException iff 'fstmFilePath' is an invalid file path
     *          or the file at 'fstmFilePath' does not contain  some valid JSON
     *          code.
     */
    Checker(const std::string & fstmFilePath);

    /**
     * @modifies this
     * @effects First, makes this.state = Running.
     *          Then, makes this.state = Terminated.
     */
    void run();

    /**
     * @return true iff this.state = Terminated
     */
    bool isTerminated() const;

    /**
     * @requires this.state = Terminated
     * @return this.result
     */
    Result getResult() const;

    /**
     * @requires this.state = Terminated
     * @return a feature expression (as string) representing the set of
     *          products exhibiting errors or deadlocks.
     */
    std::string getViolatedProducts() const;

    /**
     * @requires this.state = Terminated
     * @return The set of counter examples for the violated products.
     */
    std::auto_ptr<util::Generator<CounterExample> > getCounterExamples()
        const;

private:
    /**
     * @effects assert the representation invariant holds for this
     */
    void checkRep() const;
};

} // namespace checker
} // namespace core

#endif	/* CHECKER_HPP */
