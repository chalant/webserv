#ifndef MOCKLOGGER_HPP
#define MOCKLOGGER_HPP

/*
 * MockLogger class is a mock implementation of the ILogger interface
 * This mock implementation is used for testing purposes to simulate the behavior of the ILogger interface
 * during unit testing without relying on the actual implementation.
 */

#include "../../includes/ILogger.hpp" // Include the base interface header

class MockLogger : public ILogger
{
public:
    // Constructor
    MockLogger();

    // Destructor
    ~MockLogger();

    // Methods implementing the ILogger interface
    // These methods are overridden to provide custom behavior for testing.
    virtual void errorLog(const LogLevel logLevel, const std::string &message); // Method to log error events
    virtual void accessLog(const IRequest &request, const Response &response);  // Method to log access events
    virtual void writeLogBufferToFile();                                      // Method to write the log buffer to the log file
};

#endif // MOCKLOGGER_HPP
// Path: tests/mock_includes/MockRequest.hpp
