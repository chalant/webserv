#include "../includes/EventManager.hpp"

EventManager::EventManager(IPollfdManager *pollfdManager, IBufferManager *bufferManager, ISocket *socket, IServer *server, IRequestHandler *requestHandler, ILogger *errorLogger)
    : _pollfdManager(pollfdManager),
      _bufferManager(bufferManager),
      _socket(socket),
      _server(server),
      _requestHandler(requestHandler),
      _errorLogger(errorLogger) {}

EventManager::~EventManager() {}

void EventManager::handleEvents()
{
    this->_handleServerSocketEvents();
    this->_handleClientSocketEvents();
    this->_handleFileDescriptorEvents();
}

void EventManager::_handleServerSocketEvents()
{
    // Get Server Socket events
    int serverSocketEvents = this->_pollfdManager->getServersocketEvents();

    // If server socket has no events, return immediately
    if (serverSocketEvents == 0)
        return;

    // Check for errors on server socket
    if (serverSocketEvents & POLLERR)
        throw ServerSocketError();

    // Check for server socket closed
    if (serverSocketEvents & POLLHUP)
        throw ServerSocketClosedError();

    // Check for invalid request on server socket
    if (serverSocketEvents & POLLNVAL)
        throw ServerSocketInvalidError();

    // Check if server socket is ready to accept incoming connections
    if (serverSocketEvents & POLLIN)
    {
        // Accept incoming connection
        this->_server->acceptConnection();
    }
    else
    {
        // Log no incoming connection
        this->_errorLogger->errorLog(VERBOSE, "No incoming connection.");
    }
}

void EventManager::_handleClientSocketEvents()
{
    this->_errorLogger->errorLog(DEBUG, "Processing events.");

    // Process events
    for (size_t i = this->_pollfdManager->getClientsIndex(); i < this->_pollfdManager->getPollfdQueueSize(); i++)
    {
        short revents = this->_pollfdManager->getEvents(i);

        // If there are no events, continue
        if (revents == 0)
            continue;

        // Check for exceptions
        short pollExceptionMask = POLLHUP | POLLERR | POLLNVAL;
        if (revents & pollExceptionMask)
        {
            this->_handleException(i);
        }
        // Read and process a new request
        else if (revents & POLLIN)
        {
            int fd = this->_pollfdManager->getFd(i);
            if (this->_requestHandler->handleRequest(fd) == -1)
            {
                // Remove the socket from the poll set
                this->_pollfdManager->removePollfd(i);
                // Decrement i to compensate for the removal
                i--;
            }
            else
            {
                // Add the POLLOUT event for the socket
                this->_pollfdManager->addPollOut(i);
            }
        }
        // Write buffered response
        else if (revents & POLLOUT)
        {
            this->_processBufferedResponse(i);
        }
    }
}

void EventManager::_handleFileDescriptorEvents()
{
    // Check if the error log file descriptor is ready for writing
    short errorLogEvents = this->_pollfdManager->getErrorLogEvents();
    if ((errorLogEvents & POLLOUT) != 0)
    {
        // Flush error logs buffer
        int fd = this->_pollfdManager->getErrorLogFd();
        this->_bufferManager->flushBuffer(fd);
    }

    // Check if the access log file descriptor is ready for writing
    short accessLogEvents = this->_pollfdManager->getAccessLogEvents();
    if ((accessLogEvents & POLLOUT) != 0)
    {
        // Flush access logs buffer
        int fd = this->_pollfdManager->getAccessLogFd();
        this->_bufferManager->flushBuffer(fd);
    }
}

void EventManager::_processBufferedResponse(size_t &i)
{
    // Get the socket descriptor
    int fd = this->_pollfdManager->getFd(i);

    // Flush the buffer
    if (this->_bufferManager->flushBuffer(fd) == 0) // check if all bytes were sent
    {
        // Remove the socket from the poll set
        this->_pollfdManager->removePollfd(i);
    }
}

void EventManager::_handleException(size_t &i)
{
    short revents = this->_pollfdManager->getEvents(i);
    int fd = this->_pollfdManager->getFd(i);

    if (revents & POLLHUP)
    {
        // Log the disconnection
        this->_errorLogger->errorLog(INFO, "Client disconnected socket: " + std::to_string(fd));
    }

    if (revents & POLLNVAL)
    {
        // Log the error
        this->_errorLogger->errorLog(ERROR, "Invalid request on socket: " + std::to_string(fd));

        // Send a 400 response to the client
        std::vector<char> response = {'H', 'T', 'T', 'P', '/', '1', '.', '1', ' ', '4', '0', '0', ' ', 'B', 'a', 'd', ' ', 'R', 'e', 'q', 'u', 'e', 's', 't', '\r', '\n',
                                      'C', 'o', 'n', 't', 'e', 'n', 't', '-', 'L', 'e', 'n', 'g', 't', 'h', ':', ' ', '0', '\r', '\n', '\r', '\n'};
        if (this->_socket->send(fd, response) == -1)
        {
            // Log the error
            this->_errorLogger->errorLog(ERROR, "Error sending 400 response to socket: " + std::to_string(fd));
        }
    }

    // Check for errors on the socket
    if (revents & POLLERR)
    {
        // Log the error
        this->_errorLogger->errorLog(ERROR, "Error on socket: " + std::to_string(fd));

        // Send a 500 response to the client
        std::vector<char> response = {'H', 'T', 'T', 'P', '/', '1', '.', '1', ' ', '5', '0', '0', ' ', 'I', 'n', 't', 'e', 'r', 'n', 'a', 'l', ' ', 'S', 'e', 'r', 'v', 'e', 'r', ' ', 'E', 'r', 'r', 'o', 'r', '\r', '\n',
                                      'C', 'o', 'n', 't', 'e', 'n', 't', '-', 'L', 'e', 'n', 'g', 't', 'h', ':', ' ', '0', '\r', '\n', '\r', '\n'};
        if (this->_socket->send(fd, response) == -1)
        {
            // Log the error
            this->_errorLogger->errorLog(ERROR, "Error sending 500 response to socket: " + std::to_string(fd));
        }
    }
    // Destroy the buffer associated with the socket
    this->_bufferManager->destroyBuffer(fd);
    // Close and Remove the socket from the poll set
    this->_pollfdManager->removePollfd(i);
    // Decrement i to compensate for the removal
    i--;
}