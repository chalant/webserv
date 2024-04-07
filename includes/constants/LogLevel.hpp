#ifndef LOGLEVEL_H
#define LOGLEVEL_H

// Enumerated type representing different log levels

#include <string>

enum LogLevel
{
    VERBOSE = 0, // VERBOSE messages
    DEBUG = 1,   // Debugging information
    INFO = 2,    // Informational messages
    WARNING = 3, // Warnings
    ERROR = 4,   // Errors
    CRITICAL = 5 // Critical errors
};

const std::string LogLevelStrings[] = {
    "[VERBOSE]",
    "[DEBUG]",
    "[INFO]",
    "[WARNING]",
    "[ERROR]",
    "[CRITICAL]"};

#endif // LOGLEVEL_H
// Path: includes/constants/LogLevel.hpp
