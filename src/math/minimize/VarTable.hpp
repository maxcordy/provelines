/*
 * VarTable.hpp
 *
 * Created on 2013-08-29
 */

#ifndef MATH_MINIMIZE_VARTABLE_HPP
#define	MATH_MINIMIZE_VARTABLE_HPP

#include "forwards.hpp"
#include "util/Uncopyable.hpp"

#include <string>
#include <tr1/unordered_map>

namespace math {
namespace minimize {

/**
 * VarTable is a mutable bijective table between variable names (as strings)
 * and single characters (as chars). The capacity of a VarTable is bounded by
 * the number of distrinct alphabetic characters.
 *
 * @author mwi
 */
class VarTable : private util::Uncopyable {
private:
    std::tr1::unordered_map<std::string, char> stringTable;
    std::tr1::unordered_map<char, std::string> charTable;
    char nextChar;

    /**
     * Representation Invariant:
     *   I(c) = c.stringTable.size = c.charTable.size
     *     for all mappings <k,v> in c.stringTable .(<v,k> is in c.charTable)
     *     c.nextChar in {[a-zA-Z] | '\0'}
     */

public:
    /**
     * @effects Makes this be an empty var table.
     */
    VarTable();

    // The default destructor is fine.

    /**
     * @requires this not full && varName not in this
     * @modifies this
     * @effects Adds a new mapping for the variable name 'varName' in this with
     *           a character that is not used yet.
     * @return the character that is mapped to 'varName'.
     */
    char add(const std::string & varName);

    /**
     * @return true iff this is full.
     */
    bool isFull() const;

    /**
     * @return true iff varName is in this.
     */
    bool isIn(const std::string & varName) const;

    /**
     * @return true iff varName is in this.
     */
    bool isIn(const char & varName) const;

    /**
     * @requires varName in this
     * @return the char representation of the variable 'varName' in this.
     */
    char get(const std::string & varName) const;

    /**
     * @requires varName in this
     * @return the string representation of the variable 'varName' in this.
     */
    std::string get(const char & varName) const;

private:
    /**
     * @requires the next char is an alphabetic character
     * @modifies this
     * @effects Sets the next char to the next alaphabetic character or '\0' if
     *           there is no more alaphabetic character to generate.
     */
    void setNextChar();

    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

} // minimize
} // math

#endif	/* MATH_MINIMIZE_VARTABLE_HPP */
