#include "../includes/ExceptionHandler.hpp"

/*
 * ExceptionHandler Class:
 * Provides functionality to handle webserv exceptions thrown during execution.
 * Logs exception details using the webserv errorLogger instance and handles exceptions based on the specified log level.
 * Terminates the server if necessary (e.getLogLevel() == CRITICAL).
 *
 * Usage:
 * -through the Webserv ExceptionHandler instance
 *
 * Example:
 * try {
 *     // Some code that may throw exceptions
 *     throw Exception();
 * } catch (const WebservException& e) {
 *      exceptionHandler.handleException(e, "Main function");
 * }
 *
 * Note: 'CRITICAL' log level will cause webserv to terminate.
 * Note: To use Webserv custom exceptions, include the WebservExceptions.hpp file.
 */

// Constructor: Initializes ExceptionHandler with an errorLogger instance, _server linking is deferred
ExceptionHandler::ExceptionHandler(Logger &errorLogger)
    : _errorLogger(errorLogger),
      _server(nullptr)
{
    // Log debug message indicating the creation of an ExceptionHandler instance.
    this->_errorLogger.errorLog(DEBUG, "ExceptionHandler instance created.");
}

// Destructor: No dynamic memory management, so a default destructor is sufficient.
ExceptionHandler::~ExceptionHandler()
{
    // Log debug message indicating the destruction of an ExceptionHandler instance.
    this->_errorLogger.errorLog(DEBUG, "ExceptionHandler instance destroyed.");
}

// handleException method: Logs exception details and handles critical exceptions.
int ExceptionHandler::handleException(const WebservException &e, const std::string &context) const
{
    // Log the exception details, including context and error message.
    this->_errorLogger.errorLog(e.getLogLevel(), "[EXCEPTION] " + context + (context.empty() ? ": " : " : ") + e.what());

    // Check if the exception's log level is critical.
    if (e.getLogLevel() == CRITICAL)
    {
        // If critical, log termination message and terminate the server.
        this->_errorLogger.errorLog(CRITICAL, "webserv will now terminate.");
        this->_server->terminate(e.getErrorCode());
    }
    return e.getErrorCode();
}

// linkServer method: Sets the server instance to be referenced for termination.
void ExceptionHandler::linkServer(Server *server)
{
    this->_server = server;
}

// Path: srcs/ExceptionHandler.cpp