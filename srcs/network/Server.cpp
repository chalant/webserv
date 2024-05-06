#include "network/Server.hpp"

/*
 * The Server class is responsible for managing core operations of webserv, including initialization, connection handling, and termination.
 *
 * It oversees socket creation, binding, and listening for incoming connections, all configured for non-blocking operation.
 * Leveraging the poll() polling mechanism, Server monitors events on the server socket, log file descriptors, and client connections,
 * facilitating the acceptance of incoming connections and their inclusion in the monitoring queue.
 *
 */

/* Constructor - Initializes the Server object and sets up the server socket and polling file descriptors.*/
Server::Server(const ISocket &socket, IPollfdManager &pollfdManager, IConnectionManager &connectionManager, const IConfiguration &configuration, ILogger &logger)
    : _socket(socket),
      _pollfdManager(pollfdManager),
      _connectionManager(connectionManager),
      _logger(logger)
{
    // Get the maximum connections value
    std::string maxConnectionsString = configuration.getBlocks("http")[0]->getString("worker_connections");
    int maxConnections = std::stoi(maxConnectionsString);

    // Create a set to store unique IP:port combinations
    std::set<std::pair<int, int>> processedEndpoints;

    // Get the list of virtual servers
    std::vector<IConfiguration *> servers = configuration.getBlocks("http")[0]->getBlocks("server");

    // For each virtual server
    for (std::vector<IConfiguration *>::iterator serverIterator = servers.begin();
         serverIterator != servers.end();
         serverIterator++)
    {
        // Get the list of listen directives
        std::vector<std::string> listenVector = (*serverIterator)->getStringVector("listen");

        // For each listen directive
        for (std::vector<std::string>::iterator listenIterator = listenVector.begin();
             listenIterator != listenVector.end();
             listenIterator++)
        {
            int ip = 0; // Default IP to 0 (all network interfaces)
            int port;

            // Find the position of the colon (if present, ip was specified)
            size_t colonPos = listenIterator->find(':');
            if (colonPos != std::string::npos)
            {
                ip = std::stoi(listenIterator->substr(0, colonPos));
                port = std::stoi(listenIterator->substr(colonPos + 1));
            }
            else // ip was not specified
            {
                port = std::stoi(*listenIterator);
            }

            // Check if the current IP:port combination has already been processed
            if (processedEndpoints.find(std::make_pair(ip, port)) == processedEndpoints.end())
            {
                // Add the current IP:port combination to the set of processed endpoints
                processedEndpoints.insert(std::make_pair(ip, port));

                // Initialize a new socket
                this->_initializeServerSocket(ip, port, maxConnections);
            }
        }
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
void Server::terminate(int exitCode)
{
    exit(exitCode);
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

    // Create a connection for the client
    this->_connectionManager.addConnection(clientInfo);

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
