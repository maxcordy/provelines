/*
 * TransitionTable.cpp
 *
 * Created on 2013-08-14
 */

#include "TransitionTable.hpp"

#include "FstmState.hpp"
#include "FstmTransition.hpp"
#include "util/Assert.hpp"
#include "util/HashTable.hpp"

#include <memory>
#include <stdexcept>

using util::Generator;
using std::auto_ptr;
using std::make_pair;
using std::vector;

namespace core {
namespace fts {
namespace fstm {

// constructors
TransitionTable::TransitionTable(FstmTransitionGeneratorPtr gen,
        unsigned int size) : transitionTable(), indexTable() {
    assertTrue(gen);
    assertTrue(gen->hasNext());
    assertTrue(size > 0);

    // reserve is the right method to call, but not resize !!!
    // here lays a fucking bug that pissed me off
    transitionTable.reserve(size);

    initTables(gen);

    checkRep();
}

// public methods
FstmStatePtr TransitionTable::getInitialState() const {
    return transitionTable.at(0)->getSource();
}

TransitionGeneratorPtr TransitionTable::executables(const FstmState & state)
        const {
    int firstIndex = firstIndexOf(state);
    int lastIndex = lastIndexOf(state);

    if (firstIndex < 0) {
        assertTrue(lastIndex < 0);
        return TransitionGeneratorPtr(new EmptyTransitionGenerator());
    }

    return TransitionGeneratorPtr(new TransitionGenerator(transitionTable,
            firstIndex, lastIndex));
}

// private methods
void TransitionTable::initTables(FstmTransitionGeneratorPtr & gen) {
    unsigned int firstIndex = 0;
    FstmTransitionPtr transition = gen->next();
    FstmStatePtr curSource = transition->getSource();

    transitionTable.push_back(transition);

    while (gen->hasNext()) {
        transition = gen->next();
        if (!curSource->equals(*transition->getSource())) {
            const unsigned int lastIndex = transitionTable.size() - 1;
            indexTable.insert(*curSource, make_pair(firstIndex, lastIndex));
            firstIndex = lastIndex + 1;
            curSource = transition->getSource();
        }
        transitionTable.push_back(transition);
    }

    const unsigned int lastIndex = transitionTable.size() - 1;
    indexTable.insert(*curSource, make_pair(firstIndex, lastIndex));
}

int TransitionTable::firstIndexOf(const FstmState & state) const {
    auto_ptr<Generator<IndexPair> > g = indexTable.search(state);
    return g->hasNext() ? g->next().first : -1;
}

int TransitionTable::lastIndexOf(const FstmState & state) const {
    auto_ptr<Generator<IndexPair> > g = indexTable.search(state);
    return g->hasNext() ? g->next().second : -1;
}

void TransitionTable::checkRep() const {
    if (doCheckRep) {
        assertTrue(transitionTable.size() > 0);
        for (unsigned int i = 0; i < transitionTable.size(); ++i) {
            assertTrue(transitionTable.at(i));
        }
        assertTrue(indexTable.count() > 0);
        vector<FstmTransitionPtr>::const_iterator it;
        FstmStatePtr lastSource;
        int nbOfDistinctSources = 0;
        for (it = transitionTable.begin(); it != transitionTable.end(); ++it) {
            if (!lastSource || !lastSource->equals(*(*it)->getSource())) {
                ++nbOfDistinctSources;
                lastSource = (*it)->getSource();
                auto_ptr<Generator<IndexPair> > g =
                        indexTable.search(*lastSource);
                assertTrue(g->hasNext());
                IndexPair pair = g->next();
                assertTrue(pair.first <= pair.second);
                assertTrue(!g->hasNext());
            }
        }
        assertTrue(nbOfDistinctSources == indexTable.count());
    }
}

// transition generator
TransitionTable::TransitionGenerator::TransitionGenerator(
        const vector<FstmTransitionPtr> & transitionTable,
        unsigned int firstIndex, unsigned int lastIndex)
        : transitionTable(transitionTable), firstIndex(firstIndex),
          lastIndex(lastIndex), curIndex(firstIndex) {
    checkRep();
}

bool TransitionTable::TransitionGenerator::hasNext() const {
    return curIndex <= lastIndex;
}

TransitionPtr TransitionTable::TransitionGenerator::next() {
    assertTrue(hasNext());

    FstmTransitionPtr t = transitionTable.at(curIndex);
    ++curIndex;

    checkRep();
    return t;
}

void TransitionTable::TransitionGenerator::checkRep() const {
    if (doCheckRep) {
        assertTrue(firstIndex <= lastIndex);
        assertTrue(firstIndex <= curIndex);
        assertTrue(curIndex <= lastIndex + 1);
        assertTrue(lastIndex < transitionTable.size());
    }
}

// empty transition generator
bool TransitionTable::EmptyTransitionGenerator::hasNext() const {
    return false;
}

TransitionPtr TransitionTable::EmptyTransitionGenerator::next() {
    // should never get here
    throw std::logic_error("TransitionTable::EmptyTransitionGenerator::next");
}

} // namespace fstm
} // namespace fts
} // namespace core
