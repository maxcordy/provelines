/*
 * AbstractParser.cpp
 *
 * Created on 2013-08-01
 */

#include "AbstractParser.hpp"

#include "util/Assert.hpp"

using std::string;

namespace util {
namespace parser {

const char AbstractParser::Whitespace = ' ';
const char AbstractParser::Tabulation = '\t';
const char AbstractParser::Newline = '\n';
const char AbstractParser::CarriageReturn = '\r';

const string AbstractParser::CommentBegin = "/*";
const string AbstractParser::CommentEnd = "*/";

AbstractParser::AbstractParser(const std::string & input)
    : input(input), parsedInput(), nextSymbol(), position(1) {

    nextSymbol = input.begin();

    checkRep();
}

const string & AbstractParser::getInput() const {
    return input;
}

const string & AbstractParser::getParsedInput() const {
    return parsedInput;
}

bool AbstractParser::hasNextSymbol() const {
    return nextSymbol != input.end();
}

char AbstractParser::getNextSymbol() const {
    return *nextSymbol;
}

int AbstractParser::getPosition() const {
    return position;
}

void AbstractParser::appendToParsedInput(const string & str) {
    parsedInput += str;
}

bool AbstractParser::expect(const string & terminal) {
    eatWhitespaces();

    string::const_iterator savedNextSymbol(nextSymbol);
    unsigned int savedPosition = position;

    string::const_iterator it;
    for (it = terminal.begin(); it != terminal.end(); ++it) {
        if (!accept(*it)) {
            error();
            nextSymbol = savedNextSymbol;
            position = savedPosition;
            return false;
        }
    }
    parsedInput += terminal;
    return true;
}

void AbstractParser::eatWhitespaces() {
    bool moreWhitespaces = true;

    while (moreWhitespaces) {
        switch (*nextSymbol) {
            case Whitespace:
                parsedInput += Whitespace;
                break;
            case Newline:
                parsedInput += Newline;
                break;
            case Tabulation:
                parsedInput += Tabulation;
                break;
            case CarriageReturn:
                parsedInput += CarriageReturn;
                break;
            default:
                moreWhitespaces = false;
                break;
        }
        if (moreWhitespaces) {
            advanceCursor();
        }
    }
}

void AbstractParser::advanceCursor() {
        ++nextSymbol;
        ++position;
}

bool AbstractParser::isEndOfInputReach() const {
    return getInput().length() == getParsedInput().length();
}

bool AbstractParser::isLowercase() const {
    return getNextSymbol() >= 'a' && getNextSymbol() <= 'z';
}

bool AbstractParser::isUppercase() const {
    return getNextSymbol() >= 'A' && getNextSymbol() <= 'Z';
}

bool AbstractParser::isDigit() const {
    return getNextSymbol() >= '0' && getNextSymbol() <= '9';
}

bool AbstractParser::isUnderscore() const {
    return getNextSymbol() == '_';
}

void AbstractParser::checkRep() const {
    assertTrue(0 < position);
    assertTrue(position <= input.length() + 1);
    assertTrue(position == parsedInput.length() + 1);
}

// private methods
bool AbstractParser::accept(const char & symbol) {
    if (*nextSymbol == symbol) {
        advanceCursor();
        return true;
    }
    return false;
}

} // namespace parser
} // namespace util
