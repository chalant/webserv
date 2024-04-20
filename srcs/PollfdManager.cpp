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
PollfdManager::PollfdManager(const IConfiguration &configuration)
    : _pollfds(configuration.getInt("MaxConnections") + 3), // + 3 for server socket, error log, and access log, or more in case of several server sockets
      _fileDescriptorsIndex(-1),                            // Index for log descriptors
      _serverSocketsIndex(-1),                              // Index for server sockets
      _clientSocketsIndex(-1)                               // Index for client sockets
{
}

// Destructor for PollFdManager class
PollfdManager::~PollfdManager() {}

// Method to add a polling file descriptor
void PollfdManager::addPollfd(pollfd pollFd)
{
    this->_pollfds.push(pollFd);
}

// Method to add a file descriptor pollfd to the pollfdQueue
void PollfdManager::addFileDescriptorPollfd(pollfd pollFd)
{
    this->_fileDescriptorsIndex++;
    this->_serverSocketsIndex++;
    this->_clientSocketsIndex++;
    this->addPollfd(pollFd);
}

// Method to add a server socket pollfd to the pollfdQueue
void PollfdManager::addServerSocketPollfd(pollfd pollFd)
{
    this->_serverSocketsIndex++;
    this->_clientSocketsIndex++;
    this->addPollfd(pollFd);
}

// Method to add a client socket pollfd to the pollfdQueue
void PollfdManager::addClientSocketPollfd(pollfd pollFd)
{
    this->_clientSocketsIndex++;
    this->addPollfd(pollFd);
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
short PollfdManager::getEvents(int position)
{
    return this->_pollfds[position].events;
}

// Method to get the file descriptor at a specific position in the PollfdQueue
int PollfdManager::getDescriptor(int position)
{
    return this->_pollfds[position].fd;
}

// Method to get the file descriptors index
ssize_t PollfdManager::getFileDescriptorsIndex()
{
    return this->_fileDescriptorsIndex;
}

// Method to get the server sockets index
ssize_t PollfdManager::getServerSocketsIndex()
{
    return this->_serverSocketsIndex;
}

// Method to get the client sockets index
ssize_t PollfdManager::getClientSocketsIndex()
{
    return this->_clientSocketsIndex;
}

// Method to check if the PollfdQueue has reached its capacity
bool PollfdManager::hasReachedCapacity() const
{
    return this->_pollfds.size() == this->_pollfds.capacity();
}

// Method to get a pointer to the pollfd array
pollfd *PollfdManager::getPollfdArray()
{
    return this->_pollfds.data();
}

// Path: srcs/PollfdManager.cpp