#ifndef CONNECTION_HPP
#define CONNECTION_HPP

/*
 * Connection class
 *
 * This class represents a network connection and interaction with a client.
 * It stores the socket descriptor, client IP address, port number, and remote address,
 * as well as pointers to the request and response objects.
 * Its lifetime is tied to the active connection with the client.
 *
 * It is owned by the ConnectionManager.
 *
 */

 #define DEFAULT_TIMEOUT 300 // 5 minutes

#include "IConnection.hpp"
#include <ctime>
#include "../logger/ILogger.hpp"

class Connection : public IConnection
{
private:
    SocketDescriptor_t _socketDescriptor; // Socket descriptor for the connection
    std::string _ip;                      // Client's IP address
    int _port;                            // Port number
    std::string _remoteAddress;           // Remote address
    int _responseReadPipefd;              // Read pipe descriptor for the response
    int _requestWritePipefd;              // Write pipe descriptor for the request
    int _cgiPid;                          // PID of the CGI process
    ILogger &_logger;                     // Reference to the logger
    IRequest *_request;                   // Pointer to the request object
    IResponse *_response;                 // Pointer to the response object
    ISession *_session;                   // Pointer to the session object
    const time_t _timeout;                // Timeout for the connection
    time_t _lastAccess;                   // Last access time

public:
    Connection(std::pair<int, std::pair<std::string, std::string> > clientInfo, ILogger &logger, IRequest *request, IResponse *response, time_t timeout = DEFAULT_TIMEOUT);
    virtual ~Connection();

    // Setters
    virtual void setSession(ISession *session);

    // Getters
    virtual int getSocketDescriptor() const;
    virtual std::string getIp() const;
    virtual int getPort() const;
    virtual std::string getRemoteAddress() const;
    virtual int getResponseReadPipefd() const;
    virtual IRequest &getRequest() const;
    virtual IResponse &getResponse() const;
    virtual ISession &getSession() const;
    virtual void setCgiInfo(int pid, int responseReadPipefd, int requestWritePipefd);

    // Connection management
    virtual void touch(); // Update the last access time
    virtual bool hasExpired() const; // Check if the connection has expired
};

#endif // CONNECTION_HPP
// Path: includes/connection/Connection.hpp
