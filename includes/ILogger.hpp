#ifndef ILOGGER_HPP
#define ILOGGER_HPP

#include <string>
#include "constants/LogLevelHelper.hpp"

class IRequest;
class Response;

class ILogger
{
public:
    virtual ~ILogger() {};

    // Logging methods
    virtual void errorLog(const LogLevel logLevel, const std::string &message) = 0; // Method to log error messages
    virtual void accessLog(const IRequest &request, const Response &response) = 0;  // Method to log access events
    virtual void writeLogBufferToFile() = 0;                                        // Method to write the log buffer to the log file
};

#endif // ILOGGER_HPP
       // Path: includes/ILogger.hpp
