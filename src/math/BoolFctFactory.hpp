/*
 * BoolFctFactory.hpp
 *
 * Created on 2013-07-26
 */

#ifndef MATH_BOOLFCTFACTORY_HPP
#define MATH_BOOLFCTFACTORY_HPP

#include "forwards.hpp"

namespace math {

/**
 * BoolFctFactory is an abstract factory for creating boolean functions.
 *
 * The pointers returned by this factory must be managed by the clients.
 *
 * @author mwi
 */
class BoolFctFactory {

public:
    /**
     * @return a new boolean function for the boolean variable 'var'.
     */
    virtual BoolFct * create(const BoolVar & var) const = 0;

    /**
     * @requires varName.length > 0
     * @return a new boolean function for a boolean variable named 'varName'.
     */
    virtual BoolFct * create(const std::string & varName) const = 0;

    /**
     * @return a tautology
     */
    virtual BoolFct * getTrue() const = 0;

    /**
     * @return an unsatisfiable boolean function
     */
    virtual BoolFct * getFalse() const = 0;

    // mandatory virtual constructor
    virtual ~BoolFctFactory() {}
};

} // namespace math

#endif /* MATH_BOOLFCTFACTORY_HPP */
