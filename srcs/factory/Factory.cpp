#include "../../includes/factory/Factory.hpp"
#include "../../includes/request/Request.hpp"
#include "../../includes/response/Response.hpp"
#include "../../includes/connection/Connection.hpp"
#include "../../includes/connection/Session.hpp"

Factory::Factory(const IConfiguration &configuration, ILogger &logger)
    : _configuration(configuration),
      _logger(logger),
      _httpHelper()
{
    // Log the creation of the Factory
    this->_logger.log(VERBOSE, "Factory created.");
}

Factory::~Factory()
{
    // Log the destruction of the Factory
    this->_logger.log(VERBOSE, "Factory destroyed.");
}

IConnection *Factory::createConnection(std::pair<int, std::pair<std::string, std::string> > clientInfo)
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
