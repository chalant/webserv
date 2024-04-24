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

#include "IConnection.hpp"
#include "ILogger.hpp"

class Connection : public IConnection
{
private:
    SocketDescriptor_t _socketDescriptor; // Socket descriptor for the connection
    std::string _ip;                      // Client's IP address
    int _port;                            // Port number
    std::string _remoteAddress;           // Remote address
    int _readPipeDescriptor;              // Read pipe descriptor
    ILogger &_logger;                     // Reference to the logger
    IRequest *_request;                   // Pointer to the request object
    IResponse *_response;                 // Pointer to the response object

public:
    Connection(std::pair<int, std::pair<std::string, std::string>> clientInfo, ILogger &logger, IRequest *request, IResponse *response);
    virtual ~Connection() = default;

    virtual void setReadPipeDescriptor(int pipe);

    virtual int getSocket() const;
    virtual const std::string &getIp() const;
    virtual int getPort() const;
    virtual const std::string &getRemoteAddress() const;
    virtual int getReadPipeDescriptor() const;
    virtual IRequest &getRequest() const;
    virtual IResponse &getResponse() const;
};

#endif // CONNECTION_HPP
// Path: includes/connection/Connection.hpp
