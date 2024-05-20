#include "../../includes/constants/LogLevelHelper.hpp"
#include "../../includes/exception/WebservExceptions.hpp"

/*
 * LogLevelHelper.hpp
 *
 * The LogLevelHelper class provides functionality for working with log levels.
 * It defines an enum LogLevel to represent log levels and provides methods
 * to retrieve string representations of log levels and vice versa.
 *
 */

// Constructor initializes member variables using helper functions
LogLevelHelper::LogLevelHelper()
    : _logLevelList(_setLogLevelList()),
      _stringLogLevelMap(_setStringLogLevelMap()),
      _logLevelStringMap(_setLogLevelStringMap())
{
}

// Get string representation of LogLevel enum value
const std::string &LogLevelHelper::logLevelStringMap(LogLevel logLevel) const
{
    if (_logLevelStringMap.find(logLevel) != _logLevelStringMap.end())
    {
        return _logLevelStringMap.at(logLevel);
    }
    else
    {
        throw UnknownLogLevelError();
    }
}

// Get LogLevel enum value from string representation
LogLevel LogLevelHelper::stringLogLevelMap(const std::string &logLevel) const
{
    if (_stringLogLevelMap.find(logLevel) != _stringLogLevelMap.end())
    {
        return _stringLogLevelMap.at(logLevel);
    }
    else
    {
        throw UnknownLogLevelError(logLevel);
    }
}

// Helper function to initialize LogLevelList with string representations of
// HTTP logLevels
std::vector<std::string> LogLevelHelper::_setLogLevelList()
{
    std::vector<std::string> logLevelList;

    // Add string representations of HTTP logLevels to LogLevelList
    logLevelList.push_back("verbose");
    logLevelList.push_back("debug");
    logLevelList.push_back("info");
    logLevelList.push_back("warn");
    logLevelList.push_back("error");
    logLevelList.push_back("critical");
    logLevelList.push_back("unknown");

    return logLevelList;
}

// Helper function to initialize stringLogLevelMap with mappings from string
// representations to LogLevel enum values
std::map<std::string, LogLevel> LogLevelHelper::_setStringLogLevelMap()
{
    std::map<std::string, LogLevel> stringLogLevelMap;

    // Add mappings from string representations to LogLevel enum values
    stringLogLevelMap[ "verbose" ] = VERBOSE;
    stringLogLevelMap[ "debug" ] = DEBUG;
    stringLogLevelMap[ "info" ] = INFO;
    stringLogLevelMap[ "warn" ] = WARN;
    stringLogLevelMap[ "error" ] = ERROR;
    stringLogLevelMap[ "critical" ] = CRITICAL;
    stringLogLevelMap[ "unknown" ] = UNKNOWN;

    return stringLogLevelMap;
}

// Helper function to initialize logLevelStringMap with mappings from LogLevel
// enum values to string representations
std::map<LogLevel, std::string> LogLevelHelper::_setLogLevelStringMap()
{
    std::map<LogLevel, std::string> logLevelStringMap;

    // Add mappings from LogLevel enum values to string representations
    logLevelStringMap[ VERBOSE ] = "verbose";
    logLevelStringMap[ DEBUG ] = "debug";
    logLevelStringMap[ INFO ] = "info";
    logLevelStringMap[ WARN ] = "warn";
    logLevelStringMap[ ERROR ] = "error";
    logLevelStringMap[ CRITICAL ] = "critical";
    logLevelStringMap[ UNKNOWN ] = "unknown";

    return logLevelStringMap;
}

// Path: includes/constants/LogLevelHelper.hpp
