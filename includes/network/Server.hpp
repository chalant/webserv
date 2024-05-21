#ifndef SERVER_HPP
#define SERVER_HPP

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

#include "../configuration/BlockList.hpp"
#include "../configuration/IConfiguration.hpp"
#include "../connection/IConnectionManager.hpp"
#include "../logger/ILogger.hpp"
#include "../pollfd/IPollfdManager.hpp"
#include "IServer.hpp"
#include "ISocket.hpp"

class Server : public IServer
{
private:
    const ISocket &_socket;         // Reference to the Socket instance
    IPollfdManager &_pollfdManager; // Reference to the PollfdManager
    IConnectionManager
        &_connectionManager; // Reference to the ConnectionManager
    ILogger &_logger;        // Reference to the error logger

    void _initializeServerSocket(
        int ip, int port,
        int maxConnections); // Method to initialize the server socket

public:
    Server(const ISocket &_socket, IPollfdManager &pollfdManager,
           IConnectionManager &connectionManager,
           IConfiguration &configuration,
           ILogger &logger); // Constructor for Server class
    ~Server();               // Destructor for Server class

    virtual void
    acceptConnection(int serverSocketDescriptor); // Method to accept a new
                                                  // client connection
    virtual void
    terminate(int exitCode); // Method to terminate the server Closes file
                             // descriptors, clears memory, writes log buffers
                             // to file, and exits
};

#endif // SERVER_HPP

// Path: includes/Server.hpp
