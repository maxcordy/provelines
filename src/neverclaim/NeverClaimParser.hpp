/*
 * NeverClaimParser.hpp
 *
 * Created on 2013-08-04
 */

#ifndef NEVERCLAIM_NEVERCLAIMPARSER_HPP
#define	NEVERCLAIM_NEVERCLAIMPARSER_HPP

#include "forwards.hpp"
#include "util/parser/forwards.hpp"
#include "util/parser/AbstractParser.hpp"
#include "util/List.hpp"

#include <string>

namespace neverclaim {

/**
 * NeverClaimParser is a simple recursive descent parser for never claim
 * automata written in promela.
 *
 * The EBNF LL(1) grammar for never claims is as follows:
 *
 *   Never: 'never' '{' Formula State+ '}'
 *
 *   Formula: '/ *' LTL '* /'
 *
 *   State: LABEL ':' Transitions
 *
 *   Transitions: 'if' Transition+ 'fi' ';' | 'skip'
 *
 *   Transition: '::' BoolExp '->' 'goto' LABEL
 *
 *   BoolExp: OrExp
 *
 *   OrExp: AndExp ('||' AndExp)*
 *
 *   AndExp: NotExp ('&&' NotExp)*
 *
 *   NotExp: '!' AtomExp | AtomExp
 *
 *   AtomExp: '(' BoolExp ')' | Proposition
 *
 *   Proposition: TRUE | FALSE | NAME
 *
 *   TRUE: '1' | 'true'
 *
 *   FALSE: '0' | 'false'
 *
 *   NAME: ('a'..'z') ('a'..'z'|'A'..'Z'|'0'..'9'|'_')*
 *
 *   LABEL: ('a'..'z'|'A'..'Z') ('a'..'z'|'A'..'Z'|'0'..'9'|'_')*
 *
 *   LTL: ('a'..'z'|'A'..'Z'|'0'..'9'|'_'|' '|'('|')'|BOOL_OP)+
 *
 *   BOOL_OP: '!' | '->' | '<->' | '&&' | '||'
 *
 * Whitespaces, tabs, newlines and carriage returns are ignored.
 *
 * @author mwi
 */
class NeverClaimParser : private util::parser::AbstractParser {

private:
    /* The error message (if any). */
    std::string errorMessage;
    /* The parsed claim transitions. */
    util::List<ClaimTransitionPtr> parsedTransitions;
    /* The LTL formula that has been parsed. */
    std::string ltlFormula;

    /*
     * Representation Invariant:
     *   I(c) = I(c.super)
     */

public:
    /**
     * @effects Makes this be a new parser for the never claim 'neverClaim'.
     */
    NeverClaimParser(const std::string & neverClaim);

    // The defaut destructor is fine.

    /**
     * @modifies this
     * @effects Parses the never claim of this.
     * @return the parsed never claim automaton.
     * @throws logic_error iff an error occurs during the parsing, i.e.,
     *          the never claim automaton was not valid.
     */
    std::tr1::shared_ptr<NeverClaim> parse();

private:
    /**
     * @modifies this
     * @effects Applies the Never production rule.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool never();

    /**
     * @modifies this
     * @effects Applies the Formula production rule.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool formula();

    /**
     * @modifies this
     * @effects Applies the State production rule.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool state();

    /**
     * @modifies this
     * @effects Applies the Transitions production rule with the source state
     *           'sourceState'.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool transitions(const std::string & sourceState);

    /**
     * @modifies this
     * @effects Applies the Transition production rule with the source state
     *           'sourceState'.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool transition(const std::string & sourceState);

    /**
     * @modifies this, exp
     * @effects Applies the BoolExp production rule and sets 'exp' to the
     *           parsed boolean expression.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool boolExp(std::tr1::shared_ptr<util::parser::BoolExp> & exp);

    /**
     * @modifies this, exp
     * @effects Applies the OrExp production rule and sets 'exp' to the
     *           parsed boolean expression.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool orExp(std::tr1::shared_ptr<util::parser::BoolExp> & exp);

    /**
     * @modifies this, exp
     * @effects Applies the AndExp production rule and sets 'exp' to the
     *           parsed boolean expression.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool andExp(std::tr1::shared_ptr<util::parser::BoolExp> & exp);

    /**
     * @modifies this, exp
     * @effects Applies the NotExp production rule and sets 'exp' to the
     *           parsed boolean expression.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool notExp(std::tr1::shared_ptr<util::parser::BoolExp> & exp);

    /**
     * @modifies this, exp
     * @effects Applies the AtomExp production rule and sets 'exp' to the
     *           parsed boolean expression.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool atomExp(std::tr1::shared_ptr<util::parser::BoolExp> & exp);

    /**
     * @modifies this, exp
     * @effects Applies the Proposition production rule and sets 'exp' to the
     *           parsed boolean proposition.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool proposition(std::tr1::shared_ptr<util::parser::BoolExp> & exp);

    /**
     * @modifies this, exp
     * @effects Applies the TRUE terminal rule and sets 'exp' to the parsed
     *           boolean expression.
     * @return true iff the terminal rule matches the upcoming symbols.
     */
    bool applyTrue(std::tr1::shared_ptr<util::parser::BoolExp> & exp);

    /**
     * @modifies this, exp
     * @effects Applies the FALSE terminal rule and sets 'exp' to the
     *           parsed boolean expression.
     * @return true iff the terminal rule matches the upcoming symbols.
     */
    bool applyFalse(std::tr1::shared_ptr<util::parser::BoolExp> & exp);

    /**
     * @modifies this, exp
     * @effects Applies the NAME terminal rule and sets 'exp' to the
     *           parsed boolean expression.
     * @return true iff the terminal rule matches the upcoming symbols.
     */
    bool name(std::tr1::shared_ptr<util::parser::BoolExp> & exp);

    /**
     * @modifies this, stateLabel
     * @effects Applies the LABEL terminal rule and sets 'stateLabel' to the
     *           parsed state label.
     * @return true iff the terminal rule matches the upcoming symbols.
     */
    bool label(std::string & stateLabel);

    /**
     * @modifies this
     * @effects Applies the LTL terminal rule.
     * @return true iff the TERMINAL rule matches the upcoming symbols.
     */
    bool ltl();

    // overridden methods
    void error();

    /**
     * @effects Asserts this satisfies the rep invariant.
     */
    void checkRep() const;

};

} // namespace neverclaim

#endif	/* NEVERCLAIM_NEVERCLAIMPARSER_HPP */
