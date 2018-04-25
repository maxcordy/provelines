/*
 * CommandLine.hpp
 *
 * Created on 2013-09-02
 */

#ifndef UI_COMMANDLINE_HPP
#define	UI_COMMANDLINE_HPP

#include "util/Uncopyable.hpp"

#include <string>

namespace ui {

/**
 * CommandLine is an immutable command line entered by a user and containing
 * various parameters for an execution of ProVeLines.
 *
 * @author mwi
 */
class CommandLine : private util::Uncopyable {

private:
    bool verbose;
    std::string inputPath;

    /*
     * Representation Invariant:
     *   I(c) = true
     */

public:
    /**
     * @effects Makes this be a new command line containing 'argc' arguments
     *           in the 'argv' array.
     * @throws NotPossibleException iff there are some invalid arguments or
     *          some mandatory arguments are missing.
     */
    CommandLine(int argc, const char * argv[]);

    // The default destructor is fine.

    /**
     * @return true iff the verbose flag is present.
     */
    bool isVerbose() const;

    /**
     * @return the mandatory input path.
     */
    std::string getInputPath() const;

    /**
     * @return a description of the typical usage of this.
     */
    static std::string getUsage();

private:
    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

} // namespace ui

#endif	/* UI_COMMANDLINE_HPP */
