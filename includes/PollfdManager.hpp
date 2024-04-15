#ifndef POLLFDMANAGER_HPP
#define POLLFDMANAGER_HPP

/*
 * PollfdManager.hpp
 *
 * This file defines the PollfdManager class, which is responsible for holding and managing
 * poll file descriptors used for asynchronous I/O operations in webserv.
 * *
 * It provides methods for adding, accessing, and manipulating poll file descriptors,
 * as well as managing the pollfd queue size and checking for capacity limits.
 *
 */

#include "IPollfdManager.hpp" // Include the header file for IPollfdManager
#include "PollfdQueue.hpp" // Include the header file for PollfdQueue
#include "IConfiguration.hpp"
#include "ILogger.hpp"

// Forward declaration of Server class
class Server;

// Enumeration for fixed positions in the PollfdQueue
enum PollfdQueueFixedPositions
{
    SERVER_SOCKET_POLL_FD = 0, // The server socket file descriptor position
    ERROR_LOG_POLL_FD = 1,     // The error log file descriptor position
    ACCESS_LOG_POLL_FD = 2,    // The access log file descriptor position
    FIRST_CLIENT_POLL_FD = 3   // The first client file descriptor position
};

// Class for managing poll file descriptors
class PollfdManager : public IPollfdManager
{
private:
    PollfdQueue _pollfds; // Queue for storing polling file descriptors

public:
    // Constructor for PollfdManager class
    PollfdManager(const IConfiguration *configuration, const ILogger *errorLogger, const ILogger *accessLogger, const Server *server);

    // Destructor for PollfdManager class
    ~PollfdManager();

    // Method to add a polling file descriptor
    virtual void addPollfd(pollfd pollFd);

    // Method to remove a polling file descriptor
    virtual void removePollfd(int position);

    // Method to add the POLLOUT event for a specific position in the PollfdQueue
    virtual void addPollOut(int position);

    // Method to close all file descriptors in the PollfdQueue
    virtual void closeAllFileDescriptors();

    // Method to get the size of the PollfdQueue
    virtual size_t getPollfdQueueSize() const;

    // Method to get the events at a specific position in the PollfdQueue
    virtual int getEvents(int position);

    // Method to get the file descriptor at a specific position in the PollfdQueue
    virtual int getFd(int position);

    // Method to check if the PollfdQueue has reached its capacity
    virtual bool hasReachedCapacity() const;

    // Method to get a pointer to the pollfd array
    pollfd *getPollfdArray();
};

#endif // POLLFDMANAGER_HPP
       // Path: includes/PollFdManager.hpp