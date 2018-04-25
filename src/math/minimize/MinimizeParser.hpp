/*
 * MinimizeParser.hpp
 *
 * Created on 2013-08-29
 */

#ifndef MATH_MINIMIZE_MINIMIZEPARSER_HPP
#define	MATH_MINIMIZE_MINIMIZEPARSER_HPP

#include "forwards.hpp"
#include "util/parser/AbstractParser.hpp"

#include <string>

namespace math {
namespace minimize {

/**
 * MinimizeParser is a simple recursive descent parser for Minimize expression.
 *
 * A Minimize expression is a Boolean expression as used by the Minimize
 * program.
 *
 * The EBNF LL(1) grammar for Minimize expressions is as follows:
 *
 *   BoolExp: OrExp
 *
 *   OrExp: AndExp ('+' AndExp)*
 *
 *   AndExp: NotExp (('*')? NotExp)*
 *
 *   NotExp: AtomExp (''')?
 *
 *   AtomExp: '(' BoolExp ')' | VALUE | VARIABLE
 *
 *   VALUE: '1' | '0'
 *
 *   VARIABLE: a single character
 *
 * Whitespaces, tabs, newlines and carriage returns are ignored.
 *
 * @author mwi
 */
class MinimizeParser : public util::parser::AbstractParser {

private:
    const VarTablePtr varTable;
    /* The error message (if any). */
    std::string errorMessage;

    /*
     * Representation Invariant:
     *   I(c) = I(c.super)
     *     c.varTable != null
     */

public:
    /**
     * @requires varTable != null
     * @effects Makes this be a new Minimize parser for the Minimize expression
     *           'minimize' and with the variable table 'varTable'.
     */
    MinimizeParser(const std::string & minimize, const VarTablePtr & varTable);

    // The default destructor is fine.

    /**
     * @modifies this
     * @effects Parses the Minimize expression of this.
     * @return the parsed Boolean expression (as String).
     * @throws logic_error iff an error occurs during the parsing, i.e.,
     *          the Minimize expression was not valid
     */
    std::string parse();

private:
    /**
     * @modifies this, exp
     * @effects Applies the BoolExp production rule and sets 'exp' to
     *           the parsed Boolean expression.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool boolExp(std::string & exp);

    /**
     * @modifies this, exp
     * @effects Applies the OrExp production rule and sets 'exp' to the parsed
     *           Boolean expression.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool orExp(std::string & exp);

    /**
     * @modifies this, exp
     * @effects Applies the AndExp production rule and sets 'exp' to the parsed
     *           Boolean expression.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool andExp(std::string & exp);

    /**
     * @modifies this, exp
     * @effects Applies the NotExp production rule and sets 'exp' to the parsed
     *           Boolean expression.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool notExp(std::string & exp);

    /**
     * @modifies this, exp
     * @effects Applies the AtomExp production rule and sets 'exp' to the
     *           parsed Boolean expression.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool atomExp(std::string & exp);

    /**
     * @modifies this, val
     * @effects Applies the VALUE terminal and sets 'val' to the parsed
     *           Boolean value.
     * @return true iff the production rule matches the upcoming symbols.
     */
    bool value(std::string & val);

    /**
     * @modifies this, var
     * @effects Applies the VARIABLE terminal and sets 'var' to the parsed
     *           variable.
     * @return true iff a valid variable name is parsed.
     */
    bool variable(std::string & var);

    /**
     * @return true iff the next symbol matches the VARIABLE terminal.
     */
    bool isVariable() const;

    /**
     * @modifies this
     * @effects Sets the error message to an error message indicating the
     *           variable named 'var' does not exist in the variable table.
     */
    void errorVarNotFound(const char & var);

    // overridden methods
    void error();

    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

} // minimize
} // math

#endif	/* MATH_MINIMIZE_MINIMIZEPARSER_HPP */
