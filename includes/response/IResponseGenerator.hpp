#ifndef IRESPONSEGENERATOR_HPP
#define IRESPONSEGENERATOR_HPP

#include "IResponse.hpp"
#include "IRoute.hpp"
#include "request/IRequest.hpp"
#include "configuration/IConfiguration.hpp"

class IResponseGenerator
{
public:
    virtual ~IResponseGenerator(){};

    virtual int generateResponse(const IRoute &route, const IRequest &request, const IConfiguration &configuration, const std::string &scriptName = "") = 0;
};

#endif // IRESPONSEGENERATOR_HPP
       // Path: includes/IResponseGenerator.hpp