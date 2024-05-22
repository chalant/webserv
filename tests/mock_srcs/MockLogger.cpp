#include "../mock_includes/MockLogger.hpp"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

/*
 * MockLogger class is a mock implementation of the ILogger interface
 * This mock implementation is used for testing purposes to simulate the
 * behavior of the ILogger interface during unit testing without relying on the
 * actual implementation.
 */

// Constructor
MockLogger::MockLogger() : ILogger() {};

// Destructor
MockLogger::~MockLogger() {};

// Default log method
int MockLogger::log(const std::string &message)
{
    // Construct the log message string
    std::string log_message_string =
        this->m_getCurrentTimestamp() + " " + message + "\n";

    std::cout << log_message_string;
    return 0;
};

// Method to log error events
int MockLogger::log(const LogLevel logLevel, const std::string &message)
{
    static_cast<void>(logLevel);
    // Construct the log message string
    // std::string log_message_string = this->m_getCurrentTimestamp() + " [" +
    // this->m_log_level_helper.logLevelStringMap(logLevel) + "] " + message +
    // "\n";
    std::string log_message_string =
        this->m_getCurrentTimestamp() + " [LOGLEVEL] " + message + "\n";

    std::cout << log_message_string;
    return 0;
};

// Method to log access events
int MockLogger::log(const IConnection &connection)
{
    static_cast<void>(connection);
    return 0;
};

// Method to configure the Logger instance
void MockLogger::configure(ILoggerConfiguration &configuration)
{
    static_cast<void>(configuration);
};

// Helper method to get the current timestamp
std::string MockLogger::m_getCurrentTimestamp() const
{
    std::stringstream stream;
    std::time_t current_time = std::time(NULL);
    char buffer[ 80 ];

    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&current_time));
    stream << buffer;
    return stream.str();
}

// Path: tests/mock_includes/MockRequest.hpp
