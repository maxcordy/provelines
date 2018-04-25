/*
 * BinExp.hpp
 *
 * Created on 2013-08-09
 */

#ifndef UTIL_PARSER_BINEXP_HPP
#define UTIL_PARSER_BINEXP_HPP

#include "BoolExp.hpp"

#include <tr1/memory>

namespace util {
namespace parser {

/**
 * BinExp is an abstract binary Boolean expression. It has a left-hand side and
 * a right-hand side Boolean expression.
 *
 * @author mwi
 */
class BinExp : public BoolExp {

private:
    const std::tr1::shared_ptr<BoolExp> lhs;
    const std::tr1::shared_ptr<BoolExp> rhs;

    /*
     * Representation Invariant:
     *   I(c) = c.lhs != null && c.rhs != null
     */

protected:
    /**
     * @requires lhs and rhs are not null
     * @effects Makes this be a new binary Boolean expression with the
     *           left-hand side expression 'lhs' and the right-hand side
     *           expression 'rhs'.
     */
    BinExp(const std::tr1::shared_ptr<BoolExp> & lhs,
        const std::tr1::shared_ptr<BoolExp> & rhs);

public:
    // mandatory virtual destructor
    virtual ~BinExp() {}

    /**
     * @return the left-hand side expression of this.
     */
    virtual std::tr1::shared_ptr<BoolExp> getLeftExp() const;

    /**
     * @return the right-hand side expression of this.
     */
    virtual std::tr1::shared_ptr<BoolExp> getRightExp() const;

protected:
    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

} // namespace parser
} // namespace util

#endif /* UTIL_PARSER_BINEXP_HPP */
