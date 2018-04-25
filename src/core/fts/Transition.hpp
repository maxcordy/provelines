/*
 * Transition.hpp
 *
 * Created on 2013-07-22
 */

#ifndef CORE_FTS_TRANSITION_HPP
#define	CORE_FTS_TRANSITION_HPP

#include "core/features/forwards.hpp"

namespace core {
namespace fts {

/**
 * Transition is an interface for immutable transitions of featured
 * transition systems (FTS).
 *
 * FTS transitions are guarded by a feature expression.
 *
 * @author mwi
 */
class Transition {

public:
    /**
     * @return the feature expression of this.
     */
    virtual core::features::FeatureExpPtr getFeatureExp() const = 0;

    // mandatory virtual destructor
    virtual ~Transition() {}
};

} // namespace core
} // namespace fts

#endif	/* CORE_FTS_TRANSITION_HPP */
