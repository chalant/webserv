#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <ctime>
#include <sstream>
#include <poll.h>
#include <fcntl.h>    // for open(), O_WRONLY, O_CREAT, O_APPEND
#include <sys/stat.h> // for S_IRUSR, S_IWUSR, S_IRGRP, S_IROTH
#include <unistd.h>   // for close()
#include <iomanip>    // for std::put_time
#include "constants/LogLevel.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Configuration.hpp"

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

enum LoggerType
{
    ERRORLOGGER,
    ACCESSLOGGER
};

class Logger
{
private:
    LoggerType _type;
    std::string _logFile;
    std::ostringstream _logBufferStream;
    LogLevel _logLevel;
    size_t _bufferSize;
    pollfd *_logFilePollFd;
    int _logFileDescriptor;
    bool _enabled;

    // Private methods
    const std::string getCurrentTimestamp();                                                                                                // Method to get the current timestamp
    void appendMapToLog(std::ostringstream &ss, const std::string &fieldName, const std::unordered_map<std::string, std::string> &dataMap); // Method to append a map to the log message

public:
    // Constructors and Destructor
    Logger();                                              // Default constructor
    Logger(LoggerType type, Configuration &configuration); // Constructor with type and configuration
    Logger &operator=(const Logger &other);                // Copy assignment operator
    ~Logger();                                             // Destructor

    // Setter methods
    void setLogLevel(LogLevel logLevel);              // Setter method for log level
    void setLogFile(const std::string &logFile);      // Setter method for log file
    void setLogFileDescriptor(int logFileDescriptor); // Setter method for log file descriptor
    void setLogFilePollFd(pollfd *logFilePollFd);     // Setter method for poll file descriptors pointer

    // Getter methods
    int getLogFileDescriptor() const; // Getter method for log file descriptor

    // Logging methods
    void errorLog(LogLevel logLevel, const std::string &message);     // Method to log error messages
    void accessLog(const Request &request, const Response &response); // Method to log access events

    // Buffer methods
    void writeBufferToFile(); // Method to write the log buffer to the log file
};

#endif // LOGGER_HPP
       // Path: includes/Logger.hpp
