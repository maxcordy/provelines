/*
 * ChangeListener.hpp
 *
 * Created on 2013-08-13
 */

#ifndef CORE_FTS_FSTM_CHANGELISTENER_HPP
#define	CORE_FTS_FSTM_CHANGELISTENER_HPP

#include <string>

namespace core {
namespace fts {
namespace fstm {

/**
 * ChangeListener is an interface that defines a listener that is notified of
 * state changes.
 *
 * @author mwi
 */
class ChangeListener {

public:
    /**
     * @requires newState.length > 0
     * @modifies anything
     * @effects Informs the state has changed to the state labeled 'newState'
     *           in response to the event named 'newEvent'.
     */
    virtual void stateChanged(const std::string & newState,
        const std::string & newEvent) = 0;

    // mandatory virtual destructor
    virtual ~ChangeListener() {}
};

} // namespace fstm
} // namespace fts
} // namespace core

#endif	/* CORE_FTS_FSTM_CHANGELISTENER_HPP */
