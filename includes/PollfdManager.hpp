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
 * NOTE - pollfds referring to file descriptors, server sockets and client sockets are stored sequentially in the PollfdQueue,
 *          in this order: file descriptors, server sockets, client sockets.
 */

#include "IPollfdManager.hpp" // Include the header file for IPollfdManager
#include "PollfdQueue.hpp"    // Include the header file for PollfdQueue
#include "IConfiguration.hpp"
#include "ILogger.hpp"

// Forward declaration of Server class
class IServer;

// Class for managing poll file descriptors
class PollfdManager : public IPollfdManager
{
private:
    PollfdQueue _pollfds;      // Queue for storing polling file descriptors
    ssize_t _fileDescriptorsIndex; // Index for the access log file descriptor in the PollfdQueue
    ssize_t _serverSocketsIndex;   // Index for the server sockets in the PollfdQueue
    ssize_t _clientSocketsIndex;   // Index for the start of client sockets in the PollfdQueue

public:
    // Constructor for PollfdManager class
    PollfdManager(const IConfiguration &configuration);

    // Destructor for PollfdManager class
    ~PollfdManager();

    // Method to add a polling file descriptor
    virtual void addPollfd(pollfd pollFd);

    // Method to add a file descriptor pollfd to the pollfdQueue
    virtual void addFileDescriptorPollfd(pollfd pollFd);

    // Method to add a server socket pollfd to the pollfdQueue
    virtual void addServerSocketPollfd(pollfd pollFd);

    // Method to add a client socket pollfd to the pollfdQueue
    virtual void addClientSocketPollfd(pollfd pollFd);

    // Method to remove a polling file descriptor
    virtual void removePollfd(int position);

    // Method to add the POLLOUT event for a specific position in the PollfdQueue
    virtual void addPollOut(int position);

    // Method to close all file descriptors in the PollfdQueue
    virtual void closeAllFileDescriptors();

    // Method to get the size of the PollfdQueue
    virtual size_t getPollfdQueueSize() const;

    // Method to get the events at a specific position in the PollfdQueue
    virtual short getEvents(int position);

    // Method to get the file descriptor at a specific position in the PollfdQueue
    virtual int getDescriptor(int position);

    // Method to get the first file descriptor index
    virtual ssize_t getFileDescriptorsIndex();

    // Method to get the first server socket index
    virtual ssize_t getServerSocketsIndex();

    // Method to get the first client socket index
    virtual ssize_t getClientSocketsIndex();

    // Method to check if the PollfdQueue has reached its capacity
    virtual bool hasReachedCapacity() const;

    // Method to get a pointer to the pollfd array
    virtual pollfd *getPollfdArray();
};

#endif // POLLFDMANAGER_HPP
       // Path: includes/PollFdManager.hpp