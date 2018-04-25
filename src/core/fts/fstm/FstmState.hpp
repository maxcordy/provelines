/*
 * FstmState.hpp
 *
 * Created on 2013-07-26
 */

#ifndef CORE_FTS_FSTM_FSTMSTATE_HPP
#define CORE_FTS_FSTM_FSTMSTATE_HPP

#include "forwards.hpp"
#include "core/fts/State.hpp"
#include "util/Hashable.hpp"
#include "util/Uncopyable.hpp"

#include <string>
#include <tr1/memory>
#include <tr1/unordered_map>

namespace core {
namespace fts {
namespace fstm {

/**
 * FstmStates are immutable states of a featured state machine (FSTM). An
 * FSTM state has a name, aka a label, and for each unique name there is at most
 * one state. Because FstmStates implement the State interface, FSTM states can
 * also be considered as FTS states.
 *
 * @author mwi
 */
class FstmState : public State {

private:
    static std::tr1::unordered_map<std::string, FstmStatePtr> nameTable;

    const std::string name;

    /*
     * Representation Invariant:
     *   I(c) = c.name.length > 0
     *     for all <name, state> in c.nameTable .(name = state.name)
     */

    /**
     * @requires name not empty
     * @effects Makes this be a new FSTM state named 'name'.
     */
    explicit FstmState(const std::string & name);

public:
    // overridden methods
    std::string getName() const;
    bool equals(const State & state) const;
    unsigned int hashCode() const;

    /**
     * @requires name not empty
     * @return the state named 'name'.
     */
    static FstmStatePtr makeState(const std::string & name);

private:
    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

} // namespace fstm
} // namespace fts
} // namespace core

#endif /* CORE_FTS_FSTM_FSTMSTATE_HPP */
