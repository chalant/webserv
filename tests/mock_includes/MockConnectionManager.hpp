#ifndef MOCKCONNECTIONMANAGER_HPP
#define MOCKCONNECTIONMANAGER_HPP

/*
 * MockConnectionManager class
 *
 * This class is a mock implementation of the IConnectionManager interface.
 * It is used for testing purposes.
 *
 */

#include "connection/IConnectionManager.hpp"
#include "MockConnection.hpp"
#include "MockRequest.hpp"
#include "MockResponse.hpp"
#include "MockSession.hpp"

class MockConnectionManager : public IConnectionManager
{
public:
    // Destructor
    virtual ~MockConnectionManager();

    // Methods related to connections
    virtual void addConnection(std::pair<int, std::pair<std::string, std::string>> clientInfo);
    virtual void removeConnection(SocketDescriptor_t socketDescriptor);
    virtual IConnection &getConnection(SocketDescriptor_t socketDescriptor);
    virtual IRequest &getRequest(SocketDescriptor_t socketDescriptor);
    virtual IResponse &getResponse(SocketDescriptor_t socketDescriptor);

    // Methods related to sessions
    virtual SessionId_t addSession();
    virtual void removeSession(SessionId_t id);
    virtual ISession &getSession(SessionId_t id);
    virtual void setSessionData(SessionId_t id, const std::string &key, const std::string &value);
    virtual std::string getSessionData(SessionId_t id, const std::string &key);
    virtual size_t getNumberOfSessions() const;

    // Method to assign a session to a connection
    virtual void assignSessionToConnection(IConnection &connection, const IRequest &request, IResponse &response);

    // Method to retire idle sessions
    virtual void collectGarbage();
};

#endif // MOCKCONNECTIONMANAGER_HPP
// Path: tests/mock_includes/MockConnectionManager.hpp
