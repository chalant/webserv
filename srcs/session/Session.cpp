#include "session/Session.hpp"

Session::Session(std::pair<int, std::pair<std::string, std::string> > clientInfo, ILogger &logger, IRequest *request, IResponse *response)
    : _socketDescriptor(clientInfo.first),
     _ip(clientInfo.second.first),
     _port(std::stoi(clientInfo.second.second)),
     _remoteAddress(_ip + ":" + clientInfo.second.second),
     _logger(logger),
     _request(request),
     _response(response) {}

Session::~Session() {
    delete this->_request;
    delete this->_response;
}

void Session::setReadPipeDescriptor(int pipe)
{
    this->_readPipeDescriptor = pipe;
}

int Session::getSocket() const
{
    return this->_socketDescriptor;
}

const std::string &Session::getIp() const
{
    return this->_ip;
}

int Session::getPort() const
{
    return this->_port;
}

const std::string &Session::getRemoteAddress() const
{
    return this->_remoteAddress;
}

int Session::getReadPipeDescriptor() const
{
    return this->_readPipeDescriptor;
}

IRequest &Session::getRequest()
{
    return *this->_request;
}

IResponse &Session::getResponse()
{
    return *this->_response;
}

// Path: srcs/Session.cpp
