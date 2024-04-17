#ifndef POLLINGSERVICE_HPP
#define POLLINGSERVICE_HPP

#include "IPollingService.hpp"
#include "IPollfdManager.hpp"
#include "WebservExceptions.hpp"
#include <stddef.h>
#include <poll.h>

class PollingService : public IPollingService
{
private:
    IPollfdManager *_pollfdManager;
    int _timeout;
public:
    PollingService(IPollfdManager *pollfdManager, int timeout=100);
    ~PollingService();

    virtual void pollEvents();
    virtual void setPollingTimeout(int timeout);
};

#endif // POLLINGSERVICE_HPP
// Path: includes/PollingService.hpp
