#ifndef IRESPONSE_HPP
#define IRESPONSE_HPP

/*
 * Response class
 *
 * This class represents an HTTP response
 * It contains the status line, headers, and body of the response.
 *
 */

#include <string>
#include <map>
#include "../constants/HttpStatusCodeHelper.hpp"
#include "../constants/HttpHeaderHelper.hpp"

class IResponse
{
public:
    virtual ~IResponse() {};

    // Getters for status line, headers, and body
    virtual std::string getStatusLine() const = 0;
    virtual std::string getHeaders() const = 0;
    virtual std::string getBody() const = 0;

    // Setters for status line, headers, and body
    virtual void setStatusLine(std::string statusLine) = 0;
    virtual void setStatusLine(HttpStatusCode statusCode) = 0;
    virtual void setHeaders(std::vector<std::string> headers) = 0;
    virtual void setHeaders(std::string headers) = 0;
    virtual void addHeader(HttpHeader header, std::string value) = 0;
    virtual void addHeader(std::string header, std::string value) = 0;
    virtual void addCookie(std::string key, std::string value) = 0;
    virtual void addCookieHeaders() = 0;
    virtual void setBody(std::string body) = 0;

    // Set error response with appropriate status code
    virtual void setErrorResponse(HttpStatusCode statusCode) = 0;
    virtual void setErrorResponse(int statusCode) = 0;

    // Set response fields from a complete response vector
    virtual void setResponse(std::vector<char> response) = 0;

    // Getters for specific parts of the response
    virtual std::string getStatusCodeString() const = 0;
    virtual std::string getResponseSizeString() const = 0;
    virtual size_t getResponseSize() const = 0;
    virtual std::map<std::string, std::string> getCookiesMap() const = 0;
    virtual std::string getCookie(const std::string &key) const = 0;

    // Convert headers to map or string
    virtual std::map<std::string, std::string> getHeadersStringMap() const = 0;
    virtual std::vector<char> serialise() = 0;
};

#endif // IRESPONSE_HPP
// Path: includes/IResponse.hpp
