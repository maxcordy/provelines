/*
 * FstmState.cpp
 *
 * Created on 2013-07-26
 */

#include "FstmState.hpp"

#include "util/Assert.hpp"

#include <utility>
#include <tr1/functional>

using std::string;
using std::make_pair;
using std::tr1::hash;
using std::tr1::shared_ptr;
using std::tr1::unordered_map;

namespace core {
namespace fts {
namespace fstm {

unordered_map<string, FstmStatePtr> FstmState::nameTable;

// constructors
FstmState::FstmState(const string & name)
    : name(name) {
    checkRep();
}

// public methods
string FstmState::getName() const {
    return name;
}

bool FstmState::equals(const State & state) const {
    return this == &state;
}

unsigned int FstmState::hashCode() const {
    hash<string> hashFct;
    unsigned int hashValue = 7;
    hashValue = 17 * hashValue + hashFct(name);
    return hashValue;
}

// static factory methods
FstmStatePtr FstmState::makeState(const string & name) {
    assertTrue(name.length() > 0);

    if (nameTable.count(name) <= 0) {
        shared_ptr<FstmState> state(new FstmState(name));
        nameTable.insert(make_pair(name, state));
    }
    return nameTable[name];
}

// private methods
void FstmState::checkRep() const {
    if (doCheckRep) {
        assertTrue(name.length() > 0);
        unordered_map<string, shared_ptr<FstmState> >::const_iterator it;
        for (it = nameTable.begin(); it != nameTable.end(); ++it) {
            assertTrue(it->first == it->second->getName());
        }
    }
}

} // namespace fstm
} // namespace fts
} // namespace core
