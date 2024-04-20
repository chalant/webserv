#include "../includes/Logger.hpp"

/*
 * Logger Class:
 * Provides functionality to log errors and access events.
 * By default logs to std err; this can be overridden through the config file.
 * It supports logging to specified files with customizable log levels. Error and access logs are handled separately.
 * Log messages are not immediately written but instead pused to the BufferManager who handles writing without blocking.
 *
 * There are two instances of the Logger class in webserv:
 * 1. An error Logger, referenced for use in most classes.
 * 2. An access Logger, mainly intended for logging by the ResponseGenerator class.
 *
 * Usage:
 * - When adding logging functionality to new components or modules, use the errorLog() and accessLog() methods to add log messages to the buffer.
 * - Classes with access to the error Logger instance can log errors using the errorLog() method.
 * - Classes with access to the access Logger instance can log access events using the accessLog() method.
 *
 * Example error log:
 * this->_errorLogger.errorLog(INFO, "listening on port 8080");
 * Output in error log: timestamp="2011-01-01T01:11:11" level="[info]" message="listening on port 8080"
 *
 * Example access log:
 * this->_accesLogger.accessLog(request, response);
 * Output in access log: timestamp="2011-01-01T01:11:11" clientIP="127.0.0.1" method="GET" requestURI="/index.php" httpVersion="HTTP/1.1" etc.
 */

// Constructor for initializing an un-configured Logger instance
/*Logger::Logger(const LoggerType type, IBufferManager *bufferManager)
    : _type(type),
      _logFile((type == ERRORLOGGER) ? "./error.log" : "./access.log"),
      _bufferManager(bufferManager),
      _logLevel(INFO),
      _logFileDescriptor(open(this->_logFile.c_str(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)),
      _enabled(true),
      _logLevelHelper()
{
    if (this->_configuration->getLogFileDescriptor() == -1)
        throw LogFileOpenError(); // Opening log file failed
    if (type == ERRORLOGGER)
        this->errorLog(DEBUG, "Error Logger created");
    else // type == ACCESSLOGGER
        this->errorLog(DEBUG, "Access Logger created");
}

// Constructor for initializing Logger instance based on type (ERRORLOGGER/ACCESSLOGGER) and IConfiguration
Logger::Logger(const LoggerType type, IBufferManager *BufferManager, const IConfiguration *configuration)
    : _type(type),
      _logFile((type == ERRORLOGGER) ? configuration->getErrorLogFile() : configuration->getAccessLogFile()),
      _bufferManager(BufferManager),
      _logLevel((type == ERRORLOGGER) ? configuration->getLogLevel() : INFO),
      _logFileDescriptor(open(this->_logFile.c_str(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)),
      _enabled((type == ERRORLOGGER) ? configuration->getErrorLogEnabled() : configuration->getAccessLogEnabled()),
      _logLevelHelper()
{
    if (this->_configuration->getLogFileDescriptor() == -1)
        throw LogFileOpenError(); // Opening log file failed
    if (type == ERRORLOGGER)
    {
        this->errorLog(DEBUG, "Error Logger created");
    }
    else // type == ACCESSLOGGER
    {
        this->errorLog(DEBUG, "Access Logger created");
    }
}
*/

Logger::Logger(IBufferManager &bufferManager)
    : _configuration(nullptr),
      _bufferManager(bufferManager),
      _logLevelHelper() {}

// Destructor: Handles cleanup tasks like flushing buffer and closing log file descriptor
Logger::~Logger()
{
    this->errorLog(DEBUG, "Logger instance destruction started");
}

// Method to get the current timestamp
std::string Logger::_getCurrentTimestamp() const
{
    std::stringstream stream;

    std::time_t currentTime = std::time(NULL);
    stream << std::put_time(std::localtime(&currentTime), "%Y-%m-%d %H:%M:%S");
    return stream.str();
}

// Method to log error messages
void Logger::errorLog(LogLevel logLevel, const std::string &message)
{
    // If the Logger is configured and confirmed disabled, return without logging
    if (this->_configuration != nullptr &&
        (this->_configuration->getEnabled() == false || logLevel < this->_configuration->getLogLevel()))
        return;

    // Construct the log message string
    std::string logMessageString = this->_getCurrentTimestamp() + " [" + this->_logLevelHelper.logLevelStringMap(logLevel) + "] " + message + "\n";
    std::vector<char> logMessage(logMessageString.begin(), logMessageString.end());

    // if the logger is configured, log the log file buffer, otherwise log to stderr buffer
    int fileDescriptor = (this->_configuration != nullptr) ? this->_configuration->getLogFileDescriptor() : STDERR_FILENO;
    this->_bufferManager.pushFileBuffer(fileDescriptor, logMessage);
}

// Method to log access events
void Logger::accessLog(const IRequest &request, const Response &response)
{
    // If the Logger is configured and confirmed disabled, return without logging
    if (this->_configuration != nullptr && this->_configuration->getEnabled() == false)
        return;

    // Create a temporary stringstream object to construct the log message
    std::ostringstream logBufferStream;
    logBufferStream << "timestamp=\"" << this->_getCurrentTimestamp() << "\" "
                    << "clientIP=\"" << request.getClientIp() << "\" "
                    << "method=\"" << request.getMethodString() << "\" "
                    << "requestUri=\"" << request.getUri() << "\" "
                    << "httpVersion=\"" << request.getHttpVersion() << "\" "
                    << "statusCode=\"" << response.getStatusCodeString() << "\" "
                    << "responseSize=\"" << response.getResponseSizeString() << "\" "
                    << "userAgent=\"" << request.getHeaderValue(USER_AGENT) << "\" "
                    << "referrer=\"" << request.getHeaderValue(REFERER) << "\" ";

    // Add request headers to the log message
    this->_appendMapToLog(logBufferStream, "requestHeaders", request.getHeadersString());

    // Add response headers to the log message
    this->_appendMapToLog(logBufferStream, "responseHeaders", response.getHeadersString());

    // Add cookies to the log message
    this->_appendMapToLog(logBufferStream, "Cookies", request.getCookies());

    // Convert the log message string to a vector of chars
    std::string logMessage = logBufferStream.str();
    std::vector<char> logMessageVector(logMessage.begin(), logMessage.end());

    // If the logger is configured, log to the the access log file buffer, otherwise log to stderr buffer
    int fileDescriptor = (this->_configuration != nullptr) ? this->_configuration->getLogFileDescriptor() : STDERR_FILENO;
    this->_bufferManager.pushFileBuffer(this->_configuration->getLogFileDescriptor(), logMessageVector);
}

// Method to append map to log message
void Logger::_appendMapToLog(std::ostringstream &logBufferStream, const std::string &fieldName, const std::map<std::string, std::string> &map) const
{
    std::ostringstream mapStream;

    mapStream << fieldName << "={";
    for (std::map<std::string, std::string>::const_iterator it = map.begin(); it != map.end(); ++it)
    {
        mapStream << it->first << ": " << it->second;
        if (std::next(it) != map.end())
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

// Path: includes/WebservExceptions.hpp
