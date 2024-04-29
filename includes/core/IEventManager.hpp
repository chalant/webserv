#ifndef IEVENTMANAGER_HPP
#define IEVENTMANAGER_HPP

class IEventManager
{
public:
    virtual ~IEventManager() {}

    virtual void handleEvents() = 0;
};

#endif // IEVENTMANAGER_HPP
// Path: includes/core/IEventManager.hpp
