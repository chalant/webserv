#ifndef LOGLEVEL_H
#define LOGLEVEL_H

// Log levels

#include <string>
#include <map>

// Enum representing different log levels

enum LogLevel
{
    VERBOSE = 0, // VERBOSE messages
    DEBUG = 1,   // Debugging information
    INFO = 2,    // Informational messages
    WARNING = 3, // Warnings
    ERROR = 4,   // Errors
    CRITICAL = 5 // Critical errors
};

// Array of log level strings

const std::string LogLevelStrings[] = {
    "[VERBOSE]",
    "[DEBUG]",
    "[INFO]",
    "[WARNING]",
    "[ERROR]",
    "[CRITICAL]"};

// Map of log level strings to LogLevel enum values

const std::map<std::string, LogLevel> LogLevelMap{
    {"VERBOSE", LogLevel::VERBOSE},
    {"DEBUG", LogLevel::DEBUG},
    {"INFO", LogLevel::INFO},
    {"WARNING", LogLevel::WARNING},
    {"ERROR", LogLevel::ERROR},
    {"CRITICAL", LogLevel::CRITICAL}};

#endif // LOGLEVEL_H
// Path: includes/constants/LogLevel.hpp
