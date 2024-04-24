#ifndef ICONNECTION_HPP
#define ICONNECTION_HPP

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

typedef int SocketDescriptor_t;

#include <string>
#include "request/IRequest.hpp"
#include "response/IResponse.hpp"

class IConnection
{
public:
    virtual ~IConnection() = default;

    virtual void setReadPipeDescriptor(int pipe) = 0;

    virtual int getSocket() const = 0;
    virtual const std::string &getIp() const = 0;
    virtual int getPort() const = 0;
    virtual const std::string &getRemoteAddress() const = 0;
    virtual int getReadPipeDescriptor() const = 0;
    virtual IRequest &getRequest() const = 0;
    virtual IResponse &getResponse() const = 0;
};

#endif // ICONNECTION_HPP
// Path: includes/connection/IConnection.hpp
