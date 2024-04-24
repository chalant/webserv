#include "connection/Connection.hpp"

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

Connection::Connection(std::pair<int, std::pair<std::string, std::string> > clientInfo, ILogger &logger, IRequest *request, IResponse *response)
    : _socketDescriptor(clientInfo.first),
     _ip(clientInfo.second.first),
     _port(std::stoi(clientInfo.second.second)),
     _remoteAddress(_ip + ":" + clientInfo.second.second),
     _logger(logger),
     _request(request),
     _response(response) {}

Connection::~Connection() {
    delete this->_request;
    delete this->_response;
}

void Connection::setReadPipeDescriptor(int pipe)
{
    this->_readPipeDescriptor = pipe;
}

void Connection::setSession(ISession *session)
{
    this->_session = session;
}

int Connection::getSocketDescriptor() const
{
    return this->_socketDescriptor;
}

const std::string &Connection::getIp() const
{
    return this->_ip;
}

int Connection::getPort() const
{
    return this->_port;
}

const std::string &Connection::getRemoteAddress() const
{
    return this->_remoteAddress;
}

int Connection::getReadPipeDescriptor() const
{
    return this->_readPipeDescriptor;
}

IRequest &Connection::getRequest() const
{
    return *this->_request;
}

IResponse &Connection::getResponse() const
{
    return *this->_response;
}

// Path: srcs/Connection.cpp
