#include "../../includes/logger/Logger.hpp"
#include <iomanip>
#include <iostream>
#include <ctime>

/*
 * Logger Class:
 * Provides functionality to log errors and access events.
 * By default logs to std err; this can be overridden through the config file.
 * It supports logging to specified files with customizable log levels. Error and access logs are handled separately.
 * Log messages are not immediately written but instead pused to the BufferManager who handles writing without blocking.
 *
 * Usage:
 * - When adding logging functionality to new components or modules, use the log() methods to add log messages to the buffer.
 * - Use the appropriate log() override
 *
 * Example error log:
 * this->_logger.log(INFO, "listening on port 8080");
 * Output in error log: 2011-01-01T01:11:11 [INFO] "listening on port 8080"
 *
 * Example access log:
 * this->_logger.log(request, response);
 * Output in access log: timestamp="2011-01-01T01:11:11" clientIP="127.0.0.1" method="GET" requestURI="/index.php" httpVersion="HTTP/1.1" etc.
 */

Logger::Logger(IBufferManager &bufferManager)
    : _configuration(NULL),
      _bufferManager(bufferManager),
      _logLevelHelper() 
{
    // Log the initialization of the Logger
    this->log(VERBOSE, "Logger initialized.");
}

// Destructor: Handles cleanup tasks like flushing buffer and closing log file descriptor
Logger::~Logger() 
{
}

// Method to get the current timestamp
std::string Logger::_getCurrentTimestamp() const
{
    std::stringstream stream;
    std::time_t currentTime = std::time(NULL);
	char	buffer[80];

	strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&currentTime));
    stream << buffer;
    return stream.str();
}

// Default Method to log error messages
int Logger::log(const std::string &message)
{
    // If the Logger is disabled, return without logging
    if (this->_configuration != NULL && this->_configuration->getErrorLogEnabled() == false)
        return -1;

    // Construct the log message string
    std::string logMessage = this->_getCurrentTimestamp() + " " + message + "\n";

    // Push the log message to the log file buffer if configured, otherwise push to stderr buffer
    return this->_pushToBuffer(logMessage, this->_configuration ? this->_configuration->getErrorLogFileDescriptor() : STDERR_FILENO);
}

// Method to log error messages
int Logger::log(LogLevel logLevel, const std::string &message)
{
    // If the Logger is disabled or the log level below threshold, return without logging
    if (this->_configuration != NULL &&
        (this->_configuration->getErrorLogEnabled() == false || logLevel < this->_configuration->getLogLevel()))
        return -1;

    // Construct the log message string
    std::string logMessage = this->_getCurrentTimestamp() + " [" + this->_logLevelHelper.logLevelStringMap(logLevel) + "] " + message + "\n";

    // Push the log message to the log file buffer if configured, otherwise push to stderr buffer
    return this->_pushToBuffer(logMessage, this->_configuration ? this->_configuration->getErrorLogFileDescriptor() : STDERR_FILENO);
}

// Method to log access events
int Logger::log(const IConnection &connection)
{
    // If the Logger is configured and confirmed disabled, return without logging
    if (this->_configuration != NULL && this->_configuration->getAccessLogEnabled() == false)
        return -1;

    // Get a reference to the Request and Response objects
    const IRequest &request = connection.getRequest();
    const IResponse &response = connection.getResponse();

    // Create a temporary stringstream object to construct the log message
    std::ostringstream logBufferStream;
    logBufferStream << "timestamp=\"" << this->_getCurrentTimestamp() << "\" "
                    << "clientIP=\"" << connection.getIp() << "\" "
                    << "clientPort=\"" << connection.getPort() << "\" "
                    << "method=\"" << request.getMethodString() << "\" "
                    << "requestUri=\"" << request.getUri() << "\" "
                    << "httpVersion=\"" << request.getHttpVersion() << "\" "
                    << "statusCode=\"" << response.getStatusCodeString() << "\" "
                    << "responseSize=\"" << response.getResponseSizeString() << "\" "
                    << "userAgent=\"" << request.getHeaderValue(USER_AGENT) << "\" "
                    << "referrer=\"" << request.getHeaderValue(REFERER) << "\" ";

    // Add request headers to the log message
    this->_appendMapToLog(logBufferStream, "requestHeaders", request.getHeadersStringMap());

    // Add response headers to the log message
    this->_appendMapToLog(logBufferStream, "responseHeaders", response.getHeadersStringMap());

    // Add cookies to the log message
    this->_appendMapToLog(logBufferStream, "Cookies", request.getCookies());

    // Convert the log message string to a vector of chars
    std::string logMessage = logBufferStream.str();

    // Push the log message to the access log file buffer if configured, otherwise push to stderr buffer
    return this->_pushToBuffer(logMessage, this->_configuration ? this->_configuration->getAccessLogFileDescriptor() : STDERR_FILENO);
}

// Method to append map to log message
void Logger::_appendMapToLog(std::ostringstream &logBufferStream, const std::string &fieldName, const std::map<std::string, std::string> &map) const
{
    std::ostringstream mapStream;

    mapStream << fieldName << "={";
    for (std::map<std::string, std::string>::const_iterator it = map.begin(); it != map.end(); ++it)
    {
        mapStream << it->first << ": " << it->second;
		std::map<std::string, std::string>::const_iterator next = it;
		next++;
        if (next != map.end())
        {
            mapStream << ", ";
        }
    }
    mapStream << "} ";

    logBufferStream << mapStream.str(); // Append the constructed string to the logBufferStream
}

// Configuration method
void Logger::configure(ILoggerConfiguration &configuration)
{
    this->_configuration = &configuration;
}

// Method to push log messages to the buffer
// returns 1 if a flush is requested, or 0 otherwise
int Logger::_pushToBuffer(const std::string &logMessage, const int fileDescriptor)
{ 
    // Copy output to stderr
    std::cerr << logMessage;

    // Convert the log message string to a vector of chars
    std::vector<char> logMessageVector(logMessage.begin(), logMessage.end());

    // Push the log message to the buffer, returns 1 if a flush is requested
    int returnValue = this->_bufferManager.pushFileBuffer(fileDescriptor, logMessageVector);
    if (returnValue == 1 && this->_configuration)
    {
        // If the buffer threshold is reached, request a flush
        this->_configuration->requestFlush(fileDescriptor);
    }

    // Return the return value
    return returnValue;
}

// Path: includes/WebservExceptions.hpp
