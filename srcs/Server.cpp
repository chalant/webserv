#include "../includes/Server.hpp"

/*
 * The Server class is responsible for managing core operations of webserv, including initialization, connection handling, and termination.
 *
 * It oversees socket creation, binding, and listening for incoming connections, all configured for non-blocking operation.
 * Leveraging the poll() polling mechanism, Server monitors events on the server socket, log file descriptors, and client connections,
 * facilitating the acceptance of incoming connections and their inclusion in the monitoring queue.
 *
 */

/* Constructor - Initializes the Server object and sets up the server socket and polling file descriptors.*/
Server::Server(const ISocket *socket, IPollfdManager *pollfdManager, const IConfiguration *configuration, ILogger *errorLogger, ILogger *accessLogger, IExceptionHandler *exceptionHandler)
    : _socket(socket),
      _pollfdManager(pollfdManager),
      _configuration(configuration),
      _errorLogger(errorLogger),
      _accessLogger(accessLogger),
      _exceptionHandler(exceptionHandler),
      _pollMask(POLLIN | POLLERR | POLLHUP | POLLNVAL),
      _serverSocketDescriptor(socket->socket())
{
    // Create server socket
    if (this->_serverSocketDescriptor < 0)
        throw SocketCreateError();
    this->_errorLogger->errorLog(DEBUG, "Server socket created.");

    // Bind server socket to port
    if (this->_socket->bind(this->_serverSocketDescriptor, this->_configuration->getPort()) < 0)
        throw SocketBindError();
    this->_errorLogger->errorLog(DEBUG, "Server socket bound to port " + std::to_string(this->_configuration->getPort()));

    // Listen for incoming connections
    if (this->_socket->listen(this->_serverSocketDescriptor, this->_configuration->getMaxConnections()) < 0)
        throw SocketListenError();
    this->_errorLogger->errorLog(DEBUG, "Server socket set to listening.");

    // Set server socket to non-blocking mode
    if (this->_socket->setNonBlocking(this->_serverSocketDescriptor) < 0)
        throw SocketSetError();
    this->_errorLogger->errorLog(DEBUG, "Server socket set to non-blocking.");

    this->_errorLogger->errorLog(INFO, "Server initialized. Listening on port " + std::to_string(this->_configuration->getPort()) + ".");
}

/* Poll events on server socket */
void Server::pollEvents()
{
    // Get the pollfd array
    pollfd *pollfdArray = this->_pollfdManager->getPollfdArray();

    // Get pollfd queue size
    size_t pollfdQueueSize = this->_pollfdManager->getPollfdQueueSize();

    // Poll events on server socket
    if (this->_socket->poll(pollfdArray, pollfdQueueSize) < 0)
        throw PollError();
}

/*Accept incoming connections*/
void Server::acceptConnection()
{
    // Get Server Socket events
    int serverSocketEvents = this->_pollfdManager->getEvents(SERVER_SOCKET_POLL_FD);

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
        // Ensure maximum connections limit has not been reached
        if (this->_pollfdManager->hasReachedCapacity())
            throw MaximumConnectionsReachedError();

        // Accept incoming connection
        std::pair<int, std::pair<std::string, std::string>> clientInfo = this->_socket->accept(this->_serverSocketDescriptor);
        int clientSocketDescriptor = clientInfo.first;
        
        //currently unused
        //std::string clientIP = clientInfo.second.first;
        //std::string clientPort = clientInfo.second.second;

        // Add client socket to polling list
        if (clientSocketDescriptor < 0)
            throw ConnectionEstablishingError();
        this->_pollfdManager->addPollfd({clientSocketDescriptor, this->_pollMask, 0});

        // Set socket to non-blocking mode
        if (this->_socket->setNonBlocking(clientSocketDescriptor) < 0)
            throw SocketSetError();

        // Log accepted connection
        this->_errorLogger->errorLog(VERBOSE, "Accepted new connection.");
    }
    else
    {
        // Log no incoming connection
        this->_errorLogger->errorLog(VERBOSE, "No incoming connection.");
    }
}

/* Destructor to close file descriptors*/
Server::~Server()
{
    // Close all socket file descriptors
    this->_pollfdManager->closeAllFileDescriptors();
}

/* Terminate server*/
void Server::terminate(int exit_code)
{
    delete this->_socket;
    exit(exit_code);
}

/* Get server socket descriptor*/
int Server::getServerSocketDescriptor() const
{
    return this->_serverSocketDescriptor;
}
