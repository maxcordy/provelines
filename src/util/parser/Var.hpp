/*
 * Var.hpp
 *
 * Created on 2013-08-09
 */

#ifndef UTIL_PARSER_VAR_HPP
#define UTIL_PARSER_VAR_HPP

#include "BoolExp.hpp"

#include <string>

namespace util {
namespace parser {

/**
 * Var is a Boolean variable with a name.
 *
 * @author mwi
 */
class Var : public BoolExp {

private:
    const std::string name;

    /*
     * Representation Invariant:
     *   I(c) = true
     */

public:
    /**
     * @effects Makes this be a new Boolean variable named 'name'.
     */
    explicit Var(const std::string & name);

    /**
     * @return the name of this.
     */
    std::string getName() const;

    // The default destructor is fine.

    // overridden methods
    void accept(Visitor & visitor) const;
};

} // namespace parser
} // namespace util

#endif /* UTIL_PARSER_VAR_HPP */
