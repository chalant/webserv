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
      _logLevelStringMap(_setLogLevelStringMap()) {}

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

// Helper function to initialize LogLevelList with string representations of HTTP logLevels
std::vector<std::string> LogLevelHelper::_setLogLevelList()
{
    std::vector<std::string> logLevelList;

    // Add string representations of HTTP logLevels to LogLevelList
    logLevelList.push_back("VERBOSE");
    logLevelList.push_back("DEBUG");
    logLevelList.push_back("INFO");
    logLevelList.push_back("WARN");
    logLevelList.push_back("ERROR");
    logLevelList.push_back("CRITICAL");
    logLevelList.push_back("UNKNOWN");

    return logLevelList;
}

// Helper function to initialize stringLogLevelMap with mappings from string representations to LogLevel enum values
std::map<std::string, LogLevel> LogLevelHelper::_setStringLogLevelMap()
{
    std::map<std::string, LogLevel> stringLogLevelMap;

    // Add mappings from string representations to LogLevel enum values
    stringLogLevelMap["VERBOSE"] = VERBOSE;
    stringLogLevelMap["DEBUG"] = DEBUG;
    stringLogLevelMap["INFO"] = INFO;
    stringLogLevelMap["WARN"] = WARN;
    stringLogLevelMap["ERROR"] = ERROR;
    stringLogLevelMap["CRITICAL"] = CRITICAL;
    stringLogLevelMap["UNKNOWN"] = UNKNOWN;

    return stringLogLevelMap;
}

// Helper function to initialize logLevelStringMap with mappings from LogLevel enum values to string representations
std::map<LogLevel, std::string> LogLevelHelper::_setLogLevelStringMap()
{
    std::map<LogLevel, std::string> logLevelStringMap;

    // Add mappings from LogLevel enum values to string representations
    logLevelStringMap[VERBOSE] = "VERBOSE";
    logLevelStringMap[DEBUG] = "DEBUG";
    logLevelStringMap[INFO] = "INFO";
    logLevelStringMap[WARN] = "WARN";
    logLevelStringMap[ERROR] = "ERROR";
    logLevelStringMap[CRITICAL] = "CRITICAL";
    logLevelStringMap[UNKNOWN] = "UNKNOWN";

    return logLevelStringMap;
}

// Path: includes/constants/LogLevelHelper.hpp
