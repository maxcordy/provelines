/*
 * FstmEvent.hpp
 *
 * Created on 2013-07-26
 */

#ifndef CORE_FTS_FSTM_FSTMEVENT_HPP
#define CORE_FTS_FSTM_FSTMEVENT_HPP

#include "util/Uncopyable.hpp"

#include <string>
#include <tr1/memory>
#include <tr1/unordered_map>

namespace core {
namespace fts {
namespace fstm {

    /**
     * FstmEvents are immutable events of a featured state machine (FSTM). An
     * FSTM event has a name, and for each unique name there is at most one
     * event.
     *
     * @author mwi
     */
    class FstmEvent : private util::Uncopyable {

    private:
        static std::tr1::unordered_map<std::string,
            std::tr1::shared_ptr<FstmEvent> > nameTable;

        std::string name;

        /*
         * Representation Invariant:
         *   I(c) = c.name.length > 0
         *     for all <name, event> in c.nameTable .(name = state.event)
         */

        /**
         * @requires name not empty
         * @effects Makes this be a new FSTM event named 'name'.
         */
        explicit FstmEvent(const std::string & name);

    public:
        /**
         * @return the name of this.
         */
        std::string getName() const;

        /**
         * @requires name not empty
         * @return the event named 'name'.
         */
        static std::tr1::shared_ptr<FstmEvent> makeEvent(const std::string &
            name);

    private:
        /**
         * @effects Asserts the rep invariant holds for this.
         */
        void checkRep() const;
    };

} // namespace fstm
} // namespace fts
} // namespace core

#endif /* CORE_FTS_FSTM_FSTMEVENT_HPP */
