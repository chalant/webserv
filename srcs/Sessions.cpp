#include "../includes/Sessions.hpp"

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

Sessions::Sessions(Configuration &configuration, Logger &errorLogger, Logger &accessLogger, ExceptionHandler &exceptionHandler, Server &server)
    : _pollFds(server.getPollfdQueue()), _errorLogger(errorLogger), _accessLogger(accessLogger), _exceptionHandler(exceptionHandler), _pollExceptionMask(POLLHUP | POLLERR | POLLNVAL), _clientHandler(errorLogger, exceptionHandler), _requestParser(configuration, errorLogger, exceptionHandler), _router(configuration, errorLogger, exceptionHandler), _request(RequestHelper(), Configuration(nullptr))
{
    // Log the creation of the Sessions instance.
    errorLogger.errorLog(DEBUG, "Sessions instance created.");
}

/*
 * Sessions Destructor:
 * Default destructor for the Sessions class.
 * Logs the destruction of a Sessions instance.
 */
Sessions::~Sessions()
{
    // Log the destruction of the Sessions instance.
    this->_errorLogger.errorLog(DEBUG, "Sessions instance destroyed.");
}

/*
 * processEvents method:
 * Processes events related to client requests in the poll fd array.
 */
void Sessions::processEvents()
{
    this->_errorLogger.errorLog(DEBUG, "Processing events.");

    // Process events
    for (size_t i = FIRST_CLIENT_POLL_FD; i < this->_pollFds.size(); i++)
    {
        short revents = this->_pollFds[i].revents;

        // If there are no events, continue
        if (revents == 0)
            continue;

        // Check for exceptions
        if (revents & this->_pollExceptionMask)
        {
            this->handleException(i);
        }
        // Read and process a new request
        else if (revents & POLLIN)
        {
            this->processRequest(i);
        }
        // Write buffered response
        else if (revents & POLLOUT)
        {
            this->processBufferedResponse(i);
        }
    }
}

void Sessions::processRequest(size_t &i)
{
    // Give the 'ClientHandler' the current socket descriptor
    int fd = this->_pollFds[i].fd;
    this->_clientHandler.setSocketDescriptor(fd);

    try
    {
        // Read the raw request from the client
        std::vector<char> rawRequest = this->_clientHandler.readRequest();
        // 'RequestParser' produces a 'Request' from the raw request string
        this->_request = this->_requestParser.parseRequest(rawRequest);
    }
    catch (const WebservException &e)
    {
        // Log the exceptions
        this->_exceptionHandler.handleException(e, "Sessions::processRequest socket=\"" + std::to_string(fd) + "\"");
        // Remove the socket from the poll set
        this->_pollFds.erase(i);
        // Decrement i to compensate for the removal
        i--;
        return;
    }

    // 'Router' selects the right 'RequestHandler' for the job
    this->_requestHandler = this->_router.route(this->_request);

    // 'ARequesthandler' produces the 'Response'
    this->_response = this->_requestHandler->handleRequest(this->_request);
    delete this->_requestHandler;

    // Create the raw response string
    std::vector<char> rawResponse = this->_requestParser.rawResponse(this->_response);

    // 'ClientHandler' sends out a response to the client
    size_t sentBytes = this->_clientHandler.sendResponse(rawResponse);

    // Verify if the entire response was sent
    if (sentBytes < rawResponse.size())
    {
        size_t unsentBytes = rawResponse.size() - sentBytes;
        std::vector<char> buffer(unsentBytes);
        memcpy(&buffer[0], &rawResponse[sentBytes], unsentBytes);
        // Save the remaining bytes to the buffer
        this->_responseBuffer[fd] = buffer;
        // Update the events for the socket to include POLLOUT
        this->_pollFds.pollout(i);
    }
    else // The entire response was sent or an error occurred
    {
        // Remove the socket from the poll set
        this->_pollFds.erase(i);
        // Decrement i to compensate for the removal
        i--;
    }
}

void Sessions::processBufferedResponse(size_t &i)
{
    // Initialize the 'ClientHandler' with the socket descriptor
    int fd = this->_pollFds[i].fd;
    this->_clientHandler.setSocketDescriptor(fd);

    // Get the remaining bytes to send
    std::vector<char> &buffer = this->_responseBuffer[fd];

    // Send the remaining bytes
    size_t sentBytes = this->_clientHandler.sendResponse(buffer);

    // Verify if the entire response was sent
    if (sentBytes < buffer.size())
    {
        size_t unsentBytes = buffer.size() - sentBytes;
        std::vector<char> newBuffer(unsentBytes);
        memcpy(&newBuffer[0], &buffer[sentBytes], unsentBytes);
        // Save the remaining bytes to the buffer
        this->_responseBuffer[fd] = newBuffer;
    }
    else
    {
        // Remove the socket from the poll set
        this->_pollFds.erase(i);
        // Remove the socket from the response buffer map
        this->_responseBuffer.erase(fd);
        // Decrement i to compensate for the removal
        i--;
    }
}

void Sessions::handleException(size_t &i)
{
    short revents = this->_pollFds[i].revents;
    int fd = this->_pollFds[i].fd;

    if (revents & POLLHUP)
    {
        // Log the disconnection
        this->_errorLogger.errorLog(INFO, "Client disconnected socket: " + std::to_string(fd));
    }

    if (revents & POLLNVAL)
    {
        // Log the error
        this->_errorLogger.errorLog(ERROR, "Invalid request on socket: " + std::to_string(fd));

        // Send a 400 response to the client
        if (this->_clientHandler.sendResponse("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n") == -1)
        {
            // Log the error
            this->_errorLogger.errorLog(ERROR, "Error sending 400 response to socket: " + std::to_string(fd));
        }
    }

    // Check for errors on the socket
    if (revents & POLLERR)
    {
        // Log the error
        this->_errorLogger.errorLog(ERROR, "Error on socket: " + std::to_string(fd));

        // Send a 500 response to the client
        if (this->_clientHandler.sendResponse("HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n") == -1)
        {
            // Log the error
            this->_errorLogger.errorLog(ERROR, "Error sending 500 response to socket: " + std::to_string(fd));
        }
    }
    // Remove the socket from the response buffer map
    this->_responseBuffer.erase(fd);
    // Close and Remove the socket from the poll set
    this->_pollFds.erase(i);
    // Decrement i to compensate for the removal
    i--;
}
