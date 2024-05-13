#include "../../includes/connection/Connection.hpp"
#include "../../includes/utils/Converter.hpp"

/*
 * Connection class
 *
 * This class represents a network connection and interaction with a client.
 * It stores the socket descriptor, client IP address, port number, and remote address,
 * as well as pointers to the request and response objects.
 * Its lifetime is determined either by the 'Connection' header in the request or the timeout value.
 *
 * It is owned by the ConnectionManager.
 *
 */

// Constructor
Connection::Connection(std::pair<int, std::pair<std::string, std::string> > clientInfo, ILogger &logger, IRequest *request, IResponse *response, time_t timeout)
    : _socketDescriptor(clientInfo.first),
      _ip(clientInfo.second.first),
      _port(Converter::toInt(clientInfo.second.second)),
      _remoteAddress(_ip + ":" + clientInfo.second.second),
      _logger(logger),
      _request(request),
      _response(response),
      _timeout(timeout) 
{
    this->touch();
}

// Destructor
Connection::~Connection()
{
    delete this->_request;
    delete this->_response;
}

// Set session
void Connection::setSession(ISession *session)
{
    this->_session = session;
}

// Getters
int Connection::getSocketDescriptor() const
{
    return this->_socketDescriptor;
}

std::string Connection::getIp() const
{
    return this->_ip;
}

int Connection::getPort() const
{
    return this->_port;
}

std::string Connection::getRemoteAddress() const
{
    return this->_remoteAddress;
}

int Connection::getResponseReadPipefd() const
{
    return this->_responseReadPipefd;
}

IRequest &Connection::getRequest() const
{
    return *this->_request;
}

IResponse &Connection::getResponse() const
{
    return *this->_response;
}

ISession &Connection::getSession() const
{
    return *this->_session;
}

void Connection::setCgiInfo(int cgiPid, int responseReadPipefd, int requestWritePipefd)
{
    this->_cgiPid = cgiPid;
    this->_responseReadPipefd = responseReadPipefd;
    this->_requestWritePipefd = requestWritePipefd;
}

// Connection management
void Connection::touch()
{
    this->_lastAccess = time(NULL);
}

bool Connection::hasExpired() const
{
    // Log the connection timeout
    this->_logger.log("Connection timeout: " + Converter::toString(this->_timeout) + " seconds");
    return time(NULL) - this->_lastAccess > this->_timeout;
}

// Path: srcs/Connection.cpp
