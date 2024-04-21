#ifndef IRESPONSE_HPP
#define IRESPONSE_HPP

#include "constants/RequestHelper.hpp"
#include <string>
#include <map>

class IResponse
{
public:
    virtual ~IResponse() {};

    virtual std::string getStatusLine() const = 0;
    virtual std::string getHeaders() const = 0;
    virtual std::string getBody() const = 0;
    virtual void setStatusLine(std::string statusLine) = 0;
    virtual void setStatusLine(HttpStatusCode statusCode) = 0;
    virtual void setHeaders(std::string headers) = 0;
    virtual void setBody(std::string body) = 0;
    virtual void setErrorResponse(HttpStatusCode statusCode) = 0;
    virtual std::string getStatusCodeString() const = 0;
    virtual std::string getResponseSizeString() const = 0;
    virtual std::map<std::string, std::string> getHeadersString() const = 0;
};

#endif // IRESPONSE_HPP
// Path: includes/IResponse.hpp