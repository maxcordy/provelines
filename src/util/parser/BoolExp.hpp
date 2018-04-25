/*
 * BoolExp.hpp
 *
 * Created on 2013-08-09
 */

#ifndef UTIL_PARSER_BOOLEXP_HPP
#define UTIL_PARSER_BOOLEXP_HPP

#include "forwards.hpp"
#include "util/Uncopyable.hpp"

namespace util {
namespace parser {

/**
 * BoolExp is an immutable Boolean expression.
 *
 * @author mwi
 */
class BoolExp : private util::Uncopyable {

public:
    /**
     * @modifies visitor
     * @effects Anything
     */
    virtual void accept(Visitor & visitor) const = 0;

    // mandatory virtual destructor
    virtual ~BoolExp() {}
};

} // namespace parser
} // namespace util

#endif /* UTIL_PARSER_BOOLEXP_HPP */
