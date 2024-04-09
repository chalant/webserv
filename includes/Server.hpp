#ifndef SERVER_HPP
#define SERVER_HPP

/*
 * The Server class is responsible for managing core operations of webserv, including initialization, connection handling, and termination.
 *
 * It oversees socket creation, binding, and listening for incoming connections, all configured for non-blocking operation.
 * Leveraging the poll() polling mechanism, Server monitors events on the server socket, log file descriptors, and client connections,
 * facilitating the acceptance of incoming connections and their inclusion in the monitoring queue.
 *
 */

#include <vector>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include "constants/PollfdQueueFixedPositions.hpp"
#include "Configuration.hpp"
#include "Logger.hpp"
#include "ExceptionHandler.hpp"
#include "PollfdQueue.hpp"

class Server
{
private:
    PollfdQueue _pollFds;                // Queue for storing polling file descriptors
    Configuration &_configuration;       // Reference to the server configuration settings
    Logger &_errorLogger;                // Reference to the error logger
    Logger &_accessLogger;               // Reference to the access logger
    ExceptionHandler &_exceptionHandler; // Reference to the exception handler
    size_t _maxConnections;              // Maximum allowed connections
    short _pollMask;                     // Poll mask for polling events

public:
    Server(Configuration &configuration,
           Logger &errorLogger, Logger &accessLogger,
           ExceptionHandler &exceptionHandler); // Constructor for Server class
    const Server &operator=(const Server &src); // Assignment operator overload
    ~Server();                                  // Destructor for Server class
    void addpollfd(pollfd pollFd);              // Method to add a polling file descriptor
    PollfdQueue &getPollfdQueue();      // Method to get a reference to the PollfdQueue
    void acceptConnection();                    // Method to accept incoming connections
    void pollEvents();                          // Method to poll events
    void terminate(int exit_code);              // Method to terminate the server
                                                // Closes file descriptors, clears memory, writes log buffers to file, and exits
};

#endif // SERVER_HPP

// Path: includes/Server.hpp
