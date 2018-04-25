/*
 * FstmEvalVisitor.hpp
 *
 * Created on 2013-08-09
 */

#ifndef CORE_FTS_FSTM_FSTMEVALVISITOR_HPP
#define	CORE_FTS_FSTM_FSTMEVALVISITOR_HPP

#include "util/Stack.hpp"
#include "util/parser/forwards.hpp"
#include "util/parser/EvalVisitor.hpp"

#include <string>
#include <memory>

namespace core {
namespace fts {
namespace fstm {

/**
 * FstmEvalVisitor is an implementation of the EvalVisitor interface for
 * featured state machines (FSTM). It visits a Boolean expression to evaluate
 * if it satisfies a state of the FSTM.
 *
 * @author mwi
 */
class FstmEvalVisitor : public util::parser::EvalVisitor {

private:
    /* The name of the FSTM state against whom the evaluations are performed. */
    std::string stateName;
    /* The name of the FSTM event against whom the evaluations are performed. */
    std::string eventName;
    /* Stores the evaluations of the Boolean expressions. */
    std::auto_ptr<util::Stack<bool> > evals;

    /*
     * Representation Invariant:
     *   I(c) = c.stateName is non empty
     *     c.stateName starts with a lowercase letter
     *     c.eventName starts with a lowercase letter
     *     c.evals != null
     */

public:
    /**
     * @requires stateName.length > 0
     * @effects Makes this be a new FSTM eval visitor for the FSTM state
     *           named 'stateName' and the FSTM event named 'eventName' both
     *           with the first letter in lowercase.
     */
    FstmEvalVisitor(const std::string & stateName,
        const std::string & eventName);

    // The default destructor is fine.

    // overridden methods
    void reset();
    bool isTrue() const;
    void orExp(const util::parser::OrExp & exp);
    void andExp(const util::parser::AndExp & exp);
    void notExp(const util::parser::NotExp & exp);
    void boolValue(const util::parser::BoolValue & val);
    void var(const util::parser::Var & v);

private:
    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

} // namespace fstm
} // namespace fts
} // namespace core

#endif	/* CORE_FTS_FSTM_FSTMEVALVISITOR_HPP */
