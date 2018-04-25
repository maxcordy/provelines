/*
 * BoolFctCUDD.hpp
 *
 * Created on 2013-07-16
 */

#ifndef MATH_BOOLFCTCUDD_HPP
#define MATH_BOOLFCTCUDD_HPP

#include "BoolFct.hpp"
#include "BoolVar.hpp"

#include "util/MemoryManager.hpp"

#include <cuddObj.hh>

#include <string>

namespace math {

/**
 * BoolFctCUDD is an implementation of the BoolFct interface using binary
 * decision diagrams (BDDs) with the CUDD library.
 *
 * @author bdawagne
 */
class BoolFctCUDD : public BoolFct {

private:
    static util::MemoryManager<BoolFctCUDD> memoryManager;

    static Cudd manager;
    static BDD voidBdd;

    bool valid;
    BDD bdd;

    /*
     * Representation Invariant:
     *   I(c) = (c.valid && c.bdd = c.voidBdd) ||
     *            (!c.valid && c.bdd != c.voidBdd)
     */

    /**
     * @effects Makes this be a new boolean function 'f' such that
     *           f('var') = 'var'.
     */
    explicit BoolFctCUDD(const BoolVar & var);

    /**
     * @effects Makes this be a new boolean function with the BDD 'bdd'.
     */
    explicit BoolFctCUDD(BDD bdd);

    /**
     * @return a tautology
     */
    static BoolFctCUDD * getTrue();

    /**
     * @return an unsatisfiable boolean function
     */
    static BoolFctCUDD * getFalse();

    friend class BoolFctCUDDFactory;

public:
    // The default copy constructor is fine.

    // The default assignement operator is fine.

    // The default destructor is fine.

    // overridden methods
    BoolFct & conjunction(BoolFct & formula);
    BoolFct & disjunction(BoolFct & formula);
    BoolFct & negation();
    bool isSatisfiable() const;
    bool isTautology() const;
    bool isEquivalent(const BoolFct & formula) const;
    bool implies(const BoolFct & formula) const;
    bool isValid() const;
    BoolFct * clone() const;
    std::string toString() const;

    void * operator new(size_t size) {
        return memoryManager.allocate(size);
    }

    void operator delete(void * pointerToDelete) {
        memoryManager.free(pointerToDelete);
    }

private:
    /**
     * @modifies this
     * @effects Invalidates f.
     */
    void invalidate(BoolFctCUDD & f);

    /**
     * @return the built string representation of this.
     */
    std::string buildString() const;

    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

} // namespace math

#endif /* MATH_BOOLFCTCUDD_HPP */
