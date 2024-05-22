#include "../../includes/factory/Factory.hpp"
#include "../../includes/connection/Connection.hpp"
#include "../../includes/connection/Session.hpp"
#include "../../includes/request/Request.hpp"
#include "../../includes/response/Response.hpp"

Factory::Factory(const IConfiguration &configuration, ILogger &logger)
    : m_configuration(configuration), m_logger(logger), m_http_helper()
{
    // Log the creation of the Factory
    this->m_logger.log(VERBOSE, "Factory created.");
}

Factory::~Factory()
{
    // Log the destruction of the Factory
    this->m_logger.log(VERBOSE, "Factory destroyed.");
}

IConnection *Factory::createConnection(
    std::pair<int, std::pair<std::string, std::string> > clientInfo)
{
    return new Connection(clientInfo, this->m_logger, this->createRequest(),
                          this->createResponse());
}

IRequest *Factory::createRequest()
{
    return new Request(this->m_configuration, this->m_http_helper);
}

IResponse *Factory::createResponse() { return new Response(this->m_http_helper); }

ISession *Factory::createSession(SessionId_t id) { return new Session(id); }

// Path: srcs/Factory.cpp
