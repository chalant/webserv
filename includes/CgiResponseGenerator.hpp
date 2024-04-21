#ifndef CGIRESPONSEGENERATOR_HPP
#define CGIRESPONSEGENERATOR_HPP

#include "IResponseGenerator.hpp"
#include "constants/HttpStatusCodeHelper.hpp"
#include <unistd.h>
#include <string.h>

class CgiResponseGenerator : public IResponseGenerator
{
private:
    HttpStatusCodeHelper _httpStatusCodeHelper;
public:
    virtual int generateResponse(const IRequest &request, IResponse &response);
};

#endif // CGIRESPONSEGENERATOR_HPP
// Path: includes/CgiResponseGenerator.hpp
