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
Server::Server(const ISocket &socket, IPollfdManager &pollfdManager, ISessionManager &sessionManager, const IConfiguration &configuration, ILogger &logger)
    : _socket(socket),
      _pollfdManager(pollfdManager),
      _sessionManager(sessionManager),
      _logger(logger)
{
    std::vector<IBlock *> servers = configuration.getBlocks("server");
    for (std::vector<IBlock *>::iterator serverIterator = servers.begin();
         serverIterator != servers.end();
         serverIterator++)
    {
        int ip = (*serverIterator)->getInt("listenIp"); // 0 for all interfaces
        int port = (*serverIterator)->getInt("listenPort");
        int maxConnections = (*serverIterator)->getInt("maxConnections");
        this->_initializeServerSocket(ip, port, maxConnections);
    }
    this->_logger.log(INFO, "Finished Server initialization");
}

/* Destructor to close file descriptors*/
Server::~Server()
{
    // Close all socket file descriptors
    this->_pollfdManager.closeAllFileDescriptors();
}

/* Initialize server socket*/
void Server::_initializeServerSocket(int ip, int port, int maxConnections)
{
    // Create server socket
    int serverSocketDescriptor = this->_socket.socket();
    if (serverSocketDescriptor < 0)
        throw SocketCreateError();
    this->_logger.log(INFO, "Server socket created.");

    // Bind server socket to port
    if (this->_socket.bind(serverSocketDescriptor, ip, port) < 0)
        throw SocketBindError();
    this->_logger.log(INFO, "Server socket bound to port " + std::to_string(port));

    // Listen for incoming connections
    if (this->_socket.listen(serverSocketDescriptor, maxConnections) < 0)
        throw SocketListenError();
    this->_logger.log(INFO, "Server socket set to listening.");

    // Set server socket to non-blocking mode
    if (this->_socket.setNonBlocking(serverSocketDescriptor) < 0)
        throw SocketSetError();
    this->_logger.log(INFO, "Server socket set to non-blocking.");

    // Add server socket to polling list
    short pollMask = POLLIN | POLLERR | POLLHUP | POLLNVAL;
    this->_pollfdManager.addServerSocketPollfd({serverSocketDescriptor, pollMask, 0});

    // Log server socket initialization
    this->_logger.log(DEBUG, "Server socket initialized. Listening on " + (ip ? std::to_string(ip) : "ALL") + ":" + std::to_string(port));
}

/* Terminate server*/
void Server::terminate(int exit_code)
{
    exit(exit_code);
}

/* Accept a new client connection*/
void Server::acceptConnection(int serverSocketDescriptor)
{
    // Ensure maximum connections limit has not been reached
    if (this->_pollfdManager.hasReachedCapacity())
        throw MaximumConnectionsReachedError();

    // Accept incoming connection
    std::pair<int, std::pair<std::string, std::string>> clientInfo = this->_socket.accept(serverSocketDescriptor);
    int clientSocketDescriptor = clientInfo.first;
    std::string clientIP = clientInfo.second.first;
    std::string clientPort = clientInfo.second.second;

    // Create a session for the client
    this->_sessionManager.addSession(clientInfo);

    // Add client socket to polling list
    if (clientSocketDescriptor < 0)
        throw ConnectionEstablishingError();
    short pollMask = POLLIN | POLLERR | POLLHUP | POLLNVAL;
    this->_pollfdManager.addClientSocketPollfd({clientSocketDescriptor, pollMask, 0});
    
    // Set socket to non-blocking mode
    if (this->_socket.setNonBlocking(clientSocketDescriptor) < 0)
        throw SocketSetError();

    // Log accepted connection
    this->_logger.log(VERBOSE, "Accepted new connection from " + clientIP + ":" + clientPort + ".");
}
