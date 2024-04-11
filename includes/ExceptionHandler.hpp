#ifndef EXCEPTIONHANDLER_HPP
#define EXCEPTIONHANDLER_HPP

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
 *      exceptionHandler.handleException(e, "Main function"); // returns the error code of the exception
 * }
 *
 * Note: 'CRITICAL' log level will cause webserv to terminate.
 * Note: To use Webserv custom exceptions, include the WebservExceptions.hpp file.
 */

#include <string>
#include "constants/HttpStatusCodeHelper.hpp" // Include necessary dependencies
#include "Logger.hpp"                         // Include necessary dependencies
#include "WebservExceptions.hpp"              // Include necessary dependencies
#include "Server.hpp"                         // Include necessary dependencies

class ExceptionHandler
{
private:
    Logger &_errorLogger;                             // Reference to the errorLogger instance
    Server *_server;                                  // Pointer to the server instance
    const HttpStatusCodeHelper _httpStatusCodeHelper; // Helper instance for HTTP status codes

    // _makeHtmlPage method: Generates an HTML page with the specified HTTP status code.
    static std::string _makeHtmlPage(HttpStatusCode statusCode);

public:
    // Constructor: Initializes ExceptionHandler with an errorLogger instance.
    ExceptionHandler(Logger &errorLogger);

    // Destructor: No dynamic memory management, so a default destructor is sufficient.
    ~ExceptionHandler();

    // linkServer method: Sets the server instance to be referenced for termination.
    void linkServer(Server *_server);

    // handleException method: Logs exception details and handles critical exceptions.
    int handleException(const WebservException &e, const std::string &context="") const;
};

#endif // EXCEPTIONHANDLER_HPP
       // Path: includes/ExceptionHandler.hpp
