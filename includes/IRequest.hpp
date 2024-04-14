#ifndef IREQUEST_HPP
#define IREQUEST_HPP

/*
 * IRequest.hpp
 * Abstract base class for Request in webserv
 *
 * This file defines the Request interface, which serves as the abstract base
 * class for the Request in webserv. 
 * IRequest allows for polymorphic behavior and dependency injection,
 * thereby enabling us to create a MockRequest class for isolated unit testing.
 * 
 */

#include <string>
#include <map>
#include <vector>
#include <ctime>
#include "constants/HttpMethodHelper.hpp"
#include "constants/HttpVersionHelper.hpp"
#include "constants/HttpHeaderHelper.hpp"

class IRequest
{
public:
    virtual ~IRequest() {};

    // Clear the request object
    virtual void clear() = 0;

    // Getters
    virtual HttpMethod getMethod() const = 0;
    virtual std::string getUri() const = 0;
    virtual HttpVersion getHttpVersion() const = 0;
    virtual std::string getHeaderValue(HttpHeader header) const = 0;
    virtual std::map<std::string, std::string> getQueryParameters() const = 0;
    virtual std::map<std::string, std::string> getCookies() const = 0;
    virtual const std::vector<char> getBody() const = 0;

    // Setters
    virtual void setMethod(const std::string &method) = 0;
    virtual void setUri(const std::string &uri) = 0;
    virtual void setHttpVersion(const std::string &httpVersion) = 0;
    virtual void addHeader(const std::string &key, const std::string &value) = 0;
    virtual void setBody(const std::vector<char> &body) = 0;
};

#endif // IREQUEST_HPP
       // Path: includes/IRequest.hpp
