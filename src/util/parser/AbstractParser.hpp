/*
 * AbstractParser.hpp
 *
 * Created on 2013-08-01
 */

#ifndef UTIL_PARSER_ABSTRACTPARSER_HPP
#define	UTIL_PARSER_ABSTRACTPARSER_HPP

#include "forwards.hpp"
#include "util/Uncopyable.hpp"

#include <string>

namespace util {
namespace parser {

/**
 * AbstractParser is an abstract parser that eases the implementation of a
 * parser for a LL(1) grammar.
 *
 * Whitespaces, tabulations, newlines and carriage returns are ignored.
 *
 * @author mwi
 */
class AbstractParser : private Uncopyable {

private:
    /* Whitespace */
    static const char Whitespace;
    /* Tabulation */
    static const char Tabulation;
    /* Newline */
    static const char Newline;
    /* Carriage return */
    static const char CarriageReturn;
    /* Beginning of a comment */
    static const std::string CommentBegin;
    /* End of a comment */
    static const std::string CommentEnd;

    /* The input to be parsed. */
    const std::string input;
    /* The input that has already been parsed. */
    std::string parsedInput;
    /* The next symbol, i.e., character, to parse. */
    std::string::const_iterator nextSymbol;
    /* The position of the next symbol to parse. */
    unsigned int position;

    /*
     * Representation Invariant:
     *   I(c) = 0 < c.position <= c.input.length + 1 &&
     *     c.position == c.parsedInput.length + 1
     */

public:
    // mandatory destructor
    virtual ~AbstractParser() {}

protected:
    /**
     * @effects Makes this be a new abstract parser for the input string
     *           'input'.
     */
    AbstractParser(const std::string & input);

    /**
     * @return the input, i.e., the string to be parsed.
     */
    const std::string & getInput() const;

    /**
     * @return the input that has already been parsed.
     */
    const std::string & getParsedInput() const;

    /**
     * @return true iff there are more symbols to yield.
     */
    bool hasNextSymbol() const;

    /**
     * @requires there are more symbols to yield
     * @return the next symbol, i.e., character, to parse.
     */
    char getNextSymbol() const;

    /**
     * @return the position of the next symbol to parse.
     */
    int getPosition() const;

    /**
     * @modifies this
     * @effects Appends 'str' to the input that has already been parsed.
     */
    void appendToParsedInput(const std::string & str);

    /**
     * @requires 'terminal' is a valid terminal of the grammar
     * @modifies this
     * @effects If the terminal 'terminal', possibly prefixed by
     *           whitespaces, matches the upcoming symbols, adds it to the
     *           parsed input and advances the cursor to the next symbol.
     *           Otherwise does nothing.
     * @return true iff the terminal 'terminal' matches the upcoming
     *          symbols.
     */
    bool expect(const std::string & terminal);

    /**
     * @modifies this
     * @effects Eats the upcoming whitespaces until a non-whitespace
     *           character is met.
     */
    void eatWhitespaces();

    /**
     * @modifies this
     * @effects Advances the cursor to the next symbol to parse.
     */
    void advanceCursor();

    /**
     * @return true iff the end of the input is reached.
     */
    bool isEndOfInputReach() const;

    /**
     * @return true iff the next symbol matches [a-z].
     */
    bool isLowercase() const;

    /**
     * @return true iff the next symbol matches [A-Z].
     */
    bool isUppercase() const;

    /**
     * @return true iff the next symbol matches [0-9].
     */
    bool isDigit() const;

    /**
     * @return true iff the next symbol matches '_'.
     */
    bool isUnderscore() const;

    /**
     * @modifies this
     * @effects Sets the error message upon a parsing error.
     */
    virtual void error() = 0;

    /**
     * @effects Asserts the rep invariant holds for this.
     */
    virtual void checkRep() const;

private:
    /**
     * @modifies this
     * @effects Parses the next symbol. If it matches 'symbol', advances
     *           the cursors, else does nothing.
     * @return true iff the next symbol matches 'symbol'.
     */
    bool accept(const char & symbol);
};

} // namespace parser
} // namespace util

#endif	/* UTIL_PARSER_ABSTRACTPARSER_HPP */
