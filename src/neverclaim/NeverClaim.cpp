/**
 * NeverClaim.cpp
 *
 * Created on 2013-08-04
 */

#include "NeverClaim.hpp"

#include "ClaimState.hpp"
#include "ClaimTransition.hpp"
#include "NeverClaimParser.hpp"
#include "ltl2ba.hpp"

#include "util/Assert.hpp"
#include "util/strings.hpp"

#include <glog/logging.h>

#include <fstream>

using std::auto_ptr;
using std::ifstream;
using std::string;
using std::tr1::shared_ptr;

using util::Generator;

namespace neverclaim {

const string NeverClaim::NeverClaimFilePath("/tmp/pvl_ltl2ba.tmp");

// constructors
NeverClaim::NeverClaim(const auto_ptr<Generator<ClaimTransitionPtr> > &
        transGen) : transitionTable(), initialState() {
    assertTrue(transGen.get());
    assertTrue(transGen->hasNext());

    ClaimTransitionPtr firstTransition = transGen->next();
    initialState = firstTransition->getSource();
    transitionTable.insert(*initialState, firstTransition);

    while (transGen->hasNext()) {
        ClaimTransitionPtr transition = transGen->next();
        transitionTable.insert(*transition->getSource(), transition);
    }

    checkRep();
}

// public methods
shared_ptr<ClaimState> NeverClaim::getInitialState() const {
    return initialState;
}

auto_ptr<Generator<ClaimTransitionPtr > > NeverClaim::executables(const
        ClaimState & state) const {
    return transitionTable.search(state);
}

// static factory methods
shared_ptr<NeverClaim> NeverClaim::create(const string & ltl) {
    ltl2ba(ltl, NeverClaimFilePath);
    ifstream inputFileStream(NeverClaimFilePath.c_str());
    assertTrue(inputFileStream.is_open());

    string strNeverClaim = util::toString(inputFileStream);

    DVLOG(2) << strNeverClaim;

    return NeverClaimParser(strNeverClaim).parse();
}

// private methods
void NeverClaim::checkRep() const {
    if (doCheckRep) {
        assertTrue(transitionTable.count() > 0);
        assertTrue(initialState);
    }
}

} // namespace neverclaim
