#include "connection/ConnectionManager.hpp"

/*
 * ConnectionManager
 *
 * This class is responsible for managing connections and sessions.
 * A connection in this context represents an active interaction with a client, which ends when the response is sent.
 * A session represents a session with a client, which can span multiple connections, and lasts until the inactivity timeout.
 * 
*/

// Constructor
ConnectionManager::ConnectionManager(ILogger &logger, IFactory &factory)
    : _factory(factory),
      _logger(logger)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed srand, to create session id's
}

// Destructor
ConnectionManager::~ConnectionManager()
{
    // Delete all connections
    for (std::map<SocketDescriptor_t, IConnection *>::iterator it = this->_connections.begin();
        it != this->_connections.end();
        it++)
        delete it->second;

    // Delete all sessions
    for (std::map<SessionId_t, ISession *>::iterator it = this->_sessions.begin();
        it != this->_sessions.end();
        it++)
        delete it->second;
}

// Add a new connection
void ConnectionManager::addConnection(std::pair<int, std::pair<std::string, std::string>> clientInfo)
{
    IConnection *connection = this->_factory.createConnection(clientInfo);
    _connections[clientInfo.first] = connection;

    this->_logger.log(VERBOSE, "New connection created. Remote address: " + clientInfo.second.first + ":" + clientInfo.second.second + " Socket: " + std::to_string(clientInfo.first));
}

// Remove a connection
void ConnectionManager::removeConnection(SocketDescriptor_t socketDescriptor)
{
    delete this->_connections[socketDescriptor];
    this->_connections.erase(socketDescriptor);
}

// Get a reference to a connection
IConnection &ConnectionManager::getConnection(SocketDescriptor_t socketDescriptor)
{
    return *this->_connections[socketDescriptor];
}

// Get a reference to a request
IRequest &ConnectionManager::getRequest(SocketDescriptor_t socketDescriptor)
{
    return this->_connections[socketDescriptor]->getRequest();
}

// Get a reference to a response
IResponse &ConnectionManager::getResponse(SocketDescriptor_t socketDescriptor)
{
    return this->_connections[socketDescriptor]->getResponse();
}

// Add a new session
void ConnectionManager::addSession()
{
    // Generate a new session id
    SessionId_t sessionId = this->_generateSessionId();

    // Create a new session
    ISession *session = this->_factory.createSession(sessionId);

    // Add the session to the map
    this->_sessions[sessionId] = session;
}

// Remove a session
void ConnectionManager::removeSession(SessionId_t sessionId)
{
    delete this->_sessions[sessionId];
    this->_sessions.erase(sessionId);
}

// Get a reference to a session
ISession &ConnectionManager::getSession(SessionId_t sessionId)
{
    return *this->_sessions[sessionId];
}

// Set session data
void ConnectionManager::setSessionData(SessionId_t sessionId, const std::string &key, const std::string &value)
{
    this->_sessions[sessionId]->setData(key, value);
}

// Get session data
std::string ConnectionManager::getSessionData(SessionId_t sessionId, const std::string &key)
{
    return this->_sessions[sessionId]->getData(key);
}

// Generate a unique session ID
SessionId_t ConnectionManager::_generateSessionId() const
{
    // Random id between 1 and size_t max
    SessionId_t sessionId = std::rand() % static_cast<SessionId_t>(-1) + 1;

    // Make sure it is not in use
    while (this->_sessions.find(sessionId) != this->_sessions.end())
        sessionId = std::rand() % static_cast<SessionId_t>(-1) + 1;

    // Return the id
    return sessionId;
}

// path: srcs/connection/ConnectionManager.cpp
