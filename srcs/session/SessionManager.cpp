#include "session/SessionManager.hpp"

SessionManager::SessionManager(ILogger &logger, IFactory &factory)
    : _factory(factory),
      _logger(logger) {}

void SessionManager::addSession(std::pair<int, std::pair<std::string, std::string>> clientInfo)
{
    ISession *session = this->_factory.createSession(clientInfo);
    _sessions[clientInfo.first] = session;

    this->_logger.log(VERBOSE, "New session created. Remote address: " + clientInfo.second.first + ":" + clientInfo.second.second + " Socket: " + std::to_string(clientInfo.first));
}

void SessionManager::removeSession(int socketDescriptor)
{
    delete this->_sessions[socketDescriptor];
    this->_sessions.erase(socketDescriptor);
}

ISession &SessionManager::getSession(int socketDescriptor)
{
    return *this->_sessions[socketDescriptor];
}

IRequest &SessionManager::getRequest(int socketDescriptor)
{
    return this->_sessions[socketDescriptor]->getRequest();
}

IResponse &SessionManager::getResponse(int socketDescriptor)
{
    return this->_sessions[socketDescriptor]->getResponse();
}

// path: srcs/SessionManager.cpp
