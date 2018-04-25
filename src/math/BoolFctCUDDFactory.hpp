/*
 * BoolFctCUDDFactory.hpp
 *
 * Created on 2013-07-26
 */

#ifndef MATH_BOOLFCTCUDDFACTORY_HPP
#define MATH_BOOLFCTCUDDFACTORY_HPP

#include "BoolFctFactory.hpp"

#include "util/Uncopyable.hpp"

namespace math  {

/**
 * BoolFctCUDDFactory in an implementation of BoolFctFactory that creates
 * boolean functions as BoolFctCUDDs.
 *
 * @author mwi
 */
class BoolFctCUDDFactory : public BoolFctFactory, private util::Uncopyable {

public:
    /**
     * @effects Makes this be a new BoolFctCUDDFactory.
     */
    BoolFctCUDDFactory();

    // The default destructor is fine.

    // overridden methods
    BoolFct * create(const BoolVar & var) const;
    BoolFct * create(const std::string & varName) const;
    BoolFct * getTrue() const;
    BoolFct * getFalse() const;
};

} // namespace math

#endif /* MATH_BOOLFCTCUDDFACTORY_HPP */
