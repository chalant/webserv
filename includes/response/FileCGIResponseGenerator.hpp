#ifndef RFCCGIRESPONSEGENERATOR_HPP
#define RFCCGIRESPONSEGENERATOR_HPP

#include "../configuration/IConfiguration.hpp"
#include "../constants/HttpStatusCodeHelper.hpp"
#include "../logger/ILogger.hpp"
#include "../request/IRequest.hpp"
#include "IResponseGenerator.hpp"
#include "IRoute.hpp"
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

class FileCGIResponseGenerator : public IResponseGenerator
{
private:
	
public:
    FileCGIResponseGenerator(ILogger &logger);
    ~FileCGIResponseGenerator();

    virtual Triplet_t generateResponse(const IRoute &route,
                                       const IRequest &request,
                                       IResponse &response,
                                       IConfiguration &configuration);
};

#endif // RFCCGIRESPONSEGENERATOR_HPP
// Path: includes/CgiResponseGenerator.hpp
