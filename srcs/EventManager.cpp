#include "../includes/EventManager.hpp"
#define NO_EVENTS 0xC0

EventManager::EventManager(IPollfdManager &pollfdManager, IBufferManager &bufferManager, ISocket &socket, IServer &server, IRequestHandler &requestHandler, ILogger &logger)
    : _pollfdManager(pollfdManager),
      _bufferManager(bufferManager),
      _socket(socket),
      _server(server),
      _requestHandler(requestHandler),
      _logger(logger) {}

EventManager::~EventManager() {}

void EventManager::handleEvents()
{
    for (ssize_t pollfdIndex = 0;
         pollfdIndex < this->_pollfdManager.getPollfdQueueSize();
         pollfdIndex++)
    {
        short events = this->_pollfdManager.getEvents(pollfdIndex);

        if (events == NO_EVENTS)
        {
            continue;
        }
        else if (events & REGULAR_FILE)
        {
            this->_handleRegularFileEvents(pollfdIndex, events);
        }
        else if (events & SERVER_SOCKET)
        {
            this->_handleServerSocketEvents(pollfdIndex, events);
        }
        else if (events & CLIENT_SOCKET)
        {
            this->_handleClientSocketEvents(pollfdIndex, events);
        }
        else if (events & PIPE)
        {
            this->_handlePipeEvents(pollfdIndex, events);
        }
    }
}

void EventManager::_handleRegularFileEvents(ssize_t &pollfdIndex, short events)
{
    // Check if file is ready for writing
    if (events & POLLOUT)
    {
        int fileDescriptor = this->_pollfdManager.getDescriptor(pollfdIndex);
        // Flush the buffer
        if (this->_bufferManager.flushBuffer(fileDescriptor) == 0) // check if no bytes remain in the buffer
        {
            // Clear buffer, remove from polling and close file
            this->_cleanUp(pollfdIndex, fileDescriptor);
        }
    }

    // Check for invalid request on the file
    if (events & POLLNVAL)
    {
        // Get the file descriptor
        int fileDescriptor = this->_pollfdManager.getDescriptor(pollfdIndex);

        // Log the error
        this->_logger.log(ERROR, "Invalid request on file: " + std::to_string(fileDescriptor));

        // Clear buffer, remove from polling and close file
        this->_cleanUp(pollfdIndex, fileDescriptor);
    }
}

void EventManager::_handleServerSocketEvents(ssize_t pollfdIndex, short events)
{
    // Check for errors on server socket
    if (events & POLLERR)
        throw ServerSocketError();

    // Check for server socket closed
    if (events & POLLHUP)
        throw ServerSocketClosedError();

    // Check for invalid request on server socket
    if (events & POLLNVAL)
        throw ServerSocketInvalidError();

    // Check if server socket is ready to accept incoming connections
    if (events & POLLIN)
    {
        // Accept incoming connection
        int serverSocketDescriptor = this->_pollfdManager.getDescriptor(pollfdIndex);
        this->_server.acceptConnection(serverSocketDescriptor);
    }
}

void EventManager::_handleClientSocketEvents(ssize_t &pollfdIndex, short events)
{
    // Check for exceptions
    if (events & (POLLHUP | POLLERR | POLLNVAL))
    {
        this->_handleClientException(pollfdIndex, events);
    }

    // Read and process a new request if ready
    else if (events & POLLIN)
    {
        this->_handleRequest(pollfdIndex);
    }

    // Send response
    else if (events & POLLOUT)
    {
        this->_flushClientBuffer(pollfdIndex);
    }
}

void EventManager::_handleRequest(ssize_t &pollfdIndex)
{
    int clientSocketDescriptor = this->_pollfdManager.getDescriptor(pollfdIndex);

    int pipeDescriptor = this->_requestHandler.handleRequest(clientSocketDescriptor);

    if (pipeDescriptor == 0) // no pipe; served static files or bad request
    {
        // Add the POLLOUT event for the socket
        this->_pollfdManager.addPollOut(pollfdIndex);
    }
    else // read pipe returned
    {
        // Add the pipe to the poll set
        this->_pollfdManager.addPipePollfd({pipeDescriptor, POLLIN, 0});
    }
}

void EventManager::_flushClientBuffer(ssize_t pollfdIndex)
{
    // Get the socket descriptor
    int descriptor = this->_pollfdManager.getDescriptor(pollfdIndex);

    // Flush the buffer
    if (this->_bufferManager.flushBuffer(descriptor) == 0) // check if all bytes were sent
    {
        // Clear buffer, remove from polling and close socket
        this->_cleanUp(pollfdIndex, descriptor);
    }
}

void EventManager::_handleClientException(ssize_t &pollfdIndex, short events)
{
    int descriptor = this->_pollfdManager.getDescriptor(pollfdIndex);

    // Check for disconnection on the socket
    if (events & POLLHUP)
    {
        // Log the disconnection
        this->_logger.log(INFO, "Client disconnected socket: " + std::to_string(descriptor));

        // Clear buffer, remove from polling and close socket
        this->_cleanUp(pollfdIndex, descriptor);
    }

    // Check for invalid request on the socket
    if (events & POLLNVAL)
    {
        // Log the error
        this->_logger.log(ERROR, "Invalid request on socket: " + std::to_string(descriptor));

        // Destroy the buffer associated with the descriptor
        this->_bufferManager.destroyBuffer(descriptor);

        // Let the request handler handle the error
        this->_requestHandler.handleErrorResponse(descriptor, BAD_REQUEST); // 400

        // Add the POLLOUT event for the socket
        this->_pollfdManager.addPollOut(pollfdIndex);
    }

    // Check for errors on the socket
    if (events & POLLERR)
    {
        // Log the error
        this->_logger.log(ERROR, "Error on socket: " + std::to_string(descriptor));

        // Destroy the buffer associated with the descriptor
        this->_bufferManager.destroyBuffer(descriptor);

        // Let the request handler handle the error
        this->_requestHandler.handleErrorResponse(descriptor, INTERNAL_SERVER_ERROR); // 500

        // Add the POLLOUT event for the socket
        this->_pollfdManager.addPollOut(pollfdIndex);
    }
}

void EventManager::_cleanUp(ssize_t &pollfdIndex, int descriptor)
{
    // Destroy the buffer associated with the descriptor
    this->_bufferManager.destroyBuffer(descriptor);

    // Close the descriptor
    close(descriptor);

    // Remove the descriptor from the poll set
    this->_pollfdManager.removePollfd(pollfdIndex);

    // Decrement i to compensate for the removal
    pollfdIndex--;

    // Log the cleanup
    this->_logger.log(INFO, "Cleaned up descriptor: " + std::to_string(descriptor));
}

void EventManager::_handlePipeEvents(ssize_t &pollfdIndex, short events)
{
    // Get the pipe descriptor
    int pipeDescriptor = this->_pollfdManager.getDescriptor(pollfdIndex);

    // Declare the client socket descriptor linked to the pipe
    int clientSocket;

    // Check for exceptions
    if (events & (POLLHUP | POLLERR | POLLNVAL))
    {
        // Let the request handler handle the exception, returns the client socket descriptor linked to the pipe
        clientSocket = this->_requestHandler.handlePipeException(pipeDescriptor);
    }

    // Read and process a new request if ready
    else if (events & POLLIN)
    {
        // Let the request handler handle the pipe read, returns the client socket descriptor linked to the pipe
        clientSocket = this->_requestHandler.handlePipeRead(pipeDescriptor);
    }

    // Add the POLLOUT event for the client socket
    this->_pollfdManager.addPollOut(clientSocket);

    // Clear buffer, remove from polling and close pipe
    this->_cleanUp(pollfdIndex, pipeDescriptor);
}

// Path: srcs/EventManager.cpp
