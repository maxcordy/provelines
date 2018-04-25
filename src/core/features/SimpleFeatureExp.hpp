/*
 * SimpleFeatureExp.hpp
 *
 * Created on 2013-07-22
 */

#ifndef CORE_FEATURES_SIMPLEFEATUREEXP_HPP
#define	CORE_FEATURES_SIMPLEFEATUREEXP_HPP

#include <string>

namespace core {
namespace features {

    /**
     * SimpleFeatureExp is a simple interface for feature expressions that
     * allows them to be converted to a string.
     *
     * @author bdawagne
     */
    class SimpleFeatureExp {

    public:
        /**
         * @return a string representation of this.
         */
        virtual std::string toString() const = 0;

        // mandatory virtual destructor
        virtual ~SimpleFeatureExp() {}
    };

} // namespace feature
} // namespace core

#endif	/* CORE_FEATURES_SIMPLEFEATUREEXP_HPP */
