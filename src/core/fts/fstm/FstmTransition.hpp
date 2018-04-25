/*
 * FstmTransition.hpp
 *
 * Created on 2013-07-26
 */

#ifndef CORE_FTS_FSTM_FSTMTRANSITION_HPP
#define CORE_FTS_FSTM_FSTMTRANSITION_HPP

#include "forwards.hpp"
#include "core/fts/Transition.hpp"
#include "util/Uncopyable.hpp"

namespace core {
namespace fts {
namespace fstm {

/**
 * FstmTransition is an immutable transition of an FSTM. In addition,
 * FstmTransition implements the Transition interface, which means it also
 * represents a transition of an FTS.
 *
 * Specification Fields:
 *   - source : FstmState      // The source state of the transition.
 *   - target : FstmState      // The target state of the transition.
 *   - event  : FstmEvent      // The event of the transition.
 *   - featureExp : FeatureExp // The feature expression of the transition.
 *
 * @author mwi
 */
class FstmTransition : public Transition, private util::Uncopyable {

private:
    FstmStatePtr source;
    FstmStatePtr target;
    FstmEventPtr event;
    const core::features::FeatureExpPtr featureExp;

    /*
     * Abstraction Function:
     *   AF(c) = FstmTransition t such that
     *     t.source = c.source
     *     t.target = c.target
     *     t.event = c.event
     *     t.featureExp = c.featureExp
     *
     * Representation Invariant:
     *   I(c) = c.source != null
     *     c.target != null
     *     c.event != null
     *     c.featureExp != null
     */

public:
    /**
     * @requires source, target, and event are not empty && featureExp
     *            is not null
     * @effects Makes this be a new FstmTransition t with t.source =
     *           FstmState(source), t.target = FstmState(target), t.event =
     *           FstmState(event) and t.featureExp = featureExp.
     */
    FstmTransition(const std::string & source, const std::string & target,
        const std::string & event,
        const core::features::FeatureExpPtr & featureExp);

    // The default destructor is fine.

    /**
     * @return this.source
     */
    FstmStatePtr getSource() const;

    /**
     * @return this.target
     */
    FstmStatePtr getTarget() const;

    /**
     * @return this.event
     */
    FstmEventPtr getEvent() const;

    /**
     * @return this.featureExp
     */
    core::features::FeatureExpPtr getFeatureExp() const;

private:
    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

} // namespace fstm
} // namespace fts
} // namespace core

#endif /* CORE_FTS_FSTM_FSTMTRANSITION_HPP */
