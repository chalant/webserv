#ifndef POLLINGSERVICE_HPP
#define POLLINGSERVICE_HPP

#include "IPollingService.hpp"
#include <poll.h>
#include "../pollfd/IPollfdManager.hpp"
#include "../logger/ILogger.hpp"

class PollingService : public IPollingService
{
private:
    IPollfdManager &_pollfdManager;
    ILogger &_logger;
    int _timeout;

public:
    PollingService(IPollfdManager &pollfdManager, ILogger &logger, int timeout = 100);
    ~PollingService();

    virtual void pollEvents();
    virtual void setPollingTimeout(int timeout);
};

#endif // POLLINGSERVICE_HPP
// Path: includes/core/PollingService.hpp
