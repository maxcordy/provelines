/*
 * Environment.hpp
 *
 * Created on 2013-09-02
 */

#ifndef CONFIG_ENVIRONMENT_HPP
#define	CONFIG_ENVIRONMENT_HPP

#include "util/Uncopyable.hpp"

#include <string>

namespace config {

/**
 * Environment is a utility class that provides access to some environment
 * variables used by the application.
 *
 * @author mwi
 */
class Environment : private util::Uncopyable {

private:
    /** The MINIMIZE_HOME environment variable. */
    static const std::string MinimizeHomeEnv;

    // this class cannot be instantiated
    Environment() {}

public:
    /**
     * @requires MINIMIZE_HOME is set
     * @return the value of the MINIMIZE_HOME environment variable.
     */
    static std::string getMinimizeHome();

    /**
     * @effects Asserts the environment variables used by this application are
     *           set.
     * @throws NotPossibleException iff an environment variable is not set.
     */
    static void checkVariables();

private:
    /**
     * @return true iff the environment variable 'envVar' is set.
     */
    static bool isSet(const std::string & envVar);
};

} // namespace config

#endif	/* ENVIRONMENT_HPP */
