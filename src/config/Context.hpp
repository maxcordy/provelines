/*
 * Context.hpp
 *
 * Created on 2013-O8-02
 */

#ifndef CONFIG_CONTEXT_HPP
#define CONFIG_CONTEXT_HPP

#include "core/fts/forwards.hpp"
#include "core/features/forwards.hpp"
#include "math/forwards.hpp"

namespace config {

/**
 * Context is the context of an execution of ProVeLines. It results from a
 * configuration and provides access to the appropriate factories and
 *
 * @author mwi
 */
class Context {

private:
    math::BoolFctFactoryPtr boolFctFactory;
    core::features::BoolFeatureExpFactoryPtr boolFeatureExpFactory;

    /*
     * Representation Invariant:
     *   I(c) = c.boolFctFactory != null
     *     c.boolFeatureExpFactory != null
     */

public:
    /**
     * @effects Makes this be a new context with a default configuration.
     * @throw NotPossibleException if the configuration is invalid.
     */
    Context();

    // The default destructor is fine.

    // The default copy constructor is fine.

    // The default assignement operator is fine.

    /**
     * @return the boolean function factory of this.
     */
    math::BoolFctFactoryPtr getBoolFctFactory() const;

    /**
     * @return the boolean feature expression factory of this.
     */
    core::features::BoolFeatureExpFactoryPtr getBoolFeatureExpFactory() const;

    /**
     * @requires jsonFstm is a valid FSTM in valid JSON
     * @return a new FTS for the FSTM represented by 'jsonFstm'.
     * @throws JsonException iff 'jsonFstm' contains some syntax error.
     */
    core::fts::FTSPtr createFSTM(std::istream & jsonFstm) const;

    /**
     * @return a new feature expression representing an empty feature diagram
     *          (FD).
     */
    core::features::FeatureExpPtr createEmptyFD() const;

private:
    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

} // namespace config

#endif /* CONFIG_CONTEXT_HPP */
