#include "../includes/Factory.hpp"
#include "../includes/Request.hpp"
#include "../includes/Response.hpp"
#include "../includes/Session.hpp"

Factory::Factory(const IConfiguration &configuration, ILogger &logger)
    : _configuration(configuration),
      _logger(logger),
      _httpHelper(configuration) {}

ISession *Factory::createSession(std::pair<int, std::pair<std::string, std::string>> clientInfo)
{
    return new Session(clientInfo, this->_logger, this->createRequest(), this->createResponse());
}

IRequest *Factory::createRequest()
{
    return new Request(this->_configuration, this->_httpHelper);
}

IResponse *Factory::createResponse()
{
    return new Response(this->_httpHelper);
}

// Path: srcs/Factory.cpp
