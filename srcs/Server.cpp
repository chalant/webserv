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
Server::Server(Configuration &configuration, Logger &errorLogger, Logger &accessLogger, ExceptionHandler &exceptionHandler)
    : _pollFds(configuration.getMaxConnections() + 3), _configuration(configuration), _errorLogger(errorLogger), _accessLogger(accessLogger), _exceptionHandler(exceptionHandler), _maxConnections(configuration.getMaxConnections())
{
    // Create server socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        throw SocketCreateError();
    this->_errorLogger.errorLog(DEBUG, "Server socket created.");

    // Bind server socket to port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(_configuration.getPort());
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        throw SocketBindError();
    this->_errorLogger.errorLog(DEBUG, "Server socket bound to port " + std::to_string(_configuration.getPort()));

    // Listen for incoming connections
    if (listen(server_fd, 10) < 0)
        throw SocketListenError();
    this->_errorLogger.errorLog(DEBUG, "Server socket listening.");

    // Set server socket to non-blocking mode
    if (fcntl(server_fd, F_SETFL, O_NONBLOCK) < 0)
        throw SocketSetError();
    this->_errorLogger.errorLog(DEBUG, "Server socket set to non-blocking.");

    // Set polling mask for server socket
    this->_pollMask = POLLIN | POLLERR | POLLHUP | POLLNVAL;

    // Add server socket to polling list
    this->_pollFds.push({server_fd, this->_pollMask, 0});

    // Add error logger file descriptor to polling list
    this->_pollFds.push({this->_errorLogger.getLogFileDescriptor(), POLLOUT, 0});

    // Add access logger file descriptor to polling list
    this->_pollFds.push({this->_errorLogger.getLogFileDescriptor(), POLLOUT, 0});

    // Set polling file descriptors for loggers
    errorLogger.setLogFilePollFd(&this->_pollFds[ERROR_LOG_POLL_FD]);
    accessLogger.setLogFilePollFd(&this->_pollFds[ACCESS_LOG_POLL_FD]);

    this->_errorLogger.errorLog(INFO, "Server initialized. Listening on port " + std::to_string(_configuration.getPort()) + ".");
}

/* Poll events on file descriptors*/
void Server::pollEvents()
{
    if (poll(&this->_pollFds[0], this->_pollFds.size(), 100) < 0)
        throw PollError();
}

/*Accept incoming connections*/
void Server::acceptConnection()
{
    // If server socket has events return immediately
    if (this->_pollFds[0].revents == 0)
        return;

    // Check for errors on server socket
    if (this->_pollFds[0].revents & POLLERR)
        throw ServerSocketError();

    // Check for server socket closed
    if (this->_pollFds[0].revents & POLLHUP)
        throw ServerSocketClosedError();
    
    // Check for invalid request on server socket
    if (this->_pollFds[0].revents & POLLNVAL)
        throw ServerSocketInvalidError();

    // Check if server socket is ready to accept incoming connections
    if (this->_pollFds[0].revents & POLLIN)
    {
        // Ensure maximum connections limit has not been reached
        if (this->_pollFds.hasReachedCapacity())
            throw MaximumConnectionsReachedError();

        // Accept incoming connection
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = accept(this->_pollFds[0].fd, (struct sockaddr *)&client_addr, &client_addr_len);

        // Add client socket to polling list
        if (client_fd < 0)
            throw ConnectionEstablishingError();
        this->_pollFds.push({client_fd, this->_pollMask, 0});

        // Set socket to non-blocking mode
        if (fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0)
            throw SocketSetError();

        // Log accepted connection
        this->_errorLogger.errorLog(VERBOSE, "Accepted new connection.");
    }
    else
    {
        // Log no incoming connection
        this->_errorLogger.errorLog(VERBOSE, "No incoming connection.");
    }
}

/* Return a reference to the PollfdQueue object*/
PollfdQueue &Server::getPollfdQueue()
{
    return this->_pollFds;
}

/* Destructor to close file descriptors*/
Server::~Server()
{
    // Close server socket
    close(this->_pollFds[0].fd);

    // Close client sockets
    for (size_t i = 1; i < this->_pollFds.size(); ++i)
    {
        close(this->_pollFds[i].fd);
    }
}

/* Terminate server*/
void Server::terminate(int exit_code)
{
    this->_errorLogger.writeLogBufferToFileBlocking();
    this->_accessLogger.writeLogBufferToFileBlocking();
    exit(exit_code);
}
