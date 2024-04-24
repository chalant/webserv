#ifndef IPOLLINGSERVICE_HPP
#define IPOLLINGSERVICE_HPP

class IPollingService
{
public:
    virtual ~IPollingService() = default;

    virtual void pollEvents() = 0;
    virtual void setPollingTimeout(int) = 0;
};

#endif // IPOLLINGSERVICE_HPP
// Path: includes/core/IPollingService.hpp
