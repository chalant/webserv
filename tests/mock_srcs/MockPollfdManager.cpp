#include "MockPollfdManager.hpp"

/*
 * MockPollfdManager class is a mock class for testing purposes.
 * This class is used to simulate the behavior of the PollfdManager class
 * during unit testing without relying on the actual implementation.
 */

MockPollfdManager::MockPollfdManager() {}

MockPollfdManager::~MockPollfdManager() {}

void MockPollfdManager::addRegularFilePollfd(pollfd pollFd)
{
    this->_pollfdQueue.push_back(pollFd);
    this->_pollfdQueue.back().revents |= REGULAR_FILE;
}

void MockPollfdManager::addServerSocketPollfd(pollfd pollFd) { static_cast<void>(pollFd); }

void MockPollfdManager::addClientSocketPollfd(pollfd pollFd) { static_cast<void>(pollFd); }

void MockPollfdManager::addPipePollfd(pollfd pollFd) { static_cast<void>(pollFd); }

void MockPollfdManager::removePollfd(int position)
{
    this->_pollfdQueue.erase(this->_pollfdQueue.begin() + position);
}

void MockPollfdManager::addPollOut(int position) { static_cast<void>(position); }

void MockPollfdManager::closeAllFileDescriptors() {}

size_t MockPollfdManager::getPollfdQueueSize() const
{
    return this->_pollfdQueue.size();
}

short MockPollfdManager::getEvents(int position)
{
    return this->_pollfdQueue[position].revents;
}

int MockPollfdManager::getDescriptor(int position)
{
    return this->_pollfdQueue[position].fd;
}

bool MockPollfdManager::hasReachedCapacity() const { return false; }

pollfd *MockPollfdManager::getPollfdArray() { return nullptr; }

// Path: tests/mock_srcs/MockPollfdManager.cpp
