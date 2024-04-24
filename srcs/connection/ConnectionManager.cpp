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
    : _lastGarbageCollection(std::time(nullptr)),
      _factory(factory),
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

    // Log the new connection
    this->_logger.log(VERBOSE, "New connection created. Remote address: " + clientInfo.second.first + ":" + clientInfo.second.second + " Socket: " + std::to_string(clientInfo.first));
}

// Remove a connection
void ConnectionManager::removeConnection(SocketDescriptor_t socketDescriptor)
{
    delete this->_connections[socketDescriptor];
    this->_connections.erase(socketDescriptor);

    // Log the removed connection
    this->_logger.log(VERBOSE, "Connection removed. Socket: " + std::to_string(socketDescriptor));
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
SessionId_t ConnectionManager::addSession()
{
    // Generate a new session id
    SessionId_t sessionId = this->_generateSessionId();

    // Create a new session
    ISession *session = this->_factory.createSession(sessionId);

    // Add the session to the map
    this->_sessions[sessionId] = session;

    // Log the new session
    this->_logger.log(VERBOSE, "New session created. Session ID: " + std::to_string(sessionId));

    // Return the session id
    return sessionId;
}

// Remove a session
void ConnectionManager::removeSession(SessionId_t sessionId)
{
    delete this->_sessions[sessionId];
    this->_sessions.erase(sessionId);

    // Log the removed session
    this->_logger.log(VERBOSE, "Session removed. Session ID: " + std::to_string(sessionId));
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

// Get the number of active sessions
size_t ConnectionManager::getNumberOfSessions() const
{
    return this->_sessions.size();
}

// Assign a session to a connection
void ConnectionManager::assignSessionToConnection(IConnection &connection, const IRequest &request, IResponse &response)
{
    // Declare a session id
    SessionId_t sessionId;

    // Verify if a session already exists
    std::string sessionIdString = request.getCookie("session");
    if (sessionIdString.empty())
    {
        sessionId = this->addSession(); // Generate a new session id
    }
    else
    {
        sessionId = this->_stringToSessionId(sessionIdString);
        if (this->_sessions.find(sessionId) == this->_sessions.end())
        {
            sessionId = this->addSession(); // Session not found, generate a new session id
        }
        else
        {
            // Log the session assignment
            this->_logger.log(VERBOSE, "Existing session found. Session ID: " + std::to_string(sessionId));
        }
    }

    // Set the session for the connection
    connection.setSession(this->_sessions[sessionId]);

    // Add a "session" cookie to the response
    response.addCookie("session", std::to_string(sessionId));

    // Log the session assignment
    this->_logger.log(INFO, "Session ID: " + std::to_string(sessionId) + " Assigned to connection on Socket: " + std::to_string(connection.getSocketDescriptor()) + " with client: " + connection.getRemoteAddress());
}

// Retire idle sessions
void ConnectionManager::collectGarbage()
{
    // Check if it is time to collect garbage
    time_t now = std::time(nullptr);
    if (now - this->_lastGarbageCollection < GARBAGE_COLLECTOR_INTERVAL)
    {
        return; // Not yet time for garbage collection
    }

    // Update the last garbage collection time
    this->_lastGarbageCollection = now;

    // Log the garbage collection
    this->_logger.log(VERBOSE, "Garbage collection started.");

    // Make note of the number of sessions before garbage collection
    size_t sessionCount = this->_sessions.size();

    // Iterate over all sessions
    for (std::map<SessionId_t, ISession *>::iterator it = this->_sessions.begin();
         it != this->_sessions.end();)
    {
        // Check if the session has expired
        if (it->second->hasExpired())
        {
            // Log the expired session
            this->_logger.log(VERBOSE, "Session expired. Session ID: " + std::to_string(it->first));

            // Remove the session
            this->removeSession(it->first);
        }
        else
        {
            it++; // Move to the next session
        }
    }

    // Calculate the number of sessions retired
    size_t retiredSessions = sessionCount - this->_sessions.size();

    // Log the garbage collection
    this->_logger.log(DEBUG, "Garbage collection completed. Retired " + std::to_string(retiredSessions) + " session" + (retiredSessions == 1 ? "." : "s."));
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

// Convert a string to a session id
SessionId_t ConnectionManager::_stringToSessionId(const std::string &string) const
{
    std::istringstream iss(string);
    SessionId_t sessionId = 0;
    iss >> sessionId; // This will extract an integer from the string

    // Check if the extraction was successful
    if (iss.fail() || !iss.eof())
    {
        // Handle conversion failure, return 0 as default
        return 0;
    }

    return sessionId;
}

// path: srcs/connection/ConnectionManager.cpp
