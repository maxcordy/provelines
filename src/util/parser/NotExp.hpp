/*
 * NotExp.hpp
 *
 * Created on 2013-08-09
 */

#ifndef UTIL_PARSER_NOTEXP_HPP
#define UTIL_PARSER_NOTEXP_HPP

#include "BoolExp.hpp"

#include <tr1/memory>

namespace util {
namespace parser {

/**
 * NotExp is a logical NOT Boolean expression.
 *
 * @author mwi
 */
class NotExp : public BoolExp {

private:
    const std::tr1::shared_ptr<BoolExp> exp;

    /*
     * Representation Invariant:
     *   I(c) = c.exp != null
     */

public:
    /**
     * @requires exp != null
     * @effects Makes this be a new logical NOT Boolean expression for the
     *           expression 'exp'.
     */
    explicit NotExp(const std::tr1::shared_ptr<BoolExp> & exp);

    // The default destructor is fine.

    /**
     * @return the Boolean expression of this.
     */
    std::tr1::shared_ptr<BoolExp> getBoolExp() const;

    // overridden methods
    void accept(Visitor & visitor) const;

private:
    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

} // namespace parser
} // namespace util

#endif /* UTIL_PARSER_NOTEXP_HPP */
