#ifndef IRESPONSEGENERATOR_HPP
#define IRESPONSEGENERATOR_HPP

#include "IRoute.hpp"
#include "../request/IRequest.hpp"
#include "../configuration/IConfiguration.hpp"

typedef std::pair<int, std::pair<int, int> > Triplet_t;

class IResponseGenerator
{
public:
    virtual ~IResponseGenerator(){};

    virtual Triplet_t generateResponse(const IRoute &route, const IRequest &request, const IConfiguration &configuration, const std::string &scriptName = "") = 0;
};

#endif // IRESPONSEGENERATOR_HPP
       // Path: includes/IResponseGenerator.hpp
