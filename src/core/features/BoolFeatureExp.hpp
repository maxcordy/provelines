/*
 * BoolFeatureExp.hpp
 *
 * Created on 2013-07-22
 */

#ifndef CORE_FEATURES_BOOLFEATUREEXP_HPP
#define	CORE_FEATURES_BOOLFEATUREEXP_HPP

#include "FeatureExp.hpp"

#include "math/forwards.hpp"

#include "util/MemoryManager.hpp"

#include <memory>
#include <string>

namespace core {
namespace features {

/**
 * BoolFeatureExp is a mutable boolean feature expression, i.e., a feature
 * expression represented by a boolean function.
 *
 * @author bdawagne
 * @author mwi
 */
class BoolFeatureExp : public FeatureExp {

private:
    static util::MemoryManager<BoolFeatureExp> memoryManager;

    std::auto_ptr<math::BoolFct> function;

    /*
     * Representation Invariant:
     *   I(c) = c.function != null
     */

    // private constructor
    BoolFeatureExp(math::BoolFct * function);

    friend class BoolFeatureExpFactory;

public:
    // The default destructor is fine.

    // overridden methods
    FeatureExp * conjunction(FeatureExp & expression);
    FeatureExp * disjunction(FeatureExp & expression);
    FeatureExp * negation();
    bool isSatisfiable() const;
    bool isTautology() const;
    bool isEquivalent(const FeatureExp & expression) const;
    bool implies(const FeatureExp & expression) const;
    bool isValid() const;
    std::string toString() const;
    FeatureExp * clone() const;

    void * operator new(size_t size) {
        return memoryManager.allocate(size);
    }

    void operator delete(void * pointerToDelete) {
        memoryManager.free(pointerToDelete);
    }

private:
    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

} // namespace features
} // namespace core

#endif	/* CORE_FEATURES_BOOLFEATUREEXP_HPP */
