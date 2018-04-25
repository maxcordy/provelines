/*
 * FD.hpp
 *
 * Created on 2013-07-24
 */

#ifndef CORE_FD_FD_HPP
#define	CORE_FD_FD_HPP

#include "core/features/FeatureExp.hpp"

namespace core {
namespace fd {

/**
 * An FD is an interface for an immutable feature diagram, i.e., feature
 * model, that specifies the set of valid products of a software product
 * line (SPL).
 *
 * @author bdawagne
 */
class FD {

public:
    /**
     * @return this as a feature expression.
     */
    virtual core::features::FeatureExp * toFeatureExp() const = 0;

    // mandatory virtual destructor
    virtual ~FD() {}
};

} // namespace fd
} // namespace core

#endif	/* CORE_FD_FD_HPP */
