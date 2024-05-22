#include "../../includes/connection/ConnectionManager.hpp"
#include "../../includes/utils/Converter.hpp"
#include <cstdlib>
#include <ctime>
#include <sstream>

/*
 * ConnectionManager
 *
 * This class is responsible for managing connections and sessions.
 * A connection in this context represents an active interaction with a client,
 * which ends when the response is sent. A session represents a session with a
 * client, which can span multiple connections, and lasts until the inactivity
 * timeout.
 *
 */

// Constructor
ConnectionManager::ConnectionManager(ILogger &logger, IFactory &factory)
    : m_last_garbage_collection(std::time(NULL)), m_factory(factory),
      m_logger(logger)
{
    std::srand(static_cast<unsigned int>(
        std::time(NULL))); // Seed srand, to create session id's

    // Log the creation of the ConnectionManager
    this->m_logger.log(VERBOSE, "ConnectionManager created.");
}

// Destructor
ConnectionManager::~ConnectionManager()
{
    // Delete all connections
    for (std::map<SocketDescriptor_t, IConnection *>::iterator it =
             this->m_connections.begin();
         it != this->m_connections.end(); it++)
        delete it->second;

    // Delete all sessions
    for (std::map<SessionId_t, ISession *>::iterator it =
             this->m_sessions.begin();
         it != this->m_sessions.end(); it++)
        delete it->second;

    // Log the destruction of the ConnectionManager
    this->m_logger.log(VERBOSE, "ConnectionManager destroyed.");
}

// Add a new connection
void ConnectionManager::addConnection(
    std::pair<int, std::pair<std::string, std::string> > client_info)
{
    IConnection *connection = this->m_factory.createConnection(client_info);
    m_connections[ client_info.first ] = connection;

    // Log the new connection
    this->m_logger.log(
        VERBOSE, "New connection created. Remote address: " +
                     client_info.second.first + ":" + client_info.second.second +
                     " Socket: " + Converter::toString(client_info.first));
}

// Remove a connection
void ConnectionManager::removeConnection(SocketDescriptor_t socket_descriptor)
{
    delete this->m_connections[ socket_descriptor ];
    this->m_connections[ socket_descriptor ] = NULL;

    // Log the removed connection
    this->m_logger.log(VERBOSE, "Connection removed. Socket: " +
                                   Converter::toString(socket_descriptor));
}

// Get a reference to a connection
IConnection &
ConnectionManager::getConnection(SocketDescriptor_t socket_descriptor)
{
    return *this->m_connections[ socket_descriptor ];
}

// Get a reference to a request
IRequest &ConnectionManager::getRequest(SocketDescriptor_t socket_descriptor)
{
    return this->m_connections[ socket_descriptor ]->getRequest();
}

// Get a reference to a response
IResponse &ConnectionManager::getResponse(SocketDescriptor_t socket_descriptor)
{
    return this->m_connections[ socket_descriptor ]->getResponse();
}

// Add a new session
SessionId_t ConnectionManager::addSession()
{
    // Generate a new session id
    SessionId_t session_id = this->_generateSessionId();

    // Create a new session
    ISession *session = this->m_factory.createSession(session_id);

    // Add the session to the map
    this->m_sessions[ session_id ] = session;

    // Log the new session
    this->m_logger.log(VERBOSE, "New session created. Session ID: " +
                                   Converter::toString(session_id));

    // Return the session id
    return session_id;
}

// Remove a session
void ConnectionManager::removeSession(SessionId_t session_id)
{
    delete this->m_sessions[ session_id ];
    this->m_sessions[ session_id ] = NULL;

    // Log the removed session
    this->m_logger.log(VERBOSE, "Session removed. Session ID: " +
                                   Converter::toString(session_id));
}

// Get a reference to a session
ISession &ConnectionManager::getSession(SessionId_t session_id)
{
    return *this->m_sessions[ session_id ];
}

// Set session data
void ConnectionManager::setSessionData(SessionId_t session_id,
                                       const std::string &key,
                                       const std::string &value)
{
    this->m_sessions[ session_id ]->setData(key, value);
}

// Get session data
std::string ConnectionManager::getSessionData(SessionId_t session_id,
                                              const std::string &key)
{
    return this->m_sessions[ session_id ]->getData(key);
}

// Get the number of active sessions
size_t ConnectionManager::getNumberOfSessions() const
{
    return this->m_sessions.size();
}

// Assign a session to a connection
void ConnectionManager::assignSessionToConnection(IConnection &connection,
                                                  const IRequest &request,
                                                  IResponse &response)
{
    // Declare a session id
    SessionId_t session_id;

    // Verify if a session already exists
    std::string session_id_string = request.getCookie("session");
    if (session_id_string.empty())
    {
        session_id = this->addSession(); // Generate a new session id
    }
    else
    {
        session_id = this->_stringToSessionId(session_id_string);
        if (this->m_sessions.find(session_id) == this->m_sessions.end())
        {
            session_id = this->addSession(); // Session not found, generate a new
                                            // session id
        }
        else
        {
            // Log the session assignment
            this->m_logger.log(VERBOSE, "Existing session found. Session ID: " +
                                           Converter::toString(session_id));
        }
    }

    // Set the session for the connection
    connection.setSession(this->m_sessions[ session_id ]);

    // Add a "session" cookie to the response
    response.addCookie("session", Converter::toString(session_id));

    // Log the session assignment
    this->m_logger.log(
        INFO, "Session ID: " + Converter::toString(session_id) +
                  " Assigned to connection on Socket: " +
                  Converter::toString(connection.getSocketDescriptor()) +
                  " with client: " + connection.getRemoteAddress());
}

// Retire idle sessions and reap zombie processes
void ConnectionManager::collectGarbage()
{
    // Check if it is time to collect garbage
    time_t now = std::time(NULL);
    if (now - this->m_last_garbage_collection < GARBAGE_COLLECTOR_INTERVAL)
    {
        return; // Not yet time for garbage collection
    }

    // Update the last garbage collection time
    this->m_last_garbage_collection = now;

    // Log the garbage collection
    this->m_logger.log(VERBOSE, "Garbage collection started.");

    // Reap zombie processes
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ; // Reap all zombie processes

    // Make note of the number of sessions before garbage collection
    size_t session_count = this->m_sessions.size();
    std::vector<SessionId_t> sessions_to_erase;

    // Iterate over all sessions
    for (std::map<SessionId_t, ISession *>::iterator it =
             this->m_sessions.begin();
         it != this->m_sessions.end(); it++)
    {
        // Check if the session has expired
        if (it->second && it->second->hasExpired())
        {
            // Log the expired session
            this->m_logger.log(VERBOSE, "Session expired. Session ID: " +
                                           Converter::toString(it->first));

            // Remove the session
            this->removeSession(it->first);
            sessions_to_erase.push_back(it->first);
        }
    }

    for (size_t i = 0; i < sessions_to_erase.size(); i++)
    {
        this->m_sessions.erase(sessions_to_erase[ i ]);
    }

    std::vector<SocketDescriptor_t> connections_to_erase;
    // Iterate over all connections
    for (std::map<SocketDescriptor_t, IConnection *>::iterator it =
             this->m_connections.begin();
         it != this->m_connections.end(); it++)
    {
        // Check if the connection has expired
        if (it->second && it->second->hasExpired())
        {
            // Log the expired connection
            this->m_logger.log(VERBOSE, "Connection expired. Socket: " +
                                           Converter::toString(it->first));

            // Remove the connection
            this->removeConnection(it->first);
            connections_to_erase.push_back(it->first);
        }
    }

    for (size_t i = 0; i < connections_to_erase.size(); i++)
    {
        this->m_sessions.erase(connections_to_erase[ i ]);
    }
    // Calculate the number of sessions retired
    size_t retired_sessions = session_count - this->m_sessions.size();

    // Log the garbage collection
    this->m_logger.log(DEBUG,
                      "Garbage collection completed. Retired " +
                          Converter::toString(retired_sessions) + " session" +
                          (retired_sessions == 1 ? "." : "s.") +
                          " Remaining sessions: " +
                          Converter::toString(this->m_sessions.size()) + ".");
}

// Generate a unique session ID
SessionId_t ConnectionManager::_generateSessionId() const
{
    // Random id between 1 and size_t max
    SessionId_t session_id = std::rand() % static_cast<SessionId_t>(-1) + 1;

    // Make sure it is not in use
    while (this->m_sessions.find(session_id) != this->m_sessions.end())
        session_id = std::rand() % static_cast<SessionId_t>(-1) + 1;

    // Return the id
    return session_id;
}

// Convert a string to a session id
SessionId_t
ConnectionManager::_stringToSessionId(const std::string &string) const
{
    std::istringstream iss(string);
    SessionId_t session_id = 0;
    iss >> session_id; // This will extract an integer from the string

    // Check if the extraction was successful
    if (iss.fail() || !iss.eof())
    {
        // Handle conversion failure, return 0 as default
        return 0;
    }

    return session_id;
}

// path: srcs/connection/ConnectionManager.cpp
