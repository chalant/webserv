#ifndef SESSION_HPP
#define SESSION_HPP

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

#define DEFAULT_TIMEOUT 300 // 5 minutes

#include "ISession.hpp"
#include <ctime>
#include <map>
#include <string>

class Session : public ISession
{
private:
    const SessionId_t _id; // Unique session id
    const time_t _timeout; // Time elapsed before session expires
    time_t _lastAccess;    // Last time session was accessed
    std::map<std::string, std::string> _data; // Session data

public:
    // Constructor
    Session(SessionId_t id, time_t timeout = DEFAULT_TIMEOUT);

    // Destructor
    ~Session();

    virtual void touch();            // Update last access time
    virtual bool hasExpired() const; // Check if session has expired
    virtual void setData(const std::string &key,
                         const std::string &value); // Set session data
    virtual std::string
    getData(const std::string &key) const; // Get session data
    virtual SessionId_t getId() const;     // Get session id
};

#endif // SESSION_HPP
// Path: includes/connection/Session.hpp
