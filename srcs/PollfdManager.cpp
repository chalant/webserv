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
    : _pollfds(configuration.getInt("MaxConnections") + 3) // + 3 for server socket, error log, and access log, or more in case of several server sockets
{
}

// Destructor for PollFdManager class
PollfdManager::~PollfdManager() {}

// Method to add a polling file descriptor
void PollfdManager::_addPollfd(pollfd pollFd)
{
    this->_pollfds.push(pollFd);
}

// Method to add a regular file pollfd to the pollfdQueue
void PollfdManager::addRegularFilePollfd(pollfd pollFd)
{
    if (this->_descriptorTypeMap.find(pollFd.fd) != this->_descriptorTypeMap.end())
        return; // Flush is already pending
    this->_descriptorTypeMap[pollFd.fd] = REGULAR_FILE;
    this->_addPollfd(pollFd);
}

// Method to add a server socket pollfd to the pollfdQueue
void PollfdManager::addServerSocketPollfd(pollfd pollFd)
{
    this->_descriptorTypeMap[pollFd.fd] = SERVER_SOCKET;
    this->_addPollfd(pollFd);
}

// Method to add a client socket pollfd to the pollfdQueue
void PollfdManager::addClientSocketPollfd(pollfd pollFd)
{
    this->_descriptorTypeMap[pollFd.fd] = CLIENT_SOCKET;
    this->_addPollfd(pollFd);
}

// Method to add a pipe pollfd to the pollfdQueue
void PollfdManager::addPipePollfd(pollfd pollFd)
{
    this->_descriptorTypeMap[pollFd.fd] = PIPE;
    this->_addPollfd(pollFd);
}

// Method to remove a polling file descriptor
void PollfdManager::removePollfd(int position)
{
    this->_descriptorTypeMap.erase(this->_pollfds[position].fd);
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
    short type = this->_descriptorTypeMap[this->_pollfds[position].fd];

    // clear the first 2 bits, then add the type
    return (this->_pollfds[position].events & 0x3F) | type;
}

// Method to get the file descriptor at a specific position in the PollfdQueue
int PollfdManager::getDescriptor(int position)
{
    return this->_pollfds[position].fd;
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