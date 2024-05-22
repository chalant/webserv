#include "../../includes/connection/Session.hpp"

/*
 * Session
 *
 * Session class is used to store session data.
 * Its id is set and read from the session cookie.
 * Its lifetime is determined by the timeout since the last access.
 *
 * It is owned by the ConnectionManager.
 *
 */

// Constructor
Session::Session(SessionId_t id, time_t timeout) : m_id(id), m_timeout(timeout)
{
    this->touch();
}

// Destructor
Session::~Session() {}

// Touch session updates last access time
void Session::touch() { this->m_last_access = time(NULL); }

// Check if session has expired
bool Session::hasExpired() const
{
    return time(NULL) - this->m_last_access > this->m_timeout;
}

// Set session data
void Session::setData(const std::string &key, const std::string &value)
{
    this->m_data[ key ] = value;
}

// Get session data
std::string Session::getData(const std::string &key) const
{
    std::map<std::string, std::string>::const_iterator it =
        this->m_data.find(key);

    if (it == this->m_data.end())
        return "";
    return it->second;
}

// Get session id
SessionId_t Session::getId() const { return this->m_id; }

// Path: srcs/connection/Session.cpp
