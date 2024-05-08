#include "../../includes/core/EventManager.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include <unistd.h>


#define NO_EVENTS 0xC0
#define KEEP_DESCRIPTOR 0x01

EventManager::EventManager(IPollfdManager &pollfdManager, IBufferManager &bufferManager, IServer &server, IRequestHandler &requestHandler, ILogger &logger)
    : _pollfdManager(pollfdManager),
      _bufferManager(bufferManager),
      _server(server),
      _requestHandler(requestHandler),
      _logger(logger) {}

EventManager::~EventManager() {}

void EventManager::handleEvents()
{
    this->_logger.log(VERBOSE, "[EVENTMANAGER] Handling events");

    for (ssize_t pollfdIndex = 0;
         pollfdIndex < static_cast<ssize_t>(this->_pollfdManager.getPollfdQueueSize());
         pollfdIndex++)
    {
        short events = this->_pollfdManager.getEvents(pollfdIndex);

        if (events == NO_EVENTS)
            continue;
        
        short fileType = events & NO_EVENTS;
        if (fileType == SERVER_SOCKET)
        {
            this->_handleServerSocketEvents(pollfdIndex, events);
        }
        else if (fileType == CLIENT_SOCKET)
        {
            this->_handleClientSocketEvents(pollfdIndex, events);
        }
        else if (fileType == PIPE)
        {
            this->_handlePipeEvents(pollfdIndex, events);
        }
        else if (fileType == REGULAR_FILE)
        {
            this->_handleRegularFileEvents(pollfdIndex, events);
        }
    }
}

void EventManager::_handleRegularFileEvents(ssize_t &pollfdIndex, short events)
{
    this->_logger.log(VERBOSE, "[EVENTMANAGER] Handling regular file events");

    // Check if file is ready for writing
    if (events & POLLOUT)
    {
        this->_flushBuffer(pollfdIndex, KEEP_DESCRIPTOR);
    }

    // Check for error on the file
    if (events & POLLNVAL)
    {
        // Get the file descriptor
        int fileDescriptor = this->_pollfdManager.getDescriptor(pollfdIndex);

        // Log the error
        this->_logger.log(ERROR, "Error on file: " + std::to_string(fileDescriptor));

        // Clear buffer, remove from polling and close file
        this->_cleanUp(pollfdIndex, fileDescriptor);
    }
}

void EventManager::_handleServerSocketEvents(ssize_t pollfdIndex, short events)
{
    this->_logger.log(VERBOSE, "[EVENTMANAGER] Handling server socket events");
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
    this->_logger.log(VERBOSE, "[EVENTMANAGER] Handling client socket events");
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
        this->_flushBuffer(pollfdIndex);
    }
}

void EventManager::_handleRequest(ssize_t &pollfdIndex)
{
    int clientSocketDescriptor = this->_pollfdManager.getDescriptor(pollfdIndex);

    Triplet_t info = this->_requestHandler.handleRequest(clientSocketDescriptor);

    if (info.first == -1) // served static files or bad request
    {
        // Log the static serving
        this->_logger.log(VERBOSE, "[EVENTMANAGER] Statically served client socket: " + std::to_string(clientSocketDescriptor));

        // Add the POLLOUT event for the socket
        this->_pollfdManager.addPollOut(pollfdIndex);
    }
    else // read pipe returned
    {
        // Get the info
        int cgiPid = info.first;
        int responseReadPipe = info.second.first;
        int requestWritePipe = info.second.second;
        
        // Log the dynamic serving
        this->_logger.log(VERBOSE, "[EVENTMANAGER] Dynamically serving client socket: " + std::to_string(clientSocketDescriptor) + " waiting for process " + std::to_string(cgiPid) + " (response read pipe: " + std::to_string(responseReadPipe) + ", request write pipe: " + std::to_string(requestWritePipe) + ")");
        
        // Add the response read pipe to the poll set
        pollfd pollfd;
        pollfd.fd = responseReadPipe;
        pollfd.events = POLLIN;
        pollfd.revents = 0;
        this->_pollfdManager.addPipePollfd(pollfd);

        // Add the request write pipe to the poll set
        pollfd.fd = requestWritePipe;
        pollfd.events = POLLOUT;
        pollfd.revents = 0;
        this->_pollfdManager.addPipePollfd(pollfd);
    }
}

void EventManager::_flushBuffer(ssize_t &pollfdIndex, short options)
{
    // Get the socket descriptor
    int descriptor = this->_pollfdManager.getDescriptor(pollfdIndex);

    // Flush the buffer
    ssize_t returnValue = this->_bufferManager.flushBuffer(descriptor);
    if (returnValue == -1) // check for errors
    {
        // Log the error
        this->_logger.log(ERROR, "Error flushing buffer for descriptor: " + std::to_string(descriptor));

        // Clear buffer, remove from polling and close socket
        this->_cleanUp(pollfdIndex, descriptor);
    }
    else if (returnValue == 0) // check if all bytes were sent
    {
        // Log the flush
        this->_logger.log(VERBOSE, "Flushed buffer for descriptor: " + std::to_string(descriptor));

        // Clear buffer, remove from polling and close socket
        this->_cleanUp(pollfdIndex, descriptor, options);
    }
    else
    {
        // Log the flush
        this->_logger.log(VERBOSE, "Partially Flushed buffer for descriptor: " + std::to_string(descriptor) + " with " + std::to_string(returnValue) + " bytes remaining");
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

void EventManager::_cleanUp(ssize_t &pollfdIndex, int descriptor, short options)
{
    // Destroy the buffer associated with the descriptor
    this->_bufferManager.destroyBuffer(descriptor);

    // Close the descriptor
    if (options != KEEP_DESCRIPTOR)
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
        // Log the error
        this->_logger.log(ERROR, "Error on pipe: " + std::to_string(pipeDescriptor));

        // Let the request handler handle the exception, returns the client socket descriptor linked to the pipe
        clientSocket = this->_requestHandler.handlePipeException(pipeDescriptor);
    }

    // Read the response from the Response pipe if ready
    else if (events & POLLIN)
    {
        // Log the pipe read
        this->_logger.log(VERBOSE, "Pipe read event on pipe: " + std::to_string(pipeDescriptor));

        // Let the request handler handle the pipe read, returns the client socket descriptor linked to the pipe
        clientSocket = this->_requestHandler.handlePipeRead(pipeDescriptor);

        // Add the POLLOUT event for the client socket since the response is ready
        this->_pollfdManager.addPollOut(clientSocket);

        // Clear buffer, remove from polling and close pipe
        this->_cleanUp(pollfdIndex, pipeDescriptor);
    }

    // Write the request body to the Request pipe to the cgi process if ready
    else if (events & POLLOUT)
    {
        // Log the pipe write
        this->_logger.log(VERBOSE, "Pipe write event on pipe: " + std::to_string(pipeDescriptor));

        // Flush the buffer
        this->_flushBuffer(pollfdIndex);
    }
}

// Path: srcs/EventManager.cpp
