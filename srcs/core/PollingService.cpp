#include "../../includes/core/PollingService.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include "../../includes/utils/Converter.hpp"

PollingService::PollingService(IPollfdManager &pollfd_manager, ILogger &logger,
                               int timeout)
    : m_pollfd_manager(pollfd_manager), m_logger(logger), m_timeout(timeout)
{
}

PollingService::~PollingService() {}

void PollingService::pollEvents()
{
    // Get the pollfd array
    pollfd *pollfd_array = this->m_pollfd_manager.getPollfdArray();

    // Get pollfd queue size
    size_t pollfd_queue_size = this->m_pollfd_manager.getPollfdQueueSize();

    // Poll registered file descriptors for events
    int poll_result = ::poll(pollfd_array, pollfd_queue_size, this->m_timeout);
    if (poll_result < 0)
        throw PollError();

    // Log poll result
    if (poll_result == 0) // Timeout occurred
        this->m_logger.log(
            VERBOSE, "[POLLINGSERVICE] Poll returned after timeout (0 events)");
    else // Events occurred
        this->m_logger.log(VERBOSE, "[POLLINGSERVICE] Poll returned " +
                                       Converter::toString(poll_result) +
                                       " events.");
}

void PollingService::setPollingTimeout(int timeout)
{
    this->m_timeout = timeout;
}
