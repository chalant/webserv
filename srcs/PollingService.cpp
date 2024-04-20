#include "../includes/PollingService.hpp"

PollingService::PollingService(IPollfdManager &pollfdManager, int timeout)
    : _pollfdManager(pollfdManager),
      _timeout(timeout) {}

PollingService::~PollingService() {}

void PollingService::pollEvents()
{
    // Get the pollfd array
    pollfd *pollfdArray = this->_pollfdManager.getPollfdArray();

    // Get pollfd queue size
    size_t pollfdQueueSize = this->_pollfdManager.getPollfdQueueSize();

    // Poll registered file descriptors for events
    if (::poll(pollfdArray, pollfdQueueSize, this->_timeout) < 0)
        throw PollError();
}

void PollingService::setPollingTimeout(int timeout)
{
    this->_timeout = timeout;
}