#include "../includes/RequestHandler.hpp"

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

RequestHandler::RequestHandler(const ISocket &socket, IBufferManager &bufferManager, const IConfiguration &configuration, IRouter &router, ILogger &errorLogger, ILogger &accessLogger, const IExceptionHandler &exceptionHandler)
    : _bufferManager(bufferManager),
      _router(router),
      _errorLogger(errorLogger),
      _accessLogger(accessLogger),
      _exceptionHandler(exceptionHandler),
      _clientHandler(&ClientHandler(socket, errorLogger)),
      _requestParser(configuration, errorLogger),
      _requestHelper(configuration),
      _request(Request(_requestHelper, configuration))
{
    // Log the creation of the RequestHandler instance.
    this->_errorLogger.errorLog(DEBUG, "RequestHandler instance created.");
}

/*
 * RequestHandler Destructor:
 * Default destructor for the RequestHandler class.
 * Logs the destruction of a RequestHandler instance.
 */
RequestHandler::~RequestHandler()
{
    delete this->_clientHandler;
    // Log the destruction of the RequestHandler instance.
    this->_errorLogger.errorLog(DEBUG, "RequestHandler instance destroyed.");
}

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
        // 'RequestParser' produces a 'IRequest' from the raw request string
        this->_requestParser.parseRequest(rawRequest, this->_request);
    }
    catch (const HttpStatusCodeException &e)
    {
        // Log the HTTP status code exception and continue processing the request
        this->_exceptionHandler.handleException(e, "RequestHandler::processRequest socket=\"" + std::to_string(socketDescriptor) + "\"");
    }
    catch (const WebservException &e)
    {
        // Log the exception and abort processing the request
        this->_exceptionHandler.handleException(e, "RequestHandler::processRequest socket=\"" + std::to_string(socketDescriptor) + "\"");
        // return -1 to indicate the aborting of the request
        return -1;
    }

    // 'Router' selects the right 'ResponseGenerator' for the job
    this->_router.execRoute(&this->_request, &this->_response);

    // Create the raw response string
    // std::vector<char> rawResponse = this->_response.serialise(this->_response);

    // temporary empty response
    std::vector<char> rawResponse;
    // Push the raw response to the buffer manager
    this->_bufferManager.pushSocketBuffer(socketDescriptor, rawResponse);

    // create an access log entry
    this->_accessLogger.accessLog(this->_request, this->_response);
    return (0);
}
