/*
 * forwards.hpp
 *
 * Created on 2013-08-06
 */

#ifndef CORE_FTS_FORWARDS_HPP
#define	CORE_FTS_FORWARDS_HPP

#include "util/Generator.hpp"

#include <tr1/memory>

namespace core {
namespace fts {

// Forward declarations
class FTS;
class PropertyAutomaton;
class PropertyState;
class PropertyTransition;
class State;
class Transition;

// Smart pointer typedefs
typedef std::tr1::shared_ptr<PropertyAutomaton> PropertyAutomatonPtr;
typedef std::tr1::shared_ptr<PropertyState> PropertyStatePtr;
typedef std::tr1::shared_ptr<PropertyTransition> PropertyTransitionPtr;
typedef std::tr1::shared_ptr<State> StatePtr;
typedef std::tr1::shared_ptr<Transition> TransitionPtr;

// Generator typedefs
typedef std::tr1::shared_ptr<FTS> FTSPtr;
typedef std::tr1::shared_ptr<util::Generator<PropertyTransitionPtr> >
    PropertyTransitionGeneratorPtr;
typedef std::tr1::shared_ptr<util::Generator<TransitionPtr> >
    TransitionGeneratorPtr;

} // namespace fts
} // namespace core

#endif	/* CORE_FTS_FORWARDS_HPP */
