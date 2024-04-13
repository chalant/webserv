#ifndef POLLFDMANAGER_HPP
#define POLLFDMANAGER_HPP

/*
 * PollfdManager.hpp
 *
 * This file defines the PollfdManager class, which is responsible for managing
 * poll file descriptors used for asynchronous I/O operations in webserv.
 * *
 * It provides methods for adding, accessing, and manipulating poll file descriptors,
 * as well as managing the pollfd queue size and checking for capacity limits.
 *
 */

#include "PollfdQueue.hpp" // Include the header file for PollfdQueue

// Enumeration for fixed positions in the PollfdQueue
enum PollfdQueueFixedPositions
{
    SERVER_SOCKET_POLL_FD = 0, // The server socket file descriptor position
    ERROR_LOG_POLL_FD = 1,     // The error log file descriptor position
    ACCESS_LOG_POLL_FD = 2,    // The access log file descriptor position
    FIRST_CLIENT_POLL_FD = 3   // The first client file descriptor position
};

// Class for managing poll file descriptors
class PollfdManager
{
private:
    PollfdQueue _pollfds; // Queue for storing polling file descriptors

public:
    // Constructor for PollfdManager class
    PollfdManager(size_t maxConnections);

    // Destructor for PollfdManager class
    ~PollfdManager();

    // Method to add a polling file descriptor
    void addPollfd(pollfd pollFd);

    // Method to remove a polling file descriptor
    void removePollfd(int position);

    // Method to add the POLLOUT event for a specific position in the PollfdQueue
    void addPollOut(int position);

    // Method to close all file descriptors in the PollfdQueue
    void closeAllFileDescriptors();

    // Method to get the size of the PollfdQueue
    size_t getPollfdQueueSize() const;

    // Method to get the events at a specific position in the PollfdQueue
    int getEvents(int position);

    // Method to get the file descriptor at a specific position in the PollfdQueue
    int getFd(int position);

    // Method to check if the PollfdQueue has reached its capacity
    bool hasReachedCapacity() const;

    // Setter method for server socket poll file descriptor
    void setServerSocketPollFd(pollfd serverSocketPollFd);

    // Setter method for error log file poll file descriptor
    void setErrorLogFilePollFd(pollfd logFilePollFd);

    // Setter method for access log file poll file descriptor
    void setAccessLogFilePollFd(pollfd logFilePollFd);

    //Method to get a reference to a specific pollfd
    pollfd &getPollfd(int position);

    // Method to get a reference to the PollfdQueue
    PollfdQueue &getPollfdQueue();
};

#endif // POLLFDMANAGER_HPP
       // Path: includes/PollFdManager.hpp