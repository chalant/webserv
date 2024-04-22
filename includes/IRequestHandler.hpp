#ifndef IREQUESTHANDLER_HPP
#define IREQUESTHANDLER_HPP

#include "constants/HttpStatusCodeHelper.hpp"

class IRequestHandler
{
public:
    virtual ~IRequestHandler() = default;

    virtual int handleRequest(int) = 0;
    virtual int handlePipeException(int) = 0;
    virtual int handlePipeRead(int) = 0;
    virtual int handleErrorResponse(int, int) = 0;
    virtual int handleErrorResponse(int, HttpStatusCode) = 0;
};

#endif // IREQUESTHANDLER_HPP
// Path: includes/IRequestHandler.hpp
