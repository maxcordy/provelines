/*
 * OrExp.hpp
 *
 * Created on 2013-08-09
 */

#ifndef UTIL_PARSER_OREXP_HPP
#define UTIL_PARSER_OREXP_HPP

#include "BinExp.hpp"

namespace util {
namespace parser {

/**
 * OrExp is a logical OR Boolean expression.
 *
 * @author mwi
 */
class OrExp : public BinExp {
public:
    /**
     * @requires lhs and rhs are not null
     * @effects Makes this be a new logical OR Boolean expression with the
     *           left-hand side expression 'lhs' and the right-hand side
     *           expression 'rhs'.
     */
    OrExp(const std::tr1::shared_ptr<BoolExp> & lhs,
        const std::tr1::shared_ptr<BoolExp> & rhs);

    // The default destructor is fine.

    // overridden method
    void accept(Visitor & visitor) const;
};

} // namespace parser
} // namespace util

#endif /* UTIL_PARSER_OREXP_HPP */
