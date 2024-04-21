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
    for (ssize_t pollfdIndex = 0; pollfdIndex < this->_pollfdManager.getPollfdQueueSize(); pollfdIndex++)
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
            this->_handlePipeEvents(pollfdIndex);
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
            // Remove the file from the poll set
            this->_pollfdManager.removePollfd(pollfdIndex);
            // Decrement i to compensate for the removal
            pollfdIndex--;
        }
    }

    // Check for invalid request on the file
    if (events & POLLNVAL)
    {
        // Get the file descriptor
        int fileDescriptor = this->_pollfdManager.getDescriptor(pollfdIndex);

        // Destroy the buffer associated with the file
        this->_bufferManager.destroyBuffer(fileDescriptor);

        // Remove the file from the poll set
        this->_pollfdManager.removePollfd(pollfdIndex);

        // Close the file
        close(fileDescriptor);

        // Log the error
        this->_logger.log(ERROR, "Invalid request on file: " + std::to_string(fileDescriptor));
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

    // Read and process a new request
    else if (events & POLLIN)
    {
        int clientSocketDescriptor = this->_pollfdManager.getDescriptor(pollfdIndex);
        int pipeDescriptor = this->_requestHandler.handleRequest(clientSocketDescriptor);
        if (pipeDescriptor == -1) // error
        {
            this->_cleanUp(pollfdIndex, clientSocketDescriptor);
        }
        else if (pipeDescriptor == 0) // no pipe, served static files
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

    // Send response
    else if (events & POLLOUT)
    {
        this->_flushClientBuffer(pollfdIndex);
    }
}

void EventManager::_flushClientBuffer(ssize_t pollfdIndex)
{
    // Get the socket descriptor
    int descriptor = this->_pollfdManager.getDescriptor(pollfdIndex);

    // Flush the buffer
    if (this->_bufferManager.flushBuffer(descriptor) == 0) // check if all bytes were sent
    {
        this->_cleanUp(pollfdIndex, descriptor);
    }
}

void EventManager::_handleClientException(ssize_t &pollfdIndex, short events)
{
    int descriptor = this->_pollfdManager.getDescriptor(pollfdIndex);

    if (events & POLLHUP)
    {
        // Log the disconnection
        this->_logger.log(INFO, "Client disconnected socket: " + std::to_string(descriptor));
    }

    if (events & POLLNVAL)
    {
        // Log the error
        this->_logger.log(ERROR, "Invalid request on socket: " + std::to_string(descriptor));

        // Send a 400 response to the client
        std::vector<char> response = {'H', 'T', 'T', 'P', '/', '1', '.', '1', ' ', '4', '0', '0', ' ', 'B', 'a', 'd', ' ', 'R', 'e', 'q', 'u', 'e', 's', 't', '\r', '\n',
                                      'C', 'o', 'n', 't', 'e', 'n', 't', '-', 'L', 'e', 'n', 'g', 't', 'h', ':', ' ', '0', '\r', '\n', '\r', '\n'};
        if (this->_socket.send(descriptor, response) == -1)
        {
            // Log the error
            this->_logger.log(ERROR, "Error sending 400 response to socket: " + std::to_string(descriptor));
        }
    }

    // Check for errors on the socket
    if (events & POLLERR)
    {
        // Log the error
        this->_logger.log(ERROR, "Error on socket: " + std::to_string(descriptor));

        // Send a 500 response to the client
        std::vector<char> response = {'H', 'T', 'T', 'P', '/', '1', '.', '1', ' ', '5', '0', '0', ' ', 'I', 'n', 't', 'e', 'r', 'n', 'a', 'l', ' ', 'S', 'e', 'r', 'v', 'e', 'r', ' ', 'E', 'r', 'r', 'o', 'r', '\r', '\n',
                                      'C', 'o', 'n', 't', 'e', 'n', 't', '-', 'L', 'e', 'n', 'g', 't', 'h', ':', ' ', '0', '\r', '\n', '\r', '\n'};
        if (this->_socket.send(descriptor, response) == -1)
        {
            // Log the error
            this->_logger.log(ERROR, "Error sending 500 response to socket: " + std::to_string(descriptor));
        }
    }

    // Clean up the socket
    this->_cleanUp(pollfdIndex, descriptor);
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
