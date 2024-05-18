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

#include <map>                                // Include the map library
#include "IPollfdManager.hpp"                  // Include the header file for IPollfdManager
#include "PollfdQueue.hpp"                     // Include the header file for PollfdQueue
#include "../configuration/IConfiguration.hpp" // Include the header file for IConfiguration
#include "../configuration/BlockList.hpp"

// Forward declaration of Server class
class IServer;

// Class for managing poll file descriptors
class PollfdManager : public IPollfdManager
{
private:
    PollfdQueue _pollfds;                             // Queue for storing polling file descriptors
    std::map<int, DescriptorType> _descriptorTypeMap; // Map for storing the type of descriptor

    // Method to add a polling file descriptor
    virtual void _addPollfd(pollfd pollFd);

public:
    // Constructor for PollfdManager class
    PollfdManager(const IConfiguration &configuration);

    // Destructor for PollfdManager class
    ~PollfdManager();

    // Method to add a regular file pollfd to the pollfdQueue
    virtual void addRegularFilePollfd(pollfd pollFd);

    // Method to add a server socket pollfd to the pollfdQueue
    virtual void addServerSocketPollfd(pollfd pollFd);

    // Method to add a client socket pollfd to the pollfdQueue
    virtual void addClientSocketPollfd(pollfd pollFd);

    // Method to add a pipe pollfd to the pollfdQueue
    virtual void addPipePollfd(pollfd pollFd);

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

    // Method to check if the PollfdQueue has reached its capacity
    virtual bool hasReachedCapacity() const;

    // Method to get a pointer to the pollfd array
    virtual pollfd *getPollfdArray();
};

#endif // POLLFDMANAGER_HPP
       // Path: includes/PollFdManager.hpp