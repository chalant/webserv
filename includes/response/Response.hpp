#ifndef RESPONSE_HPP
#define RESPONSE_HPP

/*
 * Response class
 *
 * This class represents an HTTP response
 * It contains the status line, headers, and body of the response.
 *
 */

#include "../../includes/constants/HttpHelper.hpp"
#include "IResponse.hpp"

class Response : public IResponse
{
private:
    // Response Status line
    std::string _statusLine;

    // Response headers
    std::map<HttpHeader, std::string> _headers;

    // Response body
    std::vector<char> _body;

    // Body size
    size_t _contentLength;

    // Response Cookies
    std::map<std::string, std::string> _cookies;

    // Helper
    const HttpHelper &_httpHelper;

public:
    Response(const HttpHelper &httpHelper);
    ~Response();

    // Getters for status line, headers, and body
    virtual std::string getStatusLine() const;
    virtual std::string getHeaders() const;
    virtual std::string getBodyString() const;
    virtual std::vector<char> getBody() const;

    // Setters for status line, headers, and body
    virtual void setStatusLine(std::string statusLine);
    virtual void setStatusLine(HttpStatusCode statusCode);
    virtual void setHeaders(std::vector<std::string> headers);
    virtual void setHeaders(std::string headers);
    virtual void addHeader(HttpHeader header, std::string value);
    virtual void addHeader(std::string header, std::string value);
    virtual void addCookie(std::string key, std::string value);
    virtual void addCookieHeaders();
    virtual void setBody(std::string body);
    virtual void setBody(std::vector<char> body);

    // Set error response with appropriate status code
    virtual void setErrorResponse(HttpStatusCode statusCode);
    virtual void setErrorResponse(int statusCode);

    // Set response fields from a complete response vector
    virtual void setResponse(std::vector<char> response);

    // Getters for specific parts of the response
    virtual std::string getStatusCodeString() const;
    virtual std::string getResponseSizeString() const;
    virtual size_t getResponseSize() const;
    virtual std::map<std::string, std::string> getCookiesMap() const;
    virtual std::string getCookie(const std::string &key) const;

    // Convert headers to map or string
    virtual std::map<std::string, std::string> getHeadersStringMap() const;
    virtual std::vector<char> serialise();
};

#endif // RESPONSE_HPP
// Path: includes/Response.hpp
