/*
 * FstmEvent.cpp
 *
 * Created on 2013-07-26
 */

#include "FstmEvent.hpp"

#include "util/Assert.hpp"

using std::pair;
using std::string;
using std::tr1::shared_ptr;
using std::tr1::unordered_map;

namespace core {
namespace fts {
namespace fstm {

    unordered_map<string, shared_ptr<FstmEvent> > FstmEvent::nameTable;

    FstmEvent::FstmEvent(const string & name)
        : name(name) {
        checkRep();
    }

    string FstmEvent::getName() const {
        return name;
    }

    shared_ptr<FstmEvent> FstmEvent::makeEvent(const string & name) {
        assertTrue(name.length() > 0);

        if (nameTable.count(name) <= 0) {
            shared_ptr<FstmEvent> state(new FstmEvent(name));
            nameTable.insert(pair<string, shared_ptr<FstmEvent> >(name, state));
        }
        return nameTable[name];
    }

    void FstmEvent::checkRep() const {
        if (doCheckRep) {
            assertTrue(name.length() > 0);
            unordered_map<string, shared_ptr<FstmEvent> >::const_iterator it;
            for (it = nameTable.begin(); it != nameTable.end(); ++it) {
                assertTrue(it->first == it->second->getName());
            }
        }
    }

} // namespace fstm
} // namespace fts
} // namespace core
