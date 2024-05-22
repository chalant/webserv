#ifndef MOCKLOGGER_HPP
#define MOCKLOGGER_HPP

/*
 * MockLogger class is a mock implementation of the ILogger interface
 * This mock implementation is used for testing purposes to simulate the
 * behavior of the ILogger interface during unit testing without relying on the
 * actual implementation.
 */

#include "../../includes/logger/ILogger.hpp" // Include the base interface header

class MockLogger : public ILogger
{
private:
    // Helper method to get the current timestamp
    std::string m_getCurrentTimestamp() const;

public:
    // Constructor
    MockLogger();

    // Destructor
    ~MockLogger();

    // Methods implementing the ILogger interface
    // These methods are overridden to provide custom behavior for testing.

    // Logging methods
    virtual int log(const std::string &); // Default log method
    virtual int log(const LogLevel,
                    const std::string &); // Method to log error messages
    virtual int log(const IConnection &); // Method to log access events

    // Configuration method
    virtual void configure(
        ILoggerConfiguration &); // Method to configure the Logger instance
};

#endif // MOCKLOGGER_HPP
// Path: tests/mock_includes/MockRequest.hpp
