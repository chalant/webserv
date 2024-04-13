#include "../includes/PollfdManager.hpp"

/*
 * PollfdManager.hpp
 *
 * This file implements the PollfdManager class, which is responsible for managing
 * poll file descriptors used for asynchronous I/O operations in webserv.
 * *
 * It provides methods for adding, accessing, and manipulating poll file descriptors,
 * as well as managing the pollfd queue size and checking for capacity limits.
 *
 */

// Constructor for PollFdManager class
PollfdManager::PollfdManager(size_t maxConnections)
    : _pollfds(maxConnections + 3) // + 3 for server socket, error log, and access log
{
    // Zero-initialize reserved positions in the PollfdQueue
    this->_pollfds[SERVER_SOCKET_POLL_FD] = {-1, 0, 0};
    this->_pollfds[ERROR_LOG_POLL_FD] = {-1, 0, 0};
    this->_pollfds[ACCESS_LOG_POLL_FD] = {-1, 0, 0};
}

// Destructor for PollFdManager class
PollfdManager::~PollfdManager() {}

// Method to add a polling file descriptor
void PollfdManager::addPollfd(pollfd pollFd)
{
    this->_pollfds.push(pollFd);
}

// Method to remove a polling file descriptor
void PollfdManager::removePollfd(int position)
{
    this->_pollfds.erase(position);
}

// Method to add the POLLOUT event for a specific position in the PollfdQueue
void PollfdManager::addPollOut(int position)
{
    this->_pollfds.pollout(position);
}

// Method to close all file descriptors in the PollfdQueue
void PollfdManager::closeAllFileDescriptors()
{
    for (size_t i = 0; i < this->_pollfds.size(); i++)
    {
        if (this->_pollfds[i].fd != -1)
            close(this->_pollfds[i].fd);
    }
}

// Method to get the size of the PollfdQueue
size_t PollfdManager::getPollfdQueueSize() const
{
    return this->_pollfds.size();
}

// Method to get the events at a specific position in the PollfdQueue
int PollfdManager::getEvents(int position)
{
    return this->_pollfds[position].events;
}

// Method to get the file descriptor at a specific position in the PollfdQueue
int PollfdManager::getFd(int position)
{
    return this->_pollfds[position].fd;
}

// Method to check if the PollfdQueue has reached its capacity
bool PollfdManager::hasReachedCapacity() const
{
    return this->_pollfds.size() == this->_pollfds.capacity();
}

// Setter method for server socket poll file descriptor
void PollfdManager::setServerSocketPollFd(pollfd serverSocketPollFd)
{
    this->_pollfds[SERVER_SOCKET_POLL_FD] = serverSocketPollFd;
}

// Setter method for error log file poll file descriptor
void PollfdManager::setErrorLogFilePollFd(pollfd logFilePollFd)
{
    this->_pollfds[ERROR_LOG_POLL_FD] = logFilePollFd;
}

// Setter method for access log file poll file descriptor
void PollfdManager::setAccessLogFilePollFd(pollfd logFilePollFd)
{
    this->_pollfds[ACCESS_LOG_POLL_FD] = logFilePollFd;
}

// Method to get a reference to a specific pollfd
pollfd &PollfdManager::getPollfd(int position)
{
    return this->_pollfds[position];
}

// Method to get a reference to the PollfdQueue
PollfdQueue &PollfdManager::getPollfdQueue()
{
    return this->_pollfds;
}

// Path: srcs/PollfdManager.cpp