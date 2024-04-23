#ifndef IRESPONSEGENERATOR_HPP
#define IRESPONSEGENERATOR_HPP

#include "IRequest.hpp"
#include "IResponse.hpp"

class IResponseGenerator
{
public:
    virtual ~IResponseGenerator(){};

    virtual int generateResponse(const IRequest &request, IResponse &response) = 0;
};

#endif // IRESPONSEGENERATOR_HPP
       // Path: includes/IResponseGenerator.hpp