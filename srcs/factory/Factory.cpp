#include "factory/Factory.hpp"
#include "request/Request.hpp"
#include "response/Response.hpp"
#include "connection/Connection.hpp"
#include "connection/Session.hpp"

Factory::Factory(const IConfiguration &configuration, ILogger &logger)
    : _configuration(configuration),
      _logger(logger),
      _httpHelper(configuration) {}

IConnection *Factory::createConnection(std::pair<int, std::pair<std::string, std::string>> clientInfo)
{
    return new Connection(clientInfo, this->_logger, this->createRequest(), this->createResponse());
}

IRequest *Factory::createRequest()
{
    return new Request(this->_configuration, this->_httpHelper);
}

IResponse *Factory::createResponse()
{
    return new Response(this->_httpHelper);
}

ISession *Factory::createSession(SessionId_t id)
{
    return new Session(id);
}

// Path: srcs/Factory.cpp
