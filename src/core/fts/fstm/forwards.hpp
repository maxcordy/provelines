/*
 * forwards.hpp
 *
 * Created on 2013-08-06
 */

#ifndef CORE_FTS_FSTM_FORWARDS_HPP
#define	CORE_FTS_FSTM_FORWARDS_HPP

#include "util/Generator.hpp"

#include <tr1/memory>

namespace core {
namespace fts {
namespace fstm {

// Forward declarations
class ClaimPropertyState;
class ClaimPropertyTransition;
class ChangeListener;
class FstmEvent;
class FstmState;
class FstmTransition;
class TransitionTable;

// Smart pointer typedefs
typedef std::tr1::shared_ptr<ClaimPropertyState> ClaimPropertyStatePtr;
typedef std::tr1::shared_ptr<ClaimPropertyTransition>
        ClaimPropertyTransitionPtr;
typedef std::tr1::shared_ptr<ChangeListener> ChangeListenerPtr;
typedef std::tr1::shared_ptr<FstmEvent> FstmEventPtr;
typedef std::tr1::shared_ptr<FstmState> FstmStatePtr;
typedef std::tr1::shared_ptr<FstmTransition> FstmTransitionPtr;
typedef std::tr1::shared_ptr<TransitionTable> TransitionTablePtr;

// Generator typedefs
typedef std::tr1::shared_ptr<util::Generator<FstmTransitionPtr> >
    FstmTransitionGeneratorPtr;

} // namespace fstm
} // namespace fts
} // namespace core

#endif	/* CORE_FTS_FSTM_FORWARDS_HPP */
