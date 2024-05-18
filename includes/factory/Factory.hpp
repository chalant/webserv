#ifndef FACTORY_HPP
#define FACTORY_HPP

#include "IFactory.hpp"
#include "../configuration/IConfiguration.hpp"
#include "../logger/ILogger.hpp"
#include "../constants/HttpHelper.hpp"

class Factory : public IFactory
{
private:
    const IConfiguration &_configuration;
    ILogger &_logger;
    const HttpHelper _httpHelper;

public:
    Factory(const IConfiguration &configuration, ILogger &_logger);
    virtual ~Factory();

    virtual IConnection *createConnection(std::pair<int, std::pair<std::string, std::string>>);
    virtual IRequest *createRequest();
    virtual IResponse *createResponse();
    virtual ISession *createSession(SessionId_t id);
};

#endif // FACTORY_HPP
// Path: includes/Factory.hpp
