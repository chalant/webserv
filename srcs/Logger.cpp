#include "../includes/Logger.hpp"

/*
 * Logger Class:
 * Provides functionality to log errors and access events to separate files.
 * Default log files are "./error.log" and "./access.log"; this can be overridden through the config file.
 * It supports logging to specified files with customizable log levels. Error and access logs are handled separately.
 * Log messages are not immediately written to the log files but are deferred until an appropriate time, after poll() returns.
 *
 * Logger Buffer Usage Summary:
 * The Logger buffer serves as a centralized mechanism for collecting log messages generated throughout the system.
 * How it Works:
 * - Logging Messages: Various components within the system generate log messages during their operation.
 * - Buffering Log Messages: Log messages are buffered into the Logger buffer instead of directly written to an output file or stream.
 * - Deferred Writing: Log messages remain in the buffer until it's time to write them to the output medium, ensuring non-blocking behavior.
 * - Batch Processing: Log messages are processed in batches rather than individually to improve efficiency.
 * There are two instances of the Logger class in webserv:
 * 1. An error Logger, referenced for use in most classes.
 * 2. An access Logger, mainly intended for logging by the RequestHandler class.
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
Logger::Logger(const LoggerType type)
    : _type(type),
      _logFile((type == ERRORLOGGER) ? "./error.log" : "./access.log"),
      _logLevel(INFO),
      _bufferSize(99999),
      _logFileDescriptor(open(this->_logFile.c_str(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)),
      _enabled(true),
      _logLevelHelper()
{
    if (this->_logFileDescriptor == -1)
        throw LogFileOpenError(); // Opening log file failed
    if (type == ERRORLOGGER)
        this->errorLog(DEBUG, "Error Logger created");
    else // type == ACCESSLOGGER
        this->errorLog(DEBUG, "Access Logger created");
}

// Constructor for initializing Logger instance based on type (ERRORLOGGER/ACCESSLOGGER) and IConfiguration
Logger::Logger(const LoggerType type, const IConfiguration *configuration)
    : _type(type),
      _logFile((type == ERRORLOGGER) ? configuration->getErrorLogFile() : configuration->getAccessLogFile()),
      _logLevel((type == ERRORLOGGER) ? configuration->getLogLevel() : INFO),
      _bufferSize(configuration->getLogBufferSize()),
      _logFileDescriptor(open(this->_logFile.c_str(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)),
      _enabled((type == ERRORLOGGER) ? configuration->getErrorLogEnabled() : configuration->getAccessLogEnabled()),
      _logLevelHelper()
{
    if (this->_logFileDescriptor == -1)
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

// Destructor: Handles cleanup tasks like flushing buffer and closing log file descriptor
Logger::~Logger()
{
    this->errorLog(DEBUG, "Logger instance destruction started");
    this->writeLogBufferToFileBlocking();
    close(this->_logFileDescriptor);
}

// Getter method for log file descriptor
int Logger::getLogFileDescriptor() const
{
    return this->_logFileDescriptor;
}

// Method to flush buffer and write log messages to file
void Logger::writeLogBufferToFile()
{

    // Get the current buffer content
    std::string batch = this->_logBufferStream.str();

    // Return without writing to file if buffer is empty or below threshold
    if (batch.size() == 0)
    {
        this->errorLog(DEBUG, "writeLogBufferToFile(): Buffer is empty, nothing to write to log file");
        return;
    }
    else if (batch.size() < this->_bufferSize)
    {
        this->errorLog(DEBUG, "writeLogBufferToFile(): Threshold not reached, not writing to log file [current size: " + std::to_string(batch.size()) + " limit: " + std::to_string(this->_bufferSize) + "]");
        return;
    }

    // Clear the buffer stream
    this->_logBufferStream.str("");
    // Write to file
    ssize_t bytesWritten = write(this->_logFileDescriptor, batch.c_str(), batch.size());
    if (bytesWritten == -1)
    {
        this->errorLog(ERROR, "Error writing to log file"); // Writing to log file failed
        this->_logBufferStream << batch;                    // Append the entire batch to the batch stream for retry on the next run
    }
    else if (static_cast<std::size_t>(bytesWritten) < batch.size())
    {
        this->errorLog(DEBUG, "Only part of the batch was written to the log file. Might need to retry writing the rest of the batch. [bytesWritten: " + std::to_string(bytesWritten) + " out of: " + std::to_string(batch.size()) + "]");
        this->_logBufferStream << batch.substr(bytesWritten); // Append the failed part of the batch for retry on the next run
    }
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
    if (this->_enabled == false || logLevel < this->_logLevel)
        return;

    std::string logMessage = "timestamp=\"" + this->_getCurrentTimestamp() + "\" loglevel=\"" + this->_logLevelHelper.logLevelStringMap(logLevel) + "\" message=\"" + message + "\"\n";
    this->_logBufferStream << logMessage;
}

// Method to log access events
void Logger::accessLog(const IRequest &request, const Response &response)
{
    if (this->_enabled == false)
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

    // Convert the temporary stringstream object to a string and append it to the main log buffer stream
    this->_logBufferStream << logBufferStream.str();
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

// Method to flush buffer and write log messages to file (blocking)
// This method is used to write the remaining log messages to the log file when the server is shutting down
void Logger::writeLogBufferToFileBlocking()
{
    // Get the current buffer content
    std::string batch = this->_logBufferStream.str();

    // Clear the buffer stream
    this->_logBufferStream.str("");

    // Write to file, blocking until all data is written
    size_t bytesWritten = 0;
    while (bytesWritten < batch.size())
    {
        // Write the remaining bytes of the batch to the log file
        ssize_t result = write(this->_logFileDescriptor, batch.c_str() + bytesWritten, batch.size() - bytesWritten);
        if (result == -1)
            return;             // Writing to log file failed, return without writing the rest of the batch
        bytesWritten += result; // Increment bytesWritten by the number of bytes written in this iteration
    }
}

// Configuration method
void Logger::configure(const IConfiguration *configuration)
{
    this->_logFile = (this->_type == ERRORLOGGER) ? configuration->getErrorLogFile() : configuration->getAccessLogFile();
    this->_logLevel = configuration->getLogLevel();
    this->_bufferSize = configuration->getLogBufferSize();
    this->_logFileDescriptor = open(this->_logFile.c_str(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (this->_logFileDescriptor == -1)
        throw LogFileOpenError(); // Opening log file failed
    this->_enabled = (this->_type == ERRORLOGGER) ? configuration->getErrorLogEnabled() : configuration->getAccessLogEnabled();
}

// Path: includes/WebservExceptions.hpp
