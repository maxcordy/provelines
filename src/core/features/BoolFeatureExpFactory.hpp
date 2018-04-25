/*
 * BoolFeatureExpFactory.hpp
 *
 * Created on 2013-07-26
 */

#ifndef CORE_FEATURES_BOOLFEATUREEXPFACTORY_HPP
#define CORE_FEATURES_BOOLFEATUREEXPFACTORY_HPP

#include "forwards.hpp"
#include "math/forwards.hpp"

#include "util/Uncopyable.hpp"

#include <string>

namespace core {
namespace features {

/**
 * BoolFeatureExpFactory is a factory that produces boolean feature
 * expressions.
 *
 * @author mwi
 */
class BoolFeatureExpFactory : private util::Uncopyable {

private:
    std::tr1::shared_ptr<math::BoolFctFactory> functionFactory;

public:
    /**
     * @requires boolFctFactory != null
     * @effects Makes this be a new BoolFeatureExpFactory with the
     *           boolean function factory 'boolFctFactory'.
     */
    BoolFeatureExpFactory(const std::tr1::shared_ptr<math::BoolFctFactory> &
        boolFctFactory);

    /**
     * @return a new boolean feature expression for a feature named
     *          'featureName'.
     */
    BoolFeatureExp * create(const std::string & featureName) const;

    /**
     * @requires boolFct != null
     * @return a new boolean feature expression for the boolean function
     *         'boolFct'.
     */
    BoolFeatureExp * create(math::BoolFct * boolFct) const;

    /**
     * @return a tautology
     */
    BoolFeatureExp * getTrue() const;

    /**
     * @return an unsatisfiable boolean feature expression
     */
    BoolFeatureExp * getFalse() const;
};

} // namespace features
} // namespace core

#endif /* CORE_FEATURES_BOOLFEATUREEXPFACTORY_HPP */
