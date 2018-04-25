/*
 * BoolExpParser.hpp
 *
 * Created on 2013-07-23
 */

#ifndef UTIL_PARSER_BOOLEXPPARSER_HPP
#define	UTIL_PARSER_BOOLEXPPARSER_HPP

#include "forwards.hpp"
#include "AbstractParser.hpp"

#include <string>

namespace util {
namespace parser {

/**
 * BoolExpParser is a simple recursive descent parser for Boolean expressions.
 *
 * The BNF grammar for Boolean expressions is as follows:
 *
 *   BoolExp: BoolExp '||' BoolExp
 *          | BoolExp '&&' BoolExp
 *          | '!' '(' BoolExp ')'
 *          | '(' BoolExp ')'
 *          | Value
 *          | VARIABLE
 *          ;
 *
 *   Value: TRUE | FALSE
 *
 *   TRUE: '1' | 'true'
 *
 *   FALSE: '0' | 'false'
 *
 *   VARIABLE: An identifier, i.e., ^[A-Za-z_]![A-Za-z0-9_]*
 *
 * Whitespaces, tabs, newlines and carriage returns are ignored.
 *
 * An empty Boolean expression denotes a Boolean expression that is always
 * true (purely conventional).
 *
 * @author mwi
 */
class BoolExpParser : private util::parser::AbstractParser {

private:
    // terminals
    static const std::string TerminalAnd;
    static const std::string TerminalOr;
    static const std::string TerminalNot;
    static const std::string TerminalLeftParen;
    static const std::string TerminalRightParen;

    /* The error message (if any). */
    std::string errorMessage;

    /*
     * Representation Invariant:
     *   I(c) = I(c.super)
     */

public:
    /**
     * @effects Makes this be a new parser for the Boolean expression
     *           'boolExp'.
     */
    BoolExpParser(const std::string & boolExp);

    // The defaut destructor is fine.

    /**
     * @modifies this
     * @effects Parses the Boolean expression of this.
     * @return the parsed Boolean expression (as BoolExp).
     * @throws logic_error iff an error occurs during the parsing, i.e.,
     *          the Boolean expression was not valid.
     */
    BoolExpPtr parse();

private:
    /**
     * @modifies this, exp
     * @effects Applies <boolExp> ::= <logicalOr> and sets 'exp' to
     *           the parsed Boolean expression.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool boolExp(BoolExpPtr & exp);

    /**
     * @modifies this, exp
     * @effects Applies <logicalOr> ::= <logicalAnd>
     *                                  | ('||' <logicalAnd>)*
     *           and sets 'exp' to the parsed Boolean expression.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool logicalOr(BoolExpPtr & exp);

    /**
     * @modifies this, exp
     * @effects Applies <logicalAnd> ::= <prefixOp> ('&&' <prefixOp>)*
     *           and sets 'exp' to the parsed Boolean expression.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool logicalAnd(BoolExpPtr & exp);

    /**
     * @modifies this, exp
     * @effects Applies <prefixOp> ::= '!' '(' <boolExp> ')'
     *                                 | <atomic>
     *           and sets 'exp' to the parsed Boolean expression.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool prefixOp(BoolExpPtr & exp);

    /**
     * @modifies this, exp
     * @effects Applies <atomic> ::= '(' <boolExp> ')'
     *                               | <value>
     *                               | <variable>
     *           and sets 'exp' to the parsed Boolean expression.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool atomic(BoolExpPtr & exp);

    /**
     * @modifies this, val
     * @effects Applies <value> ::= TRUE | FALSE
     *           and sets 'val' to the parsed Boolean value.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool value(BoolExpPtr & val);

    /**
     * @modifies this, var
     * @effects Parses a variable name corresponding to VARIABLE and sets
     *           'var' to the parsed Boolean variable.
     * @return true iff a valid variable name is parsed.
     */
    bool variable(BoolExpPtr & var);

    // overridden methods
    void error();

    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

} // namespace parser
} // namespace util

#endif	/* UTIL_PARSER_BOOLEXPPARSER_HPP */
