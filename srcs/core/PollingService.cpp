#include "../../includes/core/PollingService.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include "../../includes/utils/Converter.hpp"

PollingService::PollingService(IPollfdManager &pollfdManager, ILogger &logger, int timeout)
    : _pollfdManager(pollfdManager),
      _logger(logger),
      _timeout(timeout) {}

PollingService::~PollingService() {}

void PollingService::pollEvents()
{
    // Get the pollfd array
    pollfd *pollfdArray = this->_pollfdManager.getPollfdArray();

    // Get pollfd queue size
    size_t pollfdQueueSize = this->_pollfdManager.getPollfdQueueSize();

    // Poll registered file descriptors for events
    int pollResult = ::poll(pollfdArray, pollfdQueueSize, this->_timeout);
    if (pollResult < 0)
        throw PollError();

    // Log poll result
    if (pollResult == 0) // Timeout occurred
        this->_logger.log(VERBOSE, "[POLLINGSERVICE] Poll returned after timeout (0 events)");
    else // Events occurred
        this->_logger.log(VERBOSE, "[POLLINGSERVICE] Poll returned " + Converter::toString(pollResult) + " events.");
}

void PollingService::setPollingTimeout(int timeout)
{
    this->_timeout = timeout;
}