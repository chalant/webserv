#ifndef ISESSIONMANAGER_HPP
#define ISESSIONMANAGER_HPP

#include <string>
#include "ISession.hpp"

class ISessionManager
{
public:
    virtual ~ISessionManager() = default;

    virtual void addSession(std::pair<int, std::pair<std::string, std::string>>) = 0;
    virtual void removeSession(int) = 0;
    virtual ISession &getSession(int) = 0;
    virtual IRequest &getRequest(int) = 0;
    virtual IResponse &getResponse(int) = 0;
};

#endif // ISESSIONMANAGER_HPP
// Path: includes/ISessionManager.hpp
