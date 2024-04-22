#ifndef SESSIONMANAGER_HPP
#define SESSIONMANAGER_HPP

#include "ISessionManager.hpp"
#include "ISession.hpp"
#include "IConfiguration.hpp"
#include "ILogger.hpp"
#include "IFactory.hpp"
#include "constants/HttpHelper.hpp"
#include <map>

class SessionManager : public ISessionManager
{
private:
    std::map<int, ISession *> _sessions;
    IFactory &_factory;
    ILogger &_logger;

public:
    SessionManager(ILogger &logger, IFactory &factory);
    virtual void addSession(std::pair<int, std::pair<std::string, std::string>> clientInfo);
    virtual void removeSession(int socketDescriptor);
    virtual ISession &getSession(int socketDescriptor);
    virtual IRequest &getRequest(int socketDescriptor);
    virtual IResponse &getResponse(int socketDescriptor);
};

#endif // SESSIONMANAGER_HPP
// Path: includes/SessionManager.hpp
