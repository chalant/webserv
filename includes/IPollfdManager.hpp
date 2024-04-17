#ifndef IPOLLFDMANAGER_HPP
#define IPOLLFDMANAGER_HPP

/*
 * IPollfdManager.hpp
 * Abstract base class for PollfdManager in webserv
 *
 * This file defines the PollfdManager interface, which serves as the abstract base
 * class for the PollfdManager in webserv.
 * IPollfdManager allows for polymorphic behavior and dependency injection,
 * thereby enabling us to create a MockPollfdManager class for isolated unit testing.
 *
 */

#include <poll.h>

class IPollfdManager
{
public:
    virtual ~IPollfdManager() {}

    // Method to add a file descriptor to the pollfdQueue
    virtual void addPollfd(pollfd pollFd) = 0;

    // Method to remove a file descriptor from the pollfdQueue
    virtual void removePollfd(int position) = 0;

    // Method to add the POLLOUT event for a specific position in the pollfdQueue
    virtual void addPollOut(int position) = 0;

    // Method to close all file descriptors in the pollfdQueue
    virtual void closeAllFileDescriptors() = 0;

    // Method to get the size of the pollfdQueue
    virtual size_t getPollfdQueueSize() const = 0;

    // Method to get the events at a specific position in the pollfdQueue
    virtual int getEvents(int position) = 0;

    // Method to get the server socket events
    virtual int getServersocketEvents() = 0;

    // Method to get the error log events
    virtual int getErrorLogEvents() = 0;

    // Method to get the access log events
    virtual int getAccessLogEvents() = 0;

    // Method to get the file descriptor at a specific position in the pollfdQueue
    virtual int getFd(int position) = 0;

    // Method to get the error log file descriptor
    virtual int getErrorLogFd() = 0;

    // Method to get the access log file descriptor
    virtual int getAccessLogFd() = 0;
    
    // Method to check if the pollfdQueue has reached its capacity
    virtual bool hasReachedCapacity() const = 0;

    // Method to get the start index of the client sockets in pollfdQueue
    virtual int getClientsIndex() = 0;

    // Method to get a pointer to the pollfd array
    virtual pollfd *getPollfdArray() = 0;
};

#endif // IPOLLFDMANAGER_HPP
// Path: includes/IPollfdManager.hpp
