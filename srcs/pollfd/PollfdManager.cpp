#include "../../includes/pollfd/PollfdManager.hpp"

/*
 * PollfdManager.hpp
 *
 * This file implements the PollfdManager class, which is responsible for
 * managing poll file descriptors used for asynchronous I/O operations in
 * webserv.
 * *
 * It provides methods for adding, accessing, and manipulating poll file
 * descriptors, as well as managing the pollfd queue size and checking for
 * capacity limits.
 *
 */

// Constructor for PollFdManager class
PollfdManager::PollfdManager(IConfiguration &configuration)
    : m_pollfds(
          configuration.getBlocks("events")[ 0 ]->getInt("worker_connections") +
          3) // + 3 for server socket, error log, and access log, or
             // more in case of several server sockets
{
}

// Destructor for PollFdManager class
PollfdManager::~PollfdManager() {}

// Method to add a polling file descriptor
void PollfdManager::m_addPollfd(pollfd pollFd) { this->m_pollfds.push(pollFd); }

// Method to add a regular file pollfd to the pollfdQueue
void PollfdManager::addRegularFilePollfd(pollfd pollFd)
{
    if (this->m_descriptor_type_map.find(pollFd.fd) !=
        this->m_descriptor_type_map.end())
        return; // Flush is already pending
    this->m_descriptor_type_map[ pollFd.fd ] = REGULAR_FILE;
    this->m_addPollfd(pollFd);
}

// Method to add a server socket pollfd to the pollfdQueue
void PollfdManager::addServerSocketPollfd(pollfd pollFd)
{
    this->m_descriptor_type_map[ pollFd.fd ] = SERVER_SOCKET;
    this->m_addPollfd(pollFd);
}

// Method to add a client socket pollfd to the pollfdQueue
void PollfdManager::addClientSocketPollfd(pollfd pollFd)
{
    this->m_descriptor_type_map[ pollFd.fd ] = CLIENT_SOCKET;
    this->m_addPollfd(pollFd);
}

// Method to add a pipe pollfd to the pollfdQueue
void PollfdManager::addPipePollfd(pollfd pollFd)
{
    this->m_descriptor_type_map[ pollFd.fd ] = PIPE;
    this->m_addPollfd(pollFd);
}

// Method to remove a polling file descriptor
void PollfdManager::removePollfd(int position)
{
    this->m_descriptor_type_map.erase(this->m_pollfds[ position ].fd);
    this->m_pollfds.erase(position);
}

// Method to add the POLLOUT event for a specific position in the PollfdQueue
void PollfdManager::addPollOut(int position)
{
    this->m_pollfds.pollout(position);
}

// Method to close all file descriptors in the PollfdQueue
void PollfdManager::closeAllFileDescriptors()
{
    for (size_t i = 0; i < this->m_pollfds.size(); i++)
    {
        if (this->m_pollfds[ i ].fd != -1)
            close(this->m_pollfds[ i ].fd);
    }
}

// Method to get the size of the PollfdQueue
size_t PollfdManager::getPollfdQueueSize() const
{
    return this->m_pollfds.size();
}

// Method to get the events at a specific position in the PollfdQueue
short PollfdManager::getEvents(int position)
{
    short type = this->m_descriptor_type_map[ this->m_pollfds[ position ].fd ];

    // clear unused bits to be sure, then add the type
    return (this->m_pollfds[ position ].revents & 0x3F) | type;
}

// Method to get the file descriptor at a specific position in the PollfdQueue
int PollfdManager::getDescriptor(int position)
{
    return this->m_pollfds[ position ].fd;
}

// Method to check if the PollfdQueue has reached its capacity
bool PollfdManager::hasReachedCapacity() const
{
    return this->m_pollfds.size() == this->m_pollfds.capacity();
}

// Method to get a pointer to the pollfd array
pollfd *PollfdManager::getPollfdArray() { return this->m_pollfds.data(); }

// Path: srcs/PollfdManager.cpp
