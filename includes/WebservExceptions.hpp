#ifndef WEBSERVEXCEPTIONS_HPP
#define WEBSERVEXCEPTIONS_HPP

#include <exception>
#include <string>
#include "constants/LogLevel.hpp"

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

// Base class for all custom exceptions in Webserv
class WebservException : public std::exception
{
private:
    LogLevel _logLevel;
    std::string _message;
    size_t _errorCode;

public:
    // Constructor
    WebservException(LogLevel logLevel, const std::string& message, size_t errorCode)
        : _logLevel(logLevel), _message(message), _errorCode(errorCode){};

    // Override what() method
    const char *what() const throw()
    {
        return this->_message.c_str();
    }

    // Getter methods
    LogLevel getLogLevel() const { return this->_logLevel; }
    size_t getErrorCode() const { return this->_errorCode; }
};

// Derived error classes
class LogFileOpenError : public WebservException
{
public:
    LogFileOpenError()
    : WebservException(CRITICAL, "Failed to open log file.", 1) {};
};

class LogFileWriteError : public WebservException
{
public:
    LogFileWriteError()
    : WebservException(ERROR, "Failed to write to log file.", 1) {};
};

#endif // WEBSERVEXCEPTIONS_HPP
       // Path: includes/WebservExceptions.hpp