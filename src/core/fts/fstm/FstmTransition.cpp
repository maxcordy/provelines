/*
 * FstmTransition.cpp
 *
 * Created on 2013-07-26
 */

#include "FstmTransition.hpp"

#include "FstmEvent.hpp"
#include "FstmState.hpp"

#include "core/features/FeatureExp.hpp"

#include "util/Assert.hpp"
#include "util/Cloneable.hpp"

using core::features::FeatureExpPtr;

using std::string;
using std::tr1::shared_ptr;

namespace core {
namespace fts {
namespace fstm {

FstmTransition::FstmTransition(const string & source, const string &
        target, const string & event, const FeatureExpPtr & featureExp)
    : source(), target(), event(), featureExp(featureExp) {
    assertTrue(source.length() > 0);
    assertTrue(target.length() > 0);
    assertTrue(event.length() > 0);

    this->source = FstmState::makeState(source);
    this->target = FstmState::makeState(target);
    this->event = FstmEvent::makeEvent(event);

    checkRep();
}

FstmStatePtr FstmTransition::getSource() const {
    return source;
}

FstmStatePtr FstmTransition::getTarget() const {
    return target;
}

FstmEventPtr FstmTransition::getEvent() const {
    return event;
}

FeatureExpPtr FstmTransition::getFeatureExp() const {
    return FeatureExpPtr(featureExp->clone());
}

void FstmTransition::checkRep() const {
    if (doCheckRep) {
        assertTrue(source);
        assertTrue(target);
        assertTrue(event);
        assertTrue(featureExp);
    }
}

} // namespace fstm
} // namespace fts
} // namespace core
