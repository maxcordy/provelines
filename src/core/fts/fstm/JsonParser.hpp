/*
 * JsonAdapter.hpp
 *
 * Created on 2013-07-29
 */

#ifndef CORE_FTS_FSTM_JSONADAPTER_HPP
#define CORE_FTS_FSTM_JSONADAPTER_HPP

#include "forwards.hpp"
#include "core/features/BoolFeatureExpFactory.hpp"
#include "util/Generator.hpp"
#include "util/parser/forwards.hpp"

#include <json/value.h>

#include <istream>

namespace core {
namespace fts {
namespace fstm {

// forward declarations
class FstmTransition;

/**
 * JsonParser is a parser for an FSTM in JSON format. It parses the FSTM
 * and returns its transitions as well as available information about the
 * FSTM.
 *
 * @author mwi
 */
class JsonParser {

private:
    const core::features::BoolFeatureExpFactory & featureExpFactory;
    /* The root value of the JSON FSTM after parsing. */
    Json::Value root;

public:
    /**
     * @effects Makes this be a new JSON adapter for the JSON FSTM
     *           'jsonFstm' with the boolean feature expression factory
     *           'featureExpFactory'.
     * @throws JsonException iff 'jsonFstm' contains some JSON syntax error.
     */
    JsonParser(std::istream & jsonFstm, const
        core::features::BoolFeatureExpFactory & featureExpFactory);

    /**
     * @return the number of transitions of the JSON FSTM.
     */
    int getNbOfTransitions() const;

    /**
     * @return a generator that will produce all the transitions of the
     *          JSON FSTM (as FstmTransitions), each exactly once, in the
     *          order they have in the parsed FSTM.
     */
    FstmTransitionGeneratorPtr transitions() const;

    /**
     * @return the number of LTL properties of the JSON FSTM.
     */
    int getNbOfProperties() const;

    /**
     * @return a generator that will produce all the LTL properties of the JSON
     *          FSTM (as strings), each exactly once, in the order they have in
     *          the parsed FSTM.
     */
    std::tr1::shared_ptr<util::Generator<std::string> > properties() const;

    /**
     * @return a generator that will produce all the states of the JSON FSTM
     *          (as strings), each exactly once, in the order they have in the
     *          parsed FSTM.
     */
    std::tr1::shared_ptr<util::Generator<std::string> > states() const;

private:
    /**
     * Generator for transitions of the JSON FSTM.
     */
    class TransitionGenerator : public util::Generator<FstmTransitionPtr> {

    private:
        const Json::Value & transitions;
        unsigned int index;
        const core::features::BoolFeatureExpFactory & featureExpFactory;

    public:
        /**
         * @effects Makes this be a new transition generator for the
         *           JSON transitions 'transitions' and with the boolean feature
         *           expression factory 'featureExpFactory'.
         */
        TransitionGenerator(const Json::Value & transitions, const
            core::features::BoolFeatureExpFactory & featureExpFactory);

        // The default destructor is fine.

        // overridden methods
        bool hasNext() const;
        FstmTransitionPtr next();

    private:
        /**
         * @requires boolExp != null
         * @return the Boolean expression 'boolExp' converted to a feature
         *          expression.
         */
        core::features::FeatureExpPtr toFeatureExp(const
            util::parser::BoolExpPtr & boolExp);
    }; // end TransitionGenerator

    /**
     * Generator for a JSON array of strings.
     */
    class StringArrayGenerator : public util::Generator<std::string> {

    private:
        const Json::Value & array;
        unsigned int index;

    public:
        /**
         * @effects Makes this be a new string array generator for the JSON
         *           array of strings 'array'.
         */
        StringArrayGenerator(const Json::Value & array);

        // The default destructor is fine.

        // overridden methods
        bool hasNext() const;
        std::string next();
    }; // end StringArrayGenerator

};

} // namespace fstm
} // namespace fts
} // namespace core

#endif /* CORE_FTS_FSTM_JSONADAPTER_HPP */
