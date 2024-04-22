#include "../includes/Server.hpp"
#include "../includes/ExceptionHandler.hpp"

/*
 * ExceptionHandler Class:
 * Provides functionality to handle webserv exceptions thrown during execution.
 * Logs exception details using the webserv logger instance.
 * When it encounters a webserv spcific exception (WebserveException),
 * it will handle them  based on the specified log level, and terminates the server if necessary (e.getLogLevel() == CRITICAL).
 *
 * Usage:
 * -through the Webserv ExceptionHandler instance
 *
 * Example:
 * try {
 *     // Some code that may throw exceptions
 *     throw Exception();
 * } catch (const WebservException& e) {
 *      exceptionHandler.handleException(e, "Main function"); // returns the error code of the exception
 * }
 *
 * Note: 'CRITICAL' log level will cause webserv to terminate (in case of a WebservException).
 * Note: To use Webserv custom exceptions, include the WebservExceptions.hpp file.
 */

// Constructor: Initializes ExceptionHandler with an logger instance, _server linking is deferred
ExceptionHandler::ExceptionHandler(ILogger &logger)
    : _logger(logger),
      _server(nullptr)
{
    // Log debug message indicating the creation of an ExceptionHandler instance.
    this->_logger.log(DEBUG, "ExceptionHandler instance created.");
}

// Constructor: Initializes ExceptionHandler with an logger instance and a pointer to the server instance.
ExceptionHandler::ExceptionHandler(ILogger &logger, Server *server)
    : _logger(logger),
      _server(server)
{
    // Log debug message indicating the creation of an ExceptionHandler instance.
    this->_logger.log(DEBUG, "ExceptionHandler instance created.");
}

// Destructor: No dynamic memory management, so a default destructor is sufficient.
ExceptionHandler::~ExceptionHandler()
{
    // Log debug message indicating the destruction of an ExceptionHandler instance.
    this->_logger.log(DEBUG, "ExceptionHandler instance destroyed.");
}

// _handleWebservException method: Logs exception details and handles critical exceptions.
int ExceptionHandler::_handleWebservException(const WebservException &e, const std::string &context) const
{
    // Log the exception details, including context and error message.
    this->_logger.log(e.getLogLevel(), "[EXCEPTION] " + context + (context.empty() ? ": " : " : ") + e.what());

    // Check if the exception's log level is critical.
    if (e.getLogLevel() == CRITICAL)
    {
        // If critical, log termination message and terminate the server.
        this->_logger.log(CRITICAL, "webserv will now terminate.");
        if (this->_server)
            this->_server->terminate(e.getErrorCode());
    }
    return e.getErrorCode();
}

// _handleStandardException method: Logs standard exception details
int ExceptionHandler::_handleStandardException(const std::exception &e, const std::string &context) const
{
    // Log the exception details, including context and error message.
    this->_logger.log(UNKNOWN, "[EXCEPTION] " + context + (context.empty() ? ": " : " : ") + e.what());
    return 0;
}

// handleException method: Verifies the type of exception and calls the appropriate handler.
int ExceptionHandler::handleException(const std::exception &e, const std::string &context) const
{
    if (const WebservException *wse = dynamic_cast<const WebservException *>(&e))
    {
        return this->_handleWebservException(*wse, context);
    }
    else
    {
        return this->_handleStandardException(e, context);
    }
}

// Path: srcs/ExceptionHandler.cpp