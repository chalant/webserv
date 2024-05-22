#include "../mock_includes/MockConnectionManager.hpp"
#include "../mock_includes/MockConnection.hpp"
#include "../mock_includes/MockRequest.hpp"
#include "../mock_includes/MockResponse.hpp"
#include "../mock_includes/MockSession.hpp"

/*
 * MockConnectionManager class
 *
 * This class is a mock implementation of the IConnectionManager interface.
 * It is used for testing purposes.
 *
 */

// Destructor
MockConnectionManager::~MockConnectionManager() {}

// Methods related to connections
void MockConnectionManager::addConnection(
    std::pair<int, std::pair<std::string, std::string> > client_info)
{
    (void)client_info;
}

void MockConnectionManager::removeConnection(
    SocketDescriptor_t socket_descriptor)
{
    (void)socket_descriptor;
}

IConnection &
MockConnectionManager::getConnection(SocketDescriptor_t socket_descriptor)
{
    (void)socket_descriptor;
    return *new MockConnection();
}

IRequest &MockConnectionManager::getRequest(SocketDescriptor_t socket_descriptor)
{
    (void)socket_descriptor;
    return *new MockRequest();
}

IResponse &
MockConnectionManager::getResponse(SocketDescriptor_t socket_descriptor)
{
    (void)socket_descriptor;
    return *new MockResponse();
}

// Methods related to sessions
SessionId_t MockConnectionManager::addSession() { return 0; }

void MockConnectionManager::removeSession(SessionId_t id) { (void)id; }

ISession &MockConnectionManager::getSession(SessionId_t id)
{
    (void)id;
    return *new MockSession();
}

void MockConnectionManager::setSessionData(SessionId_t id,
                                           const std::string &key,
                                           const std::string &value)
{
    (void)id;
    (void)key;
    (void)value;
}

std::string MockConnectionManager::getSessionData(SessionId_t id,
                                                  const std::string &key)
{
    (void)id;
    (void)key;
    return "";
}

size_t MockConnectionManager::getNumberOfSessions() const { return 0; }

void MockConnectionManager::assignSessionToConnection(IConnection &connection,
                                                      const IRequest &request,
                                                      IResponse &response)
{
    (void)connection;
    (void)request;
    (void)response;
}

void MockConnectionManager::collectGarbage() {}

// path: tests/mock_srcs/MockConnectionManager.cpp
