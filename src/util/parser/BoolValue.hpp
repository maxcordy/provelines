/*
 * BoolValue.hpp
 *
 * Created on 2013-08-09
 */

#ifndef UTIL_PARSER_BOOLVALUE_HPP
#define UTIL_PARSER_BOOLVALUE_HPP

#include "BoolExp.hpp"

namespace util {
namespace parser {

/**
 * BoolValue is a Boolean value, i.e., either true or false.
 *
 * @author mwi
 */
class BoolValue : public BoolExp {

private:
    const bool value;

    /*
     * Representation Invariant:
     *   I(c) = true
     */

public:
    /**
     * @effects Makes this be a new Boolean value with the value 'value'.
     */
    explicit BoolValue(bool value);

    // The default destructor is fine.

    /**
     * @return the value of this.
     */
    bool getValue() const;

    // overridden method
    void accept(Visitor & visitor) const;

};

} // namespace parser
} // namespace util

#endif /* UTIL_PARSER_BOOLVALUE_HPP */
