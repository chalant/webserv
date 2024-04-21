#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "IResponse.hpp"

class Response
{
private:
    std::string _statusLine;
    std::string _body;
    std::map<HttpHeader, std::string> _headers;
    RequestHelper _requestHelper;

public:
    Response();
    virtual ~Response() {};

    virtual std::string getStatusLine() const;
    virtual std::string getHeaders() const;
    virtual std::string getBody() const;
    virtual void setStatusLine(std::string statusLine);
    virtual void setStatusLine(HttpStatusCode statusCode);
    virtual void setHeaders(std::vector<std::string> headers);
    virtual void setHeaders(std::string headers);
    virtual void addHeader(HttpHeader header, std::string value);
    virtual void addHeader(std::string header, std::string value);
    virtual void setBody(std::string body);
    virtual void setErrorResponse(HttpStatusCode statusCode);
    virtual std::string getStatusCodeString() const;
    virtual std::string getResponseSizeString() const;
    virtual std::map<std::string, std::string> getHeadersStringMap() const;
    virtual std::string getHeadersString() const;
};

#endif // RESPONSE_HPP
// Path: includes/Response.hpp
