#ifndef SESSIONS_HPP
#define SESSIONS_HPP

/*
 * The Sessions class serves as the orchestrator for handling client requests within webserv.
 *
 * It coordinates the processing of client requests, managing the client handler, request parser, router, and request handler objects.
 *
 * When a request is registered, Sessions initializes a ClientHandler with the relevant
 * socket descriptor, reads and processes the request, and sends a response back to the client.
 *
 * In case the entire response cannot be sent at once, Sessions handles partial sends by buffering remaining
 * bytes associated with the client's socket descriptor and updating the socket's event status to include POLLOUT
 * for subsequent transmission.
 *
 * In scenarios where exceptions occur, Sessions handles various types of exceptions,
 * logs relevant information, and sends appropriate HTTP responses to the client.
 *
 * Throughout its operation, Sessions ensures smooth communication between clients and the server while managing
 * exceptions effectively.
 */

#include <string>
#include <map>
#include <cstring>
#include <vector>
#include "constants/PollfdQueueFixedPositions.hpp"
#include "Server.hpp"
#include "ClientHandler.hpp"
#include "RequestParser.hpp"
#include "Router.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Logger.hpp"
#include "ExceptionHandler.hpp"
#include "ARequestHandler.hpp"

class Sessions
{
private:
    // Private member variables

    std::map<int, std::vector<char>> _responseBuffer; // Buffer for storing incompletely sent responses
    PollfdQueue &_pollFds;                            // Reference to a queue of poll file descriptors
    ClientHandler _clientHandler;                     // Handles communication with clients
    const RequestParser _requestParser;               // Parses incoming requests
    const Router _router;                             // Routes requests to appropriate handlers
    ARequestHandler *_requestHandler;                 // Pointer to the recruited request handler
    Request _request;                                 // Represents an HTTP request
    Response _response;                               // Represents an HTTP response
    Logger &_errorLogger;                             // Reference to the error logger
    Logger &_accessLogger;                            // Reference to the access logger
    const ExceptionHandler &_exceptionHandler;        // Reference to the exception handler
    short _pollExceptionMask;                         // Mask for poll exceptions

    // Private member functions

    // Processes an incoming request
    void processRequest(size_t &i);

    // Processes buffered responses
    void processBufferedResponse(size_t &i);

    // Handles exceptions
    void handleException(size_t &i);

public:
    // Constructor
    Sessions(const Configuration &Configuration, Logger &errorLogger, Logger &accessLogger, const ExceptionHandler &ExceptionHandler, Server &server);

    // Copy assignment operator
    const Sessions &operator=(const Sessions &src);

    // Destructor
    ~Sessions();

    // Processes events from the poll mechanism
    void processEvents();
};

#endif // SESSIONS_HPP
// Path: srcs/Sessions.cpp
