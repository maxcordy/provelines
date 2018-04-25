/*
 * ltl2ba.hpp
 *
 * Created on 2013-07-31
 */

#ifndef NEVERCLAIM_LTL2BA_HPP
#define	NEVERCLAIM_LTL2BA_HPP

#include <string>

namespace neverclaim {

/**
 * @requires filePath is a valid path to a file
 * @modifies the file at 'filePath'
 * @effects Converts the LTL property 'ltlProperty' into a Büchi automaton
 *           written in the file at 'filePath' in the form of a never claim.
 * @return true iff ltlProperty is a valid LTL property
 */
bool ltl2ba(const std::string & ltlProperty, const std::string & filePath);

} // namespace

#endif	/* NEVERCLAIM_LTL2BA_HPP */
