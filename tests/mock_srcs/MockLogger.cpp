#include "../mock_includes/MockLogger.hpp"

/*
 * MockLogger class is a mock implementation of the ILogger interface
 * This mock implementation is used for testing purposes to simulate the behavior of the ILogger interface
 * during unit testing without relying on the actual implementation.
 */

// Constructor
MockLogger::MockLogger() : ILogger(){};

// Destructor
MockLogger::~MockLogger(){};

// Method to log error events
void MockLogger::errorLog(const LogLevel logLevel, const std::string &message)
{
    static_cast<void>(logLevel);
    static_cast<void>(message);
};

// Method to log access events
void MockLogger::accessLog(const IRequest &request, const Response &response)
{
    static_cast<void>(request);
    static_cast<void>(response);
};

// Method to write the log buffer to the log file
void MockLogger::writeLogBufferToFile(){}; // Method to write the log buffer to the log file

// Getter method for log file descriptor
int MockLogger::getLogFileDescriptor() const
{
    return 0;
};

// Method to configure the Logger instance
void MockLogger::configure(const IConfiguration *){};

// Path: tests/mock_includes/MockRequest.hpp