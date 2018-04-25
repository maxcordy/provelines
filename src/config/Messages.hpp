/*
 * Messages.hpp
 *
 * Created on 2013-09-02
 */

#ifndef CONFIG_MESSAGES_HPP
#define	CONFIG_MESSAGES_HPP

#include "util/Uncopyable.hpp"

#include <string>

namespace config {

/**
 * Messages is a utility class that holds the user messages used by the
 * application.
 *
 * @author mwi
 */
class Messages : private util::Uncopyable {

private:
    // this class cannot be instantiated
    Messages() {};

public:
    /**
     * @return a message indicating a wrong number of arguments was passed to
     *          the command line.
     */
    static std::string wrongNumberOfArgs();

    /**
     * @return a message indicating the argument 'arg' is invalid.
     */
    static std::string invalidArg(const std::string & arg);

    /**
     * @requires envVar.length > 0
     * @return a message indicating the environment variable 'envVar' is not
     *          set.
     */
    static std::string envVarNotSet(const std::string & envVar);

    /**
     * @return a message indicating the presence of an empty file path.
     */
    static std::string emptyFilePath();

    /**
     * @return a message indicating the file at 'filePath' cannot be opened.
     */
    static std::string errorOpeningFile(const std::string & filePath);

    /**
     * @requires jsonFilePath.length > 0
     * @return a message indicating the JSON file at 'jsonFilePath' contains
     *          some syntax error.
     */
    static std::string invalidJson(const std::string & jsonFilePath);
};

} // namespace config

#endif	/* CONFIG_MESSAGES_HPP */
