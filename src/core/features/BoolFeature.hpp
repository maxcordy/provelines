/*
 * BoolFeature.hpp
 *
 * Created on 2013-07-22
 */

#ifndef CORE_FEATURES_BOOLFEATURE_HPP
#define	CORE_FEATURES_BOOLFEATURE_HPP

#include "math/BoolVar.hpp"

#include <string>

namespace core {
namespace features {

    /**
     * BoolFeature is an immutable boolean feature with a name and an id.
     *
     * @author bdawagne
     */
    class BoolFeature {

    private:
        math::BoolVar variable;

        /*
         * Representation Invariant:
         *   I(c) = true
         */

        /**
         * @effects Makes this be a new boolean feature represented by the
         *           boolean variable 'variable'.
         */
        BoolFeature(math::BoolVar variable);

    public:
        /**
         * @return the id of this.
         */
        int getId() const;

        /**
         * @return the name of this.
         */
        std::string getName() const;

    private:
        /**
         * @effects Asserts the rep invariant holds for this.
         */
        void checkRep() const;
    };

} // namespace features
} // namespace core

#endif	/* CORE_FEATURES_BOOLFEATURE_HPP */
