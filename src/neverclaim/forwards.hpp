/*
 * forwards.hpp
 *
 * Created on 2013-08-04
 */

#ifndef NEVERCLAIM_FORWARDS_HPP
#define	NEVERCLAIM_FORWARDS_HPP

#include <memory>
#include <tr1/memory>

#include "util/Generator.hpp"

namespace neverclaim {

// Forward declarations
class ClaimState;
class ClaimTransition;
class NeverClaim;

// Smart pointer typedefs
typedef std::tr1::shared_ptr<ClaimState> ClaimStatePtr;
typedef std::tr1::shared_ptr<ClaimTransition> ClaimTransitionPtr;
typedef std::tr1::shared_ptr<NeverClaim> NeverClaimPtr;

// Generator typedefs
typedef std::auto_ptr<util::Generator<ClaimTransitionPtr> >
    ClaimTransitionGeneratorPtr;

} // namespace neverclaim

#endif	/* NEVERCLAIM_FORWARDS_HPP */
