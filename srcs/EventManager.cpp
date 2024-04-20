#include "../includes/EventManager.hpp"

EventManager::EventManager(IPollfdManager &pollfdManager, IBufferManager &bufferManager, ISocket &socket, IServer &server, IRequestHandler &requestHandler, ILogger &errorLogger)
    : _pollfdManager(pollfdManager),
      _bufferManager(bufferManager),
      _socket(socket),
      _server(server),
      _requestHandler(requestHandler),
      _errorLogger(errorLogger) {}

EventManager::~EventManager() {}

void EventManager::handleEvents()
{
    this->_handleFileDescriptorsEvents();
    this->_handleServerSocketsEvents();
    this->_handleClientSocketsEvents();
}

void EventManager::_handleServerSocketsEvents()
{
    this->_errorLogger.errorLog(DEBUG, "Processing Server Socket Events.");

    for (ssize_t serverSocketIndex = this->_pollfdManager.getServerSocketsIndex();
         serverSocketIndex < this->_pollfdManager.getClientSocketsIndex();
         serverSocketIndex++)
    {
        this->_handleServerSocketEvents(serverSocketIndex);
    }
}

void EventManager::_handleServerSocketEvents(ssize_t serverSocketIndex)
{
    short serverSocketEvents = this->_pollfdManager.getEvents(serverSocketIndex);
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
        int serverSocketDescriptor = this->_pollfdManager.getDescriptor(serverSocketIndex);
        this->_server.acceptConnection(serverSocketDescriptor);
    }
    else
    {
        // Log no incoming connection
        this->_errorLogger.errorLog(VERBOSE, "No incoming connection.");
    }
}

void EventManager::_handleClientSocketsEvents()
{
    this->_errorLogger.errorLog(DEBUG, "Processing Client Socket Events.");

    for (ssize_t clientSocketIndex = this->_pollfdManager.getClientSocketsIndex();
         clientSocketIndex < this->_pollfdManager.getPollfdQueueSize();
         clientSocketIndex++)
    {
        this->_handleClientSocketEvents(clientSocketIndex);
    }
}
void EventManager::_handleClientSocketEvents(ssize_t &clientSocketIndex)
{
    int clientSocketDescriptor = this->_pollfdManager.getDescriptor(clientSocketIndex);
    short clientSocketEvents = this->_pollfdManager.getEvents(clientSocketIndex);

    // If there are no events, return immediately
    if (clientSocketEvents == 0)
        return;

    // Check for exceptions
    short pollExceptionMask = POLLHUP | POLLERR | POLLNVAL;
    if (clientSocketEvents & pollExceptionMask)
    {
        this->_handleException(clientSocketIndex);
    }
    // Read and process a new request
    else if (clientSocketEvents & POLLIN)
    {
        int fd = this->_pollfdManager.getDescriptor(clientSocketIndex);
        if (this->_requestHandler.handleRequest(clientSocketDescriptor) == -1)
        {
            // Remove the socket from the poll set
            this->_pollfdManager.removePollfd(clientSocketIndex);
            // Decrement i to compensate for the removal
            clientSocketIndex--;
        }
        else
        {
            // Add the POLLOUT event for the socket
            this->_pollfdManager.addPollOut(clientSocketIndex);
        }
    }
    // Write buffered response
    else if (clientSocketEvents & POLLOUT)
    {
        this->_processBufferedResponse(clientSocketIndex);
    }
}

void EventManager::_handleFileDescriptorsEvents()
{
    for (ssize_t fileDescriptorIndex = this->_pollfdManager.getFileDescriptorsIndex();
         fileDescriptorIndex < this->_pollfdManager.getServerSocketsIndex();
         fileDescriptorIndex++)
    {
        short fileDescriptorEvents = this->_pollfdManager.getEvents(fileDescriptorIndex);
        if (fileDescriptorEvents & POLLOUT)
        {
            // Flush error logs buffer
            int fileDescriptor = this->_pollfdManager.getDescriptor(fileDescriptorIndex);
            this->_bufferManager.flushBuffer(fileDescriptor);
        }
    }
}

void EventManager::_processBufferedResponse(ssize_t clientSocketIndex)
{
    // Get the socket descriptor
    int clientSocketDescriptor = this->_pollfdManager.getDescriptor(clientSocketIndex);

    // Flush the buffer
    if (this->_bufferManager.flushBuffer(clientSocketDescriptor) == 0) // check if all bytes were sent
    {
        // Remove the socket from the poll set
        this->_pollfdManager.removePollfd(clientSocketIndex);
    }
}

void EventManager::_handleException(ssize_t &clientSocketIndex)
{
    int clientSocketDescriptor = this->_pollfdManager.getDescriptor(clientSocketIndex);
    short clientSocketEvents = this->_pollfdManager.getEvents(clientSocketIndex);

    if (clientSocketEvents & POLLHUP)
    {
        // Log the disconnection
        this->_errorLogger.errorLog(INFO, "Client disconnected socket: " + std::to_string(clientSocketDescriptor));
    }

    if (clientSocketEvents & POLLNVAL)
    {
        // Log the error
        this->_errorLogger.errorLog(ERROR, "Invalid request on socket: " + std::to_string(clientSocketDescriptor));

        // Send a 400 response to the client
        std::vector<char> response = {'H', 'T', 'T', 'P', '/', '1', '.', '1', ' ', '4', '0', '0', ' ', 'B', 'a', 'd', ' ', 'R', 'e', 'q', 'u', 'e', 's', 't', '\r', '\n',
                                      'C', 'o', 'n', 't', 'e', 'n', 't', '-', 'L', 'e', 'n', 'g', 't', 'h', ':', ' ', '0', '\r', '\n', '\r', '\n'};
        if (this->_socket.send(clientSocketDescriptor, response) == -1)
        {
            // Log the error
            this->_errorLogger.errorLog(ERROR, "Error sending 400 response to socket: " + std::to_string(clientSocketDescriptor));
        }
    }

    // Check for errors on the socket
    if (clientSocketEvents & POLLERR)
    {
        // Log the error
        this->_errorLogger.errorLog(ERROR, "Error on socket: " + std::to_string(clientSocketDescriptor));

        // Send a 500 response to the client
        std::vector<char> response = {'H', 'T', 'T', 'P', '/', '1', '.', '1', ' ', '5', '0', '0', ' ', 'I', 'n', 't', 'e', 'r', 'n', 'a', 'l', ' ', 'S', 'e', 'r', 'v', 'e', 'r', ' ', 'E', 'r', 'r', 'o', 'r', '\r', '\n',
                                      'C', 'o', 'n', 't', 'e', 'n', 't', '-', 'L', 'e', 'n', 'g', 't', 'h', ':', ' ', '0', '\r', '\n', '\r', '\n'};
        if (this->_socket.send(clientSocketDescriptor, response) == -1)
        {
            // Log the error
            this->_errorLogger.errorLog(ERROR, "Error sending 500 response to socket: " + std::to_string(clientSocketDescriptor));
        }
    }
    // Destroy the buffer associated with the socket
    this->_bufferManager.destroyBuffer(clientSocketDescriptor);
    // Close and Remove the socket from the poll set
    this->_pollfdManager.removePollfd(clientSocketIndex);
    // Decrement i to compensate for the removal
    clientSocketIndex--;
}