#include "../../includes/network/Server.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include "../../includes/utils/Converter.hpp"
#include <cstdlib>
#include <set>

/*
 * The Server class is responsible for managing core operations of webserv,
 * including initialization, connection handling, and termination.
 *
 * It oversees socket creation, binding, and listening for incoming connections,
 * all configured for non-blocking operation. Leveraging the poll() polling
 * mechanism, Server monitors events on the server socket, log file descriptors,
 * and client connections, facilitating the acceptance of incoming connections
 * and their inclusion in the monitoring queue.
 *
 */

/* Constructor - Initializes the Server object and sets up the server socket and
 * polling file descriptors.*/
Server::Server(const ISocket &socket, IPollfdManager &pollfdManager,
               IConnectionManager &connectionManager,
               IConfiguration &configuration, ILogger &logger)
    : _socket(socket), _pollfdManager(pollfdManager),
      _connectionManager(connectionManager), _logger(logger)
{
    // Log server initialization
    this->_logger.log(VERBOSE, "Initializing Server...");

    // Get the maximum connections value
    int maxConnections =
        configuration.getBlocks("events")[ 0 ]->getInt("worker_connections");

    // Create a set to store unique IP:port combinations
    std::set<std::pair<int, int> > processedEndpoints;

    // Get the list of virtual servers
    std::vector<IConfiguration *> servers =
        configuration.getBlocks("http")[ 0 ]->getBlocks("server");

    // For each virtual server
    for (std::vector<IConfiguration *>::iterator serverIterator =
             servers.begin();
         serverIterator != servers.end(); serverIterator++)
    {
        // Get the list of listen directives
        std::vector<std::string> listenVector =
            (*serverIterator)->getStringVector("listen");

        // For each listen directive
        for (std::vector<std::string>::iterator listenIterator =
                 listenVector.begin();
             listenIterator != listenVector.end(); listenIterator++)
        {
            int ip = 0; // Default IP to 0 (all network interfaces)
            int port;

            // Find the position of the colon (if present, ip was specified)
            size_t colonPos = listenIterator->find(':');
            if (colonPos != std::string::npos)
            {
                ip = Converter::toInt(listenIterator->substr(0, colonPos));
                port = Converter::toInt(listenIterator->substr(colonPos + 1));
            }
            else // ip was not specified
            {
                port = Converter::toInt(*listenIterator);
            }

            // Check if the current IP:port combination has already been
            // processed
            if (processedEndpoints.find(std::make_pair(ip, port)) ==
                processedEndpoints.end())
            {
                // Add the current IP:port combination to the set of processed
                // endpoints
                processedEndpoints.insert(std::make_pair(ip, port));

                // Initialize a new socket
                this->_initializeServerSocket(ip, port, maxConnections);
            }
        }
    }
    this->_logger.log(VERBOSE, "... finished Server initialization");
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

    // Bind server socket to port
    if (this->_socket.bind(serverSocketDescriptor, ip, port) < 0)
        throw SocketBindError();

    // Listen for incoming connections
    if (this->_socket.listen(serverSocketDescriptor, maxConnections) < 0)
        throw SocketListenError();

    // Set server socket to non-blocking mode
    if (this->_socket.setNonBlocking(serverSocketDescriptor) < 0)
        throw SocketSetError();

    // Add server socket to polling list
    pollfd pollfd;
    pollfd.fd = serverSocketDescriptor;
    pollfd.events = POLLIN | POLLERR | POLLHUP | POLLNVAL;
    pollfd.revents = 0;
    this->_pollfdManager.addServerSocketPollfd(pollfd);

    // Log server socket initialization
    this->_logger.log(INFO, "Server socket initialized. Listening on " +
                                (ip ? Converter::toString(ip) : "ALL") + ":" +
                                Converter::toString(port));
}

/* Terminate server*/
void Server::terminate(int exitCode) { exit(exitCode); }

/* Accept a new client connection*/
void Server::acceptConnection(int serverSocketDescriptor)
{
    // Ensure maximum connections limit has not been reached
    if (this->_pollfdManager.hasReachedCapacity())
        throw MaximumConnectionsReachedError();

    // Accept incoming connection
    std::pair<int, std::pair<std::string, std::string> > clientInfo =
        this->_socket.accept(serverSocketDescriptor);
    int clientSocketDescriptor = clientInfo.first;
    std::string clientIP = clientInfo.second.first;
    std::string clientPort = clientInfo.second.second;

    // Create a connection for the client
    this->_connectionManager.addConnection(clientInfo);

    // Add client socket to polling list
    if (clientSocketDescriptor < 0)
        throw ConnectionEstablishingError();
    pollfd pollfd;
    pollfd.fd = clientSocketDescriptor;
    pollfd.events = POLLIN | POLLERR | POLLHUP | POLLNVAL;
    pollfd.revents = 0;
    this->_pollfdManager.addClientSocketPollfd(pollfd);

    // Set socket to non-blocking mode
    if (this->_socket.setNonBlocking(clientSocketDescriptor) < 0)
        throw SocketSetError();

    // Log accepted connection
    this->_logger.log(VERBOSE, "Accepted new connection from " + clientIP +
                                   ":" + clientPort + ".");
}
