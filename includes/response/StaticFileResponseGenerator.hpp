#ifndef STATICFILERESPONSEGENERATOR_HPP
#define STATICFILERESPONSEGENERATOR_HPP

#include "../logger/ILogger.hpp"
#include "IResponseGenerator.hpp"

class StaticFileResponseGenerator : public IResponseGenerator
{
private:
    ILogger &_logger;

public:
    StaticFileResponseGenerator(ILogger &logger);
    ~StaticFileResponseGenerator();
    Triplet_t generateResponse(const IRoute &route, const IRequest &request,
                               const IConfiguration &configuration);
};

#endif // STATICFILERESPONSEGENERATOR_HPP
// Path: includes/response/StaticFileResponseGenerator.hpp
