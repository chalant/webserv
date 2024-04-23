#ifndef FACTORY_HPP
#define FACTORY_HPP

#include "IFactory.hpp"
#include "configuration/IConfiguration.hpp"
#include "logger/ILogger.hpp"
#include "constants/HttpHelper.hpp"

class Factory : public IFactory
{
private:
    const IConfiguration &_configuration;
    ILogger &_logger;
    const HttpHelper _httpHelper;
public:
    Factory(const IConfiguration &configuration, ILogger &_logger);

    virtual ISession * createSession(std::pair<int, std::pair<std::string, std::string>>);
    virtual IRequest * createRequest();
    virtual IResponse * createResponse();
};

#endif // FACTORY_HPP
// Path: includes/Factory.hpp
