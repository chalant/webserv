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
Server::Server(ISocket &socket, PollfdManager &pollfdManager, const IConfiguration &configuration, ILogger &errorLogger, ILogger &accessLogger, IExceptionHandler &exceptionHandler)
    : _socket(socket),
      _pollfdManager(pollfdManager),
      _configuration(configuration),
      _errorLogger(errorLogger),
      _accessLogger(accessLogger),
      _exceptionHandler(exceptionHandler),
      _pollMask(POLLIN | POLLERR | POLLHUP | POLLNVAL)
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

    // Add server socket to polling list
    this->_pollfdManager.setServerSocketPollFd({server_fd, this->_pollMask, 0});

    // Link server instance to exception handler
    exceptionHandler.linkServer(this);

    this->_errorLogger.errorLog(INFO, "Server initialized. Listening on port " + std::to_string(_configuration.getPort()) + ".");
}

/* Poll events on server socket */
void Server::pollEvents()
{
    // Get the pollfd array
    pollfd &pollfdArray = this->_pollfdManager.getPollfd(SERVER_SOCKET_POLL_FD);

    // Get pollfd queue size
    size_t pollfdQueueSize = this->_pollfdManager.getPollfdQueueSize();

    // Poll events on server socket
    if (poll(&pollfdArray, pollfdQueueSize, 100) < 0)
        throw PollError();
}

/*Accept incoming connections*/
void Server::acceptConnection()
{
    // Get Server Socket events
    int serverSocketEvents = this->_pollfdManager.getEvents(SERVER_SOCKET_POLL_FD);

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
        if (this->_pollfdManager.hasReachedCapacity())
            throw MaximumConnectionsReachedError();

        // Accept incoming connection
        int serverSocketFd = this->_pollfdManager.getFd(SERVER_SOCKET_POLL_FD);
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int clientFd = accept(serverSocketFd, (struct sockaddr *)&client_addr, &client_addr_len);

        // Add client socket to polling list
        if (clientFd < 0)
            throw ConnectionEstablishingError();
        this->_pollfdManager.addPollfd({clientFd, this->_pollMask, 0});

        // Set socket to non-blocking mode
        if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0)
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

/* Destructor to close file descriptors*/
Server::~Server()
{
    // Close all socket file descriptors
    this->_pollfdManager.closeAllFileDescriptors();
}

/* Terminate server*/
void Server::terminate(int exit_code)
{
    exit(exit_code);
}
