#include "../includes/PollfdManager.hpp"
#include "../includes/Server.hpp"

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
PollfdManager::PollfdManager(const IConfiguration* configuration, const ILogger* errorLogger, const ILogger* accessLogger, const IServer* server)
    : _pollfds(configuration ? configuration->getMaxConnections() + 3 : 0) // + 3 for server socket, error log, and access log
{
    // Obtain necessary descriptors from dependencies
    int serverSocketDescriptor = server->getServerSocketDescriptor();
    int errorLogFileDescriptor = errorLogger->getLogFileDescriptor();
    int accessLogFileDescriptor = accessLogger->getLogFileDescriptor();

    // Make pollfds entries for server socket, error log, and access log
    this->_pollfds[SERVER_SOCKET_POLL_FD] = {serverSocketDescriptor, POLLIN, 0};
    this->_pollfds[ERROR_LOG_POLL_FD] = {errorLogFileDescriptor, POLLOUT, 0};
    this->_pollfds[ACCESS_LOG_POLL_FD] = {accessLogFileDescriptor, POLLOUT, 0};
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

// Method to get the server socket events
int PollfdManager::getServersocketEvents()
{
    return this->_pollfds[SERVER_SOCKET_POLL_FD].revents;
}

// Method to get the error log events
int PollfdManager::getErrorLogEvents()
{
    return this->_pollfds[ERROR_LOG_POLL_FD].revents;
}

// Method to get the access log events
int PollfdManager::getAccessLogEvents()
{
    return this->_pollfds[ACCESS_LOG_POLL_FD].revents;
}

// Method to get the file descriptor at a specific position in the PollfdQueue
int PollfdManager::getFd(int position)
{
    return this->_pollfds[position].fd;
}

// Method to get the error log file descriptor
int PollfdManager::getErrorLogFd()
{
    return this->_pollfds[ERROR_LOG_POLL_FD].fd;
}

// Method to get the access log file descriptor
int PollfdManager::getAccessLogFd()
{
    return this->_pollfds[ACCESS_LOG_POLL_FD].fd;
}

// Method to check if the PollfdQueue has reached its capacity
bool PollfdManager::hasReachedCapacity() const
{
    return this->_pollfds.size() == this->_pollfds.capacity();
}

// Method to get the start index of the client sockets in PollfdQueue
int PollfdManager::getClientsIndex()
{
    return FIRST_CLIENT_POLL_FD;
}

// Method to get a pointer to the pollfd array
pollfd *PollfdManager::getPollfdArray()
{
    return this->_pollfds.data();
}

// Path: srcs/PollfdManager.cpp