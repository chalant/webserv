#ifndef LOGLEVELHELPER_HPP
#define LOGLEVELHELPER_HPP

/*
 * LogLevelHelper.hpp
 *
 * The LogLevelHelper class provides functionality for working with log levels.
 * It defines an enum LogLevel to represent log levels and provides methods
 * to retrieve string representations of log levels and vice versa.
 *
 */

#include <string>
#include <vector>
#include <map>

// Forward declaration of WebServer exceptions
class WebservExceptions;

enum LogLevel
{
    VERBOSE = 0,  // VERBOSE messages
    DEBUG = 1,    // Debugging information
    INFO = 2,     // Informational messages
    WARN = 3,     // Warnings
    ERROR = 4,    // Errors
    CRITICAL = 5, // Critical errors
    UNKNOWN = 6   // Unknown log level
};

class LogLevelHelper
{
private:
    // Member variables
    const std::vector<std::string> _logLevelList;             // List of string representations of log levels
    const std::map<std::string, LogLevel> _stringLogLevelMap; // Map of string representations to LogLevel enum values
    const std::map<LogLevel, std::string> _logLevelStringMap; // Map of LogLevel enum values to string representations

    // Private member functions for initialization
    static std::vector<std::string> _setLogLevelList();
    static std::map<std::string, LogLevel> _setStringLogLevelMap();
    static std::map<LogLevel, std::string> _setLogLevelStringMap();

public:
    // Constructor
    LogLevelHelper();

    // Member functions to access data
    const std::string &logLevelStringMap(LogLevel logLevel) const; // Get string representation of LogLevel
    LogLevel stringLogLevelMap(const std::string &logLevel) const; // Get LogLevel enum value from string representation
};

#endif // LOGLEVELHELPER_HPP
// Path: includes/constants/LogLevelHelper.hpp
