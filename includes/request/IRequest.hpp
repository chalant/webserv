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
#include "../constants/HttpHelper.hpp"

class IRequest
{
public:
    virtual ~IRequest() {};

    // Getters
    virtual HttpMethod getMethod() const = 0;
    virtual std::string getMethodString() const = 0;
    virtual std::string getUri() const = 0;
    virtual HttpVersion getHttpVersion() const = 0;
    virtual std::string getHttpVersionString() const = 0;
    virtual const std::map<HttpHeader, std::string> getHeaders() const = 0;
    virtual std::string getHeaderValue(HttpHeader header) const = 0;
    virtual std::map<std::string, std::string> getHeadersStringMap() const = 0;
    virtual std::map<std::string, std::string> getQueryParameters() const = 0;
    virtual std::map<std::string, std::string> getCookies() const = 0;
    virtual std::string getCookie(const std::string &) const = 0;
    virtual const std::vector<char> getBody() const = 0;
    virtual std::string getBodyString() const = 0;
    virtual std::string getQueryString() const = 0;
    virtual std::string getContentLength() const = 0;
    virtual std::string getContentType() const = 0;
    virtual std::string getPathInfo(const std::string &scriptName) const = 0;
    virtual std::string getClientIp() const = 0;
    virtual std::string getHostName() const = 0;
    virtual std::string getHostPort() const = 0;
    virtual std::string getAuthority() const = 0;

    // Setters
    virtual void setMethod(const std::string &method) = 0;
    virtual void setUri(const std::string &uri) = 0;
    virtual void setHttpVersion(const std::string &httpVersion) = 0;
    virtual void addHeader(const std::string &key, const std::string &value) = 0;
    virtual void setBody(const std::vector<char> &body) = 0;
    virtual void setBody(const std::string &body) = 0;
    virtual void addCookie(const std::string &key, const std::string &value) = 0;
    virtual void setAuthority() = 0;
};

#endif // IREQUEST_HPP
       // Path: includes/IRequest.hpp
