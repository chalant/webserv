#ifndef MOCKPOLLFDMANAGER_HPP
#define MOCKPOLLFDMANAGER_HPP

/*
 * MockPollfdManager class is a mock class for testing purposes.
 * This class is used to simulate the behavior of the PollfdManager class
 * during unit testing without relying on the actual implementation.
 */

#include "../../includes/pollfd/IPollfdManager.hpp"
#include <vector> // std::vector
#include <unistd.h> // size_t

class MockPollfdManager : public IPollfdManager
{
private:
    // Mock pollfd queue
    std::vector<pollfd> _pollfdQueue; // for testing purposes

public:
    // Constructor
    MockPollfdManager();
    virtual ~MockPollfdManager();
    virtual void addRegularFilePollfd(pollfd pollFd);
    virtual void addServerSocketPollfd(pollfd pollFd);
    virtual void addClientSocketPollfd(pollfd pollFd);
    virtual void addPipePollfd(pollfd pollFd);
    virtual void removePollfd(int position);
    virtual void addPollOut(int position);
    virtual void closeAllFileDescriptors();
    virtual size_t getPollfdQueueSize() const;
    virtual short getEvents(int position);
    virtual int getDescriptor(int position);
    virtual bool hasReachedCapacity() const;
    virtual pollfd *getPollfdArray();
};

#endif // MOCKPOLLFDMANAGER_HPP
// Path: tests/mock_includes/MockPollfdManager.hpp
