/*
 * State.hpp
 *
 * Created on 2013-07-22
 */

#ifndef CORE_FTS_STATE_HPP
#define	CORE_FTS_STATE_HPP

#include "forwards.hpp"

#include "util/Hashable.hpp"
#include "util/Uncopyable.hpp"

#include <string>

namespace core {
namespace fts {

/**
 * State is an interface for immutables states of featured transition systems
 * (FTS).
 *
 * Specification Fields:
 *   - name: string // The name of the state.
 *
 * @author mwi
 */
class State : public util::Hashable<State>, private util::Uncopyable {

public:
    /**
     * @return this.name
     */
    virtual std::string getName() const = 0;

    // mandatory virtual destructor
    virtual ~State() {}
};

} // namespace fts
} // namespace core

#endif	/* CORE_FTS_STATE_HPP */
