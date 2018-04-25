/*
 * BoolVar.cpp
 *
 * Created on 2013-07-16
 */

#include "BoolVar.hpp"

#include "util/Assert.hpp"

#include <sstream>

using std::ostringstream;
using std::pair;
using std::string;
using std::tr1::unordered_map;

namespace math {

unordered_map<string, int> BoolVar::nameTable;
unordered_map<int, string> BoolVar::idTable;
int BoolVar::maxId = 1;

BoolVar::BoolVar(const string & name, int id)
    : name(name), id(id) {
    checkRep();
}

string BoolVar::getName() const {
    return name;
}

int BoolVar::getId() const {
    return id;
}

BoolVar BoolVar::makeBoolVar(const string & name) {
    assertTrue(name.length() > 0);

    int id;
    if(nameTable.count(name) == 0) {
        ++maxId;
        id = maxId;
        save(name, id);
    } else {
        id = nameTable[name];
    }

    return BoolVar(name, id);
}

BoolVar BoolVar::makeBoolVar(int id) {
    assertTrue(id > 0);

    string name;
    if(idTable.count(id) == 0) {
        name = generateName(id);
        save(name, id);
    } else {
        name = idTable[id];
    }

    if (id > maxId) {
        maxId = id;
    }

    return BoolVar(name, id);
}

BoolVar BoolVar::createBoolVar(const string & name, int id)  {
    assertTrue(name.length() > 0);
    assertTrue(id > 0);
    assertTrue(nameTable.count(name) == 0);
    assertTrue(idTable.count(id) == 0);

    save(name, id);

    if (id > maxId) {
        maxId = id;
    }

    return BoolVar(name, id);
}

void BoolVar::checkRep() const {
    if (doCheckRep) {
        assertTrue(nameTable.count(name) == 1);
        assertTrue(nameTable[name] == id);
        assertTrue(idTable.count(id) == 1);
        assertTrue(idTable[id] == name);
        unordered_map<string, int>::const_iterator it;
        for (it = nameTable.begin(); it != nameTable.end(); ++it) {
            assertTrue(it->first.length() > 0);
            assertTrue(it->second > 0);
            assertTrue(it->second <= maxId);
            assertTrue(idTable.count(it->second) == 1);
        }
    }
}

string BoolVar::generateName(int id) {
    ostringstream oss;
    oss << "_" << id;
    string result = oss.str();
    while (nameTable.count(result) > 0) {
        oss << "_";
        result = oss.str();
    }
    return result;
}

void BoolVar::save(const string & name, int id) {
    nameTable.insert(pair<string, int>(name, id));
    idTable.insert(pair<int, string>(id, name));
}

} // namespace math
