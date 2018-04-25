/*
 * PropertyState.hpp
 *
 * Created on 2013-08-13
 */

#ifndef CORE_FTS_PROPERTYSTATE_HPP
#define	CORE_FTS_PROPERTYSTATE_HPP

#include "util/Hashable.hpp"
#include "util/Uncopyable.hpp"

#include <string>

namespace core {
namespace fts {

/**
 * PropertyState is an interface for immutable states of property automata.
 *
 * Specification Fields:
 *   - label : string   // The label of the state.
 *   - accepting : bool // Whether the state is an accepting state.
 *   - final : bool     // Whether the state is an accepting and final state.
 *
 * Abstract Invariant:
 *   - final => accepting
 *
 * @author mwi
 */
class PropertyState : public util::Hashable<PropertyState>,
        private util::Uncopyable {
    
public:

    /**
     * @return this.label
     */
    virtual const std::string & getLabel() const = 0;

    /**
     * @return this.accepting
     */
    virtual bool isAccepting() const = 0;

    /**
     * @return this.final
     */
    virtual bool isFinal() const = 0;

    // mandatory virtual destructor
    virtual ~PropertyState() {}
};

} // namespace fts
} // namespace core

#endif	/* CORE_FTS_PROPERTYSTATE_HPP */
