/*
 * BoolFctCUDD.cpp
 *
 * Created on 2013-07-16
 */

#include "BoolFctCUDD.hpp"

#include "util/Assert.hpp"

#include <glog/logging.h>
#include <sstream>

using std::tr1::shared_ptr;
using std::string;
using std::stringstream;

using util::MemoryManager;

namespace math {

MemoryManager<BoolFctCUDD> BoolFctCUDD::memoryManager;

Cudd BoolFctCUDD::manager;

BDD BoolFctCUDD::voidBdd;

// constructors
BoolFctCUDD::BoolFctCUDD(BDD bdd)
    : valid(true), bdd(bdd) {
    checkRep();
}

BoolFctCUDD::BoolFctCUDD(const BoolVar & var)
    : valid(true), bdd(manager.bddVar(var.getId())) {
    checkRep();
}

// public methods
BoolFctCUDD * BoolFctCUDD::getTrue() {
    return new BoolFctCUDD(manager.bddOne());
}

BoolFctCUDD * BoolFctCUDD::getFalse() {
    return new BoolFctCUDD(manager.bddZero());
}

BoolFct & BoolFctCUDD::conjunction(BoolFct & formula) {
    assertTrue(this != &formula);
    BoolFctCUDD& rhs = static_cast<BoolFctCUDD&>(formula);
    assertTrue(valid && rhs.valid);

    bdd *= rhs.bdd;
    invalidate(rhs);

    checkRep();
    rhs.checkRep();

    return *this;
}

BoolFct & BoolFctCUDD::disjunction(BoolFct & formula) {
    assertTrue(this != &formula);
    BoolFctCUDD& rhs = static_cast<BoolFctCUDD&>(formula);
    assertTrue(valid && rhs.valid);

    bdd += rhs.bdd;
    invalidate(rhs);

    checkRep();
    rhs.checkRep();

    return *this;
}

BoolFct & BoolFctCUDD::negation() {
    assertTrue(valid);

    bdd = !bdd;

    checkRep();

    return *this;
}

bool BoolFctCUDD::isSatisfiable() const {
    assertTrue(valid);

    return bdd != manager.bddZero();
}

bool BoolFctCUDD::isTautology() const {
    assertTrue(valid);

    return bdd == manager.bddOne();
}

bool BoolFctCUDD::isEquivalent(const BoolFct & formula) const {
    const BoolFctCUDD& rhs = static_cast<const BoolFctCUDD&>(formula);
    assertTrue(valid && rhs.valid);

    return bdd == rhs.bdd;
}

bool BoolFctCUDD::implies(const BoolFct & formula) const {
    const BoolFctCUDD& rhs = static_cast<const BoolFctCUDD&>(formula);
    assertTrue(valid && rhs.valid);

    // return bdd <= rhs.bdd; see issue on jira

    if (bdd == rhs.bdd) {
        // should be more efficient because it compares pointers
        return true;
    } else {

        return (!bdd + rhs.bdd) == manager.bddOne();
    }
}

bool BoolFctCUDD::isValid() const {
    return valid;
}

BoolFct * BoolFctCUDD::clone() const {
    return new BoolFctCUDD(this->bdd);
}

string BoolFctCUDD::toString() const {
    if (bdd == manager.bddOne()) {
        return "true";
    }
    if (bdd == manager.bddZero()) {
        return "false";
    }
    return buildString();
}

// private methods
void BoolFctCUDD::invalidate(BoolFctCUDD & fct) {
    /* From cudd documentation :
     * "BDDs are freed when execution leaves the scope in which they are
     * defined or when the variables referring to them are overwritten."
     * The following statement is thus used to free the underlying BDD.
     */
    fct.bdd = voidBdd;
    fct.valid = false;
}

string BoolFctCUDD::buildString() const {
    FILE * file = tmpfile();
    manager.SetStdout(file);
    bdd.PrintMinterm();
    rewind(file);
    stringstream stream;
    stream << "(";
    bool nextLiteral = false;
    bool nextMinterm = false;
    int i = 0;
    int c = !EOF;
    while (c != EOF) {
        c = fgetc(file);
        switch(c) {
            case '-':
                ++i;
                break;
            case '1':
                if (nextLiteral) {
                    stream << " && ";
                }
                if (nextMinterm) {
                    stream << ") || (";
                }
                stream << BoolVar::makeBoolVar(i).getName();
                nextLiteral = true;
                nextMinterm = false;
                ++i;
                break;
            case '0':
                if (nextLiteral) {
                    stream << " && ";
                }
                if (nextMinterm) {
                    stream << ") || (";
                }
                stream << "!(" << BoolVar::makeBoolVar(i).getName() << ")";
                nextLiteral = true;
                nextMinterm = false;
                ++i;
                break;
            case ' ':
                char buffer[8];
                fgets(buffer, 8, file);
                nextLiteral = false;
                nextMinterm = true;
                i = 0;
                break;
        }
    }
    fclose(file);
    stream << ")";
    return stream.str();
}

void BoolFctCUDD::checkRep() const {
    /* FIXME: The use of a comparison operator on a void bdd causes a
     * segmentation fault
     */
    if (doCheckRep) {
        // assertTrue((bdd != voidBdd) == valid);
    }
}

} // namespace math
