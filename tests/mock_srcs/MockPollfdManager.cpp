#include "../mock_includes/MockPollfdManager.hpp"

/*
 * MockPollfdManager class is a mock class for testing purposes.
 * This class is used to simulate the behavior of the PollfdManager class
 * during unit testing without relying on the actual implementation.
 */

MockPollfdManager::MockPollfdManager() {}

MockPollfdManager::~MockPollfdManager() {}

void MockPollfdManager::addRegularFilePollfd(pollfd pollfd)
{
    m_pollfd_queue.push_back(pollfd);
    m_pollfd_queue.back().revents |= REGULAR_FILE;
}

void MockPollfdManager::addServerSocketPollfd(pollfd pollfd)
{
    static_cast<void>(pollfd);
}

void MockPollfdManager::addClientSocketPollfd(pollfd pollfd)
{
    static_cast<void>(pollfd);
}

void MockPollfdManager::addPipePollfd(pollfd pollfd)
{
    static_cast<void>(pollfd);
}

void MockPollfdManager::removePollfd(int position)
{
    m_pollfd_queue.erase(m_pollfd_queue.begin() + position);
}

void MockPollfdManager::addPollOut(int position)
{
    static_cast<void>(position);
}

void MockPollfdManager::closeAllFileDescriptors() {}

size_t MockPollfdManager::getPollfdQueueSize() const
{
    return m_pollfd_queue.size();
}

short MockPollfdManager::getEvents(int position)
{
    return m_pollfd_queue[ position ].revents;
}

int MockPollfdManager::getDescriptor(int position)
{
    return m_pollfd_queue[ position ].fd;
}

bool MockPollfdManager::hasReachedCapacity() const { return false; }

pollfd *MockPollfdManager::getPollfdArray() { return NULL; }

int MockPollfdManager::getPollfdQueueIndex(int fd) { 
    static_cast<void>(fd);
    return 0; }

// Path: tests/mock_srcs/MockPollfdManager.cpp
