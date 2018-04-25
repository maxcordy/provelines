/*
 * VarTable.cpp
 *
 * Created on 2013-08-29
 */

#include "VarTable.hpp"

#include "util/Assert.hpp"
#include "util/Generator.hpp"

#include <memory>

using std::auto_ptr;
using std::make_pair;
using std::string;
using std::tr1::unordered_map;
using util::Generator;

namespace math {
namespace minimize {

// constructors
VarTable::VarTable() : stringTable(), charTable(), nextChar('a') {
    checkRep();
}

// public methods
char VarTable::add(const string & varName) {
    assertTrue(!isFull());
    assertTrue(!isIn(varName));

    const char charRep = nextChar;
    stringTable.insert(make_pair(varName, charRep));
    charTable.insert(make_pair(charRep, varName));

    setNextChar();

    checkRep();
    return charRep;
}

bool VarTable::isFull() const {
    return nextChar == '\0';
}

bool VarTable::isIn(const string & varName) const {
    return stringTable.count(varName) > 0;
}

bool VarTable::isIn(const char & varName) const {
    return charTable.count(varName) > 0;
}

char VarTable::get(const string & varName) const {
    unordered_map<string, char>::const_iterator it;
    it = stringTable.find(varName);
    assertTrue(it != stringTable.end());
    return it->second;
}

string VarTable::get(const char & varName) const {
    unordered_map<char, string>::const_iterator it;
    it = charTable.find(varName);
    assertTrue(it != charTable.end());
    return it->second;
}

// private methods
void VarTable::setNextChar() {
    assertTrue(nextChar != '\0');

    if (nextChar == 'z') {
        nextChar = 'A';
    } else if(nextChar == 'Z') {
        nextChar = '\0';
    } else {
        ++nextChar;
    }
}

void VarTable::checkRep() const {
    if (doCheckRep) {
        assertTrue(stringTable.size() == charTable.size());
        unordered_map<string, char>::const_iterator it;
        for (it = stringTable.begin(); it != stringTable.end(); ++it) {
            assertTrue(charTable.count(it->second) == 1);
            unordered_map<char, string>::const_iterator elt;
            elt = charTable.find(it->second);
            assertTrue(elt != charTable.end());
            assertTrue(elt->second == it->first);
        }
        assertTrue((nextChar >= 'a' && nextChar <= 'z')
                || (nextChar >= 'A' && nextChar <= 'Z')
                || (nextChar == '\0'));
    }
}

} // minimize
} // math
