#include "../includes/RequestHandler.hpp"

/*
 * RequestHandler class
 *
 * Responsible for handling client requests:
 * - Reads the raw request from the client
 * - Parses it into a Request object
 * - Passes the Request object to the Router
 *
 * The response is either served statically or dynamically, depending on the route:
 * - Served statically: Response sent immediately to buffer
 * - Served dynamically: Response obtained from a separate process
 *   (read end of the pipe is returned)
 */

// Constructor
RequestHandler::RequestHandler(const ISocket &socket, IBufferManager &bufferManager, const IConfiguration &configuration, IRouter &router, ILogger &logger, const IExceptionHandler &exceptionHandler)
    : _bufferManager(bufferManager),
      _router(router),
      _logger(logger),
      _exceptionHandler(exceptionHandler),
      _clientHandler(&ClientHandler(socket, logger)),
      _requestParser(configuration, logger),
      _httpHelper(configuration),
      _request(_httpHelper, configuration),
      _response(_httpHelper)
{
    // Log the creation of the RequestHandler instance.
    this->_logger.log(DEBUG, "RequestHandler instance created.");
}

// Destructor
RequestHandler::~RequestHandler()
{
    delete this->_clientHandler;
    // Log the destruction of the RequestHandler instance.
    this->_logger.log(DEBUG, "RequestHandler instance destroyed.");
}

// Handles a client request
// Return the pipe descriptor for dynamic content or 0 for static content and invalid requests
int RequestHandler::handleRequest(int socketDescriptor)
{
    // Give the 'ClientHandler' the current socket descriptor
    this->_clientHandler->setSocketDescriptor(socketDescriptor);

    try
    {
        // Read the raw request from the client
        std::vector<char> rawRequest = this->_clientHandler->readRequest();

        // Clear the request instance
        this->_request.clear();

        // Clear the response instance
        this->_response.clear();

        // Parse the raw request into a Request object
        this->_requestParser.parseRequest(rawRequest, this->_request);
    }

    catch (const WebservException &e)
    {
        // Get the status code
        int statusCode;
        if (dynamic_cast<const HttpStatusCodeException *>(&e))
            statusCode = e.getErrorCode(); // An HttpStatusCodeException was thrown
        else
            statusCode = 500; // Internal Server Error; Default status code for other exceptions

        // Log the exception
        this->_exceptionHandler.handleException(e, "RequestHandler::processRequest socket=\"" + std::to_string(socketDescriptor) + "\"");

        // Handle error response
        return this->handleErrorResponse(socketDescriptor, statusCode); // and return 0
    }

    // 'Router' selects the right 'ResponseGenerator' for the job
    int pipeDescriptor = this->_router.execRoute(&this->_request, &this->_response);

    // If a pipe was created, return it
    if (pipeDescriptor != 0)
    {
        this->_pipeRoutes[pipeDescriptor] = socketDescriptor;
        return pipeDescriptor; // cgi content
    }
    else // static content
    {
        // Push the response to the buffer
        return this->_sendResponse(socketDescriptor); // and return 0
    }
}

// Handles exceptions related to pipe events - returns the client socket descriptor destination for the response
int RequestHandler::handlePipeException(int pipeDescriptor)
{
    // Get the client socket descriptor linked to the pipe
    int clientSocket = this->_pipeRoutes[pipeDescriptor];

    // Remove the pipe descriptor from the map
    this->_pipeRoutes.erase(pipeDescriptor);

    // Handle error response
    this->handleErrorResponse(clientSocket, INTERNAL_SERVER_ERROR);

    // Return the client socket descriptor
    return clientSocket;
}

// Handles read input from pipe
int RequestHandler::handlePipeRead(int pipeDescriptor)
{
    // Get the client socket descriptor linked to the pipe
    int clientSocket = this->_pipeRoutes[pipeDescriptor];

    // Remove the pipe descriptor from the map
    this->_pipeRoutes.erase(pipeDescriptor);

    // Clear the response instance
    this->_response.clear();

    // Give the ClientHandler the current socket descriptor
    this->_clientHandler->setSocketDescriptor(clientSocket);

    // Read the response from the pipe
    std::vector<char> response = this->_clientHandler->readRequest();

    // Set the response
    this->_response.setResponse(response);

    // Push the response to the buffer
    this->_sendResponse(clientSocket);

    // Return the client socket descriptor
    return clientSocket;
}

// Sends the response to the buffer
int RequestHandler::_sendResponse(int socketDescriptor)
{
    // Serialise the response
    std::vector<char> serialisedResponse = this->_response.serialise();

    // Push the response to the buffer
    this->_bufferManager.pushSocketBuffer(socketDescriptor, serialisedResponse);

    // create an access log entry
    this->_logger.log(this->_request, this->_response);

    // return 0
    return (0);
}

// Handles error responses
int RequestHandler::handleErrorResponse(int socketDescriptor, int statusCode)
{
    // Clear the response instance
    this->_response.clear();

    // Set the response to the error status code
    this->_response.setErrorResponse(statusCode);

    // Push the response to the buffer
    return this->_sendResponse(socketDescriptor); // and return 0
}

// Handle error responses - HttpStatusCode input
int RequestHandler::handleErrorResponse(int socketDescriptor, HttpStatusCode statusCode)
{
    // Clear the response instance
    this->_response.clear();

    // Set the response to the error status code
    this->_response.setErrorResponse(statusCode);

    // Push the response to the buffer
    return this->_sendResponse(socketDescriptor); // and return 0
}

// path: srcs/RequestHandler.cpp
