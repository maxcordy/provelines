/*
 * Minimizer.hpp
 *
 * Created on 2013-08-29
 */

#ifndef MATH_MINIMIZE_MINIMIZER_HPP
#define	MATH_MINIMIZE_MINIMIZER_HPP

#include "util/Uncopyable.hpp"

#include <string>

namespace math {
namespace minimize {

/**
 * Minimizer is a minimizer of Boolean expressions.
 *
 * @author mwi
 */
class Minimizer : private util::Uncopyable {

private:
    /* Temp file path to use with Minimize.jar */
    static const std::string MinimizeFilePath;

    /**
     * Representation Invariant:
     *   I(c) = c.MinimizeFilePath.length > 0
     */

public:
    /**
     * @effects Makes this be a new minimizer.
     */
    Minimizer();

    // The default destructor is fine.

    /**
     * @return the minimized Boolean expression 'expression'.
     */
    std::string minimize(const std::string & expression) const;

private:
    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

} // minimize
} // math

#endif	/* MATH_MINIMIZE_MINIMIZER_HPP */
