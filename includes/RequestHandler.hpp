#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

/*
 * The RequestHandler class serves as the orchestrator for handling client requests within webserv.
 *
 * It coordinates the processing of client requests, managing the client handler, request parser, router, and request handler objects.
 *
 * When a request is registered, RequestHandler initializes a ClientHandler with the relevant
 * socket descriptor, reads and processes the request, and sends a response back to the client.
 *
 * In case the entire response cannot be sent at once, RequestHandler handles partial sends by buffering remaining
 * bytes associated with the client's socket descriptor and updating the socket's event status to include POLLOUT
 * for subsequent transmission.
 *
 * In scenarios where exceptions occur, RequestHandler handles various types of exceptions,
 * logs relevant information, and sends appropriate HTTP responses to the client.
 *
 * Throughout its operation, RequestHandler ensures smooth communication between clients and the server while managing
 * exceptions effectively.
 */

#include <IRequestHandler.hpp>
#include <string>
#include <map>
#include <cstring>
#include <vector>
#include "ClientHandler.hpp"
#include "RequestParser.hpp"
#include "constants/HttpHelper.hpp"
#include "IRouter.hpp"
#include "IRequest.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "ILogger.hpp"
#include "IExceptionHandler.hpp"
#include "IResponseGenerator.hpp"
#include "PollfdManager.hpp"
#include "IBufferManager.hpp"
#include "ISessionManager.hpp"
#include "IConfiguration.hpp"
#include "Configuration.hpp"

class RequestHandler : public IRequestHandler
{
private:
    // Private member variables
    IBufferManager &_bufferManager;     // Reference to the buffer manager
    ISessionManager &_sessionManager;   // Reference to the session manager
    IClientHandler *_clientHandler;     // Handles communication with clients
    const RequestParser _requestParser; // Parses incoming requests
    IRouter &_router;                   // Routes requests to appropriate handlers
    // AResponseGenerator *_requestHandler;                 // Pointer to the recruited request handler
    HttpHelper _httpHelper;                     // Helper class for request-related operations
    ILogger &_logger;                           // Reference to the logger
    const IExceptionHandler &_exceptionHandler; // Reference to the exception handler
    std::map<int, int> _pipeRoutes;             // Map of pipe descriptors to socket descriptors

    // private method
    int _sendResponse(int socketDescriptor);

public:
    // Constructor
    RequestHandler(const ISocket &socket, IBufferManager &bufferManager, ISessionManager &sessionManager, const IConfiguration &configuration, IRouter &router, ILogger &logger, const IExceptionHandler &IExceptionHandler);

    // Destructor
    ~RequestHandler();

    // Handles client requests
    int handleRequest(int socketDescriptor);

    // Handles exceptions related to pipe events
    int handlePipeException(int pipeDescriptor);

    // Handles read input from pipe
    int handlePipeRead(int pipeDescriptor);

    // Handles error responses
    int handleErrorResponse(int socketDescriptor, int statusCode);
    int handleErrorResponse(int socketDescriptor, HttpStatusCode statusCode);
};

#endif // SESSIONS_HPP
// Path: srcs/RequestHandler.cpp
