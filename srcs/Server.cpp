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
Server::Server(const ISocket *socket, IPollfdManager *pollfdManager, const IConfiguration *configuration, ILogger *errorLogger)
    : _socket(socket),
      _pollfdManager(pollfdManager),
      _errorLogger(errorLogger),
      _serverSocketDescriptor(socket->socket())
{
    // Create server socket
    if (this->_serverSocketDescriptor < 0)
        throw SocketCreateError();
    this->_errorLogger->errorLog(DEBUG, "Server socket created.");

    // Bind server socket to port
    if (this->_socket->bind(this->_serverSocketDescriptor, configuration->getPort()) < 0)
        throw SocketBindError();
    this->_errorLogger->errorLog(DEBUG, "Server socket bound to port " + std::to_string(configuration->getPort()));

    // Listen for incoming connections
    if (this->_socket->listen(this->_serverSocketDescriptor, configuration->getMaxConnections()) < 0)
        throw SocketListenError();
    this->_errorLogger->errorLog(DEBUG, "Server socket set to listening.");

    // Set server socket to non-blocking mode
    if (this->_socket->setNonBlocking(this->_serverSocketDescriptor) < 0)
        throw SocketSetError();
    this->_errorLogger->errorLog(DEBUG, "Server socket set to non-blocking.");

    this->_errorLogger->errorLog(INFO, "Server initialized. Listening on port " + std::to_string(configuration->getPort()) + ".");
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

/* Accept a new client connection*/
void Server::acceptConnection()
{
    // Ensure maximum connections limit has not been reached
    if (this->_pollfdManager->hasReachedCapacity())
        throw MaximumConnectionsReachedError();

    // Accept incoming connection
    std::pair<int, std::pair<std::string, std::string>> clientInfo = this->_socket->accept(this->_serverSocketDescriptor);
    int clientSocketDescriptor = clientInfo.first;
    std::string clientIP = clientInfo.second.first;
    std::string clientPort = clientInfo.second.second;

    // Add client socket to polling list
    if (clientSocketDescriptor < 0)
        throw ConnectionEstablishingError();
    short pollMask = POLLIN | POLLERR | POLLHUP | POLLNVAL;
    this->_pollfdManager->addPollfd({clientSocketDescriptor, pollMask, 0});

    // Set socket to non-blocking mode
    if (this->_socket->setNonBlocking(clientSocketDescriptor) < 0)
        throw SocketSetError();

    // Log accepted connection
    this->_errorLogger->errorLog(VERBOSE, "Accepted new connection from " + clientIP + ":" + clientPort + ".");
}
