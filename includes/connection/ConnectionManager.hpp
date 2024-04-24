#ifndef CONNECTIONMANAGER_HPP
#define CONNECTIONMANAGER_HPP

/*
 * ConnectionManager Interface
 *
 * This class is responsible for managing connections and sessions.
 * A connection in this context represents an active interaction with a client, which ends when the response is sent.
 * A session represents a session with a client, which can span multiple connections, and lasts until the inactivity timeout.
 * 
*/

#include "IConnectionManager.hpp"
#include <map>
#include "IConnection.hpp"
#include "ISession.hpp"
#include "configuration/IConfiguration.hpp"
#include "logger/ILogger.hpp"
#include "factory/IFactory.hpp"
#include "constants/HttpHelper.hpp"

class ConnectionManager : public IConnectionManager
{
private:
    std::map<SocketDescriptor_t, IConnection *> _connections; // active connections
    std::map<SessionId_t, ISession *> _sessions;              // active sessions
    IFactory &_factory;                                         // factory object to create connections and sessions
    ILogger &_logger; // logger object

    // Generate a unique session ID
    SessionId_t _generateSessionId() const;

public:
    // Constructor
    ConnectionManager(ILogger &logger, IFactory &factory);

    // Destructor
    virtual ~ConnectionManager();

    // Methods related to connections
    virtual void addConnection(std::pair<int, std::pair<std::string, std::string>> clientInfo);
    virtual void removeConnection(SocketDescriptor_t socketDescriptor);
    virtual IConnection &getConnection(SocketDescriptor_t socketDescriptor);
    virtual IRequest &getRequest(SocketDescriptor_t socketDescriptor);
    virtual IResponse &getResponse(SocketDescriptor_t socketDescriptor);

    // Methods related to sessions
    virtual void addSession();
    virtual void removeSession(SessionId_t id);
    virtual ISession &getSession(SessionId_t id);
    virtual void setSessionData(SessionId_t id, const std::string &key, const std::string &value);
    virtual std::string getSessionData(SessionId_t id, const std::string &key);
};

#endif // CONNECTIONMANAGER_HPP
// Path: includes/connection/ConnectionManager.hpp
