#ifndef ICONNECTIONMANAGER_HPP
#define ICONNECTIONMANAGER_HPP

/*
 * ConnectionManager Interface
 *
 * This class is responsible for managing connections and sessions.
 * A connection in this context represents an active interaction with a client, which ends when the response is sent.
 * A session represents a session with a client, which can span multiple connections, and lasts until the inactivity timeout.
 * 
*/

#include <string>
#include "IConnection.hpp"

class IConnectionManager
{
public:
    virtual ~IConnectionManager() = default;

    // Methods related to connections
    virtual void addConnection(std::pair<int, std::pair<std::string, std::string>>) = 0;
    virtual void removeConnection(SocketDescriptor_t) = 0;
    virtual IConnection &getConnection(SocketDescriptor_t) = 0;
    virtual IRequest &getRequest(SocketDescriptor_t) = 0;
    virtual IResponse &getResponse(SocketDescriptor_t) = 0;

    // Methods related to sessions
    virtual void addSession() = 0;
    virtual void removeSession(SessionId_t) = 0;
    virtual ISession &getSession(SessionId_t) = 0;
    virtual void setSessionData(SessionId_t, const std::string &, const std::string &) = 0;
    virtual std::string getSessionData(SessionId_t, const std::string &) = 0;
};

#endif // ICONNECTIONMANAGER_HPP
// Path: includes/connection/IConnectionManager.hpp
