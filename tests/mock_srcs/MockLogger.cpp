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
    // Construct the log message string
    //std::string logMessageString = this->_getCurrentTimestamp() + " [" + this->_logLevelHelper.logLevelStringMap(logLevel) + "] " + message + "\n";
    std::string logMessageString = this->_getCurrentTimestamp() + " [LOGLEVEL] " + message + "\n";
    std::vector<char> logMessage(logMessageString.begin(), logMessageString.end());

    std::cout << logMessageString;
};

// Method to log access events
void MockLogger::accessLog(const IRequest &request, const Response &response)
{
    static_cast<void>(request);
    static_cast<void>(response);
};

// Method to configure the Logger instance
void MockLogger::configure(ILoggerConfiguration &configuration){
    static_cast<void>(configuration);
};

// Helper method to get the current timestamp
std::string MockLogger::_getCurrentTimestamp() const
{
    std::stringstream stream;

    std::time_t currentTime = std::time(NULL);
    stream << std::put_time(std::localtime(&currentTime), "%Y-%m-%d %H:%M:%S");
    return stream.str();
}

// Path: tests/mock_includes/MockRequest.hpp