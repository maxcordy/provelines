/*
 * PropertyTransition.hpp
 *
 * Created on 2013-07-22
 */

#ifndef CORE_FTS_PROPERTYTRANSITION_HPP
#define	CORE_FTS_PROPERTYTRANSITION_HPP

#include "util/Uncopyable.hpp"

namespace core {
namespace fts {

/**
 * PropertyTransition is an interface for immutable transitions of property
 * automata.
 *
 * @author mwi
 */
class PropertyTransition : private util::Uncopyable {

public:
    // mandatory virtual destructor
    virtual ~PropertyTransition() {}
};

} // namespace core
} // namespace fts

#endif	/* CORE_FTS_PROPERTYTRANSITION_HPP */
