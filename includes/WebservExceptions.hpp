#ifndef WEBSERVEXCEPTIONS_HPP
#define WEBSERVEXCEPTIONS_HPP

/*
 * Webserv Error Handling System
 *
 * Error handling, logging, and if necessary gracefully terminating is taken care of
 * by the Webserv ExceptionHandler instance in combination with custom error classes.
 *
 * This file contains the definitions of custom error classes used by webserv.
 * All custom error classes are inherited from the base class WebservException, which is a subclass of std::exception.
 *
 * How to Add New Errors:
 * - To add a new error, create a new class inherited from WebservException.
 * - Define the error message, log level, and error code in the constructor of the new error class.
 * - Optionally, you can add additional methods or members specific to the new error type.
 *
 * Usage:
 * - Throw custom errors when exceptional conditions occur during the execution of the server.
 * - Catch custom errors and pass them to the ExceptionHandler for logging and handling.
 * - NOTE: Setting the log level to CRITICAL will cause the ExceptionHandler to terminate Webserv.
 *
 * Example:
 * // Define a new custom error class
 * class NoConfigFileError : public WebservException {
 * public:
 *     NoConfigFileError()
 *         : WebservException(CRITICAL, "No configuration file found.", 1) {}
 * };
 *
 * // Throw and catch a custom error
 * try {
 *     throw NoConfigFileError();
 * } catch (const WebservException& e) {
 *     exceptionHandler.handleException(e, "Main function");
 * }
 */

#include <exception>
#include <string>
#include <sstream>
#include "constants/LogLevelHelper.hpp"

// Base class for all custom exceptions in Webserv
class WebservException : public std::exception
{
private:
    LogLevel _logLevel;
    const std::string _message;
    int _errorCode;

public:
    // Constructor
    WebservException(LogLevel logLevel, const std::string &message, int errorCode)
        : _logLevel(logLevel),
          _message(message),
          _errorCode(errorCode){};

    // Override what() method
    const char *what() const throw()
    {
        return this->_message.c_str();
    }

    // Getter methods
    LogLevel getLogLevel() const { return this->_logLevel; }
    int getErrorCode() const { return this->_errorCode; }
};

// Derived error classes
class NoConfigFileError : public WebservException
{
public:
    NoConfigFileError()
        : WebservException(CRITICAL, "No configuration file found.", 1){};
};

class InvalidConfigFileError : public WebservException
{
public:
    InvalidConfigFileError()
        : WebservException(CRITICAL, "Invalid configuration file.", 1){};
};

class MaximumConnectionsReachedError : public WebservException
{
public:
    MaximumConnectionsReachedError()
        : WebservException(WARN, "Maximum Connections reached.", 1){};
};

class ConnectionEstablishingError : public WebservException
{
public:
    ConnectionEstablishingError()
        : WebservException(CRITICAL, "Failed to accept Connection.", 1){};
};

class PollError : public WebservException
{
public:
    PollError()
        : WebservException(CRITICAL, "Failed to poll events.", 1){};
};

class SocketCreateError : public WebservException
{
public:
    SocketCreateError()
        : WebservException(CRITICAL, "Failed to create socket.", 1){};
};

class SocketSetError : public WebservException
{
public:
    SocketSetError()
        : WebservException(CRITICAL, "Failed to set socket to non-blocking.", 1){};
};

class SocketListenError : public WebservException
{
public:
    SocketListenError()
        : WebservException(CRITICAL, "Failed to listen on socket.", 1){};
};

class SocketBindError : public WebservException
{
public:
    SocketBindError()
        : WebservException(CRITICAL, "Failed to bind socket.", 1){};
};

class ServerSocketError : public WebservException
{
public:
    ServerSocketError()
        : WebservException(CRITICAL, "Error on server socket.", 1){};
};

class ServerSocketClosedError : public WebservException
{
public:
    ServerSocketClosedError()
        : WebservException(CRITICAL, "Server socket closed.", 1){};
};

class ServerSocketInvalidError : public WebservException
{
public:
    ServerSocketInvalidError()
        : WebservException(CRITICAL, "Invalid server socket.", 1){};
};

class LogFileOpenError : public WebservException
{
public:
    LogFileOpenError()
        : WebservException(CRITICAL, "Failed to open log file.", 1){};
};

class LogFileWriteError : public WebservException
{
public:
    LogFileWriteError()
        : WebservException(ERROR, "Failed to write to log file.", 1){};
};

class InvalidHeaderError : public WebservException
{
public:
    InvalidHeaderError()
        : WebservException(ERROR, "Invalid headers.", 1){};
};

class InvalidMethodError : public WebservException
{
public:
    InvalidMethodError()
        : WebservException(ERROR, "Invalid method.", 1){};
};

class InvalidHttpVersionError : public WebservException
{
public:
    InvalidHttpVersionError()
        : WebservException(ERROR, "Invalid HTTP version.", 1){};
};

class UnknownMethodError : public WebservException
{
public:
    UnknownMethodError()
        : WebservException(ERROR, "Unknown method.", 1){};
    UnknownMethodError(const std::string &method)
        : WebservException(ERROR, "Unknown method: \"" + method + "\"", 1){};
};

class UnknownHttpVersionError : public WebservException
{
public:
    UnknownHttpVersionError()
        : WebservException(ERROR, "Unknown HTTP version.", 1){};
    UnknownHttpVersionError(const std::string &version)
        : WebservException(ERROR, "Unknown HTTP version: \"" + version + "\"", 1){};
};

class UnknownHeaderError : public WebservException
{
public:
    UnknownHeaderError()
        : WebservException(ERROR, "Unknown header.", 1){};
    UnknownHeaderError(const std::string &header)
        : WebservException(ERROR, "Unknown header: \"" + header + "\"", 1){};
};

class UnknownLogLevelError : public WebservException
{
public:
    UnknownLogLevelError()
        : WebservException(ERROR, "Unknown log level.", 1){};
    UnknownLogLevelError(const std::string &logLevel)
        : WebservException(ERROR, "Unknown log level: \"" + logLevel + "\"", 1){};
};

class UnknownHttpStatusCodeError : public WebservException
{
public:
    UnknownHttpStatusCodeError()
        : WebservException(ERROR, "Unknown HTTP status code.", 1){};
    UnknownHttpStatusCodeError(const std::string &statusCode)
        : WebservException(ERROR, "Unknown HTTP status code: \"" + statusCode + "\"", 1){};
};

class HttpStatusCodeException : public WebservException
{
public:
    HttpStatusCodeException(int statusCode)
        : WebservException(INFO, ("Http Status Code: \"" + (std::ostringstream() << statusCode).str() + "\""), statusCode) {};
    HttpStatusCodeException(int statusCode, const std::string &message)
        : WebservException(INFO, ("Http Status Code " + (std::ostringstream() << statusCode).str() + ": " + message), statusCode) {}
};

#endif // WEBSERVEXCEPTIONS_HPP
       // Path: includes/WebservExceptions.hpp
