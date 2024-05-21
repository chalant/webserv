#ifndef MOCKRESPONSE_HPP
#define MOCKRESPONSE_HPP

/*
 * MockResponse class
 *
 * This class is a mock implementation of the IResponse interface.
 * It is used for testing purposes.
 *
 */

#include "../../includes/response/IResponse.hpp"
#include "../../includes/constants/HttpHelper.hpp"

class MockResponse : public IResponse
{
private:
    HttpHelper _httpHelper;
    std::string _statusLine;
    std::map<HttpHeader, std::string> _headers;
    std::string _body;
public:
    // Default constructor
    MockResponse();

    // Destructor
    virtual ~MockResponse();

    // Getters for status line, headers, and body
    virtual std::string getStatusLine() const;
    virtual std::string getHeaders() const;
    virtual std::string getBody() const;

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

#endif // MOCKRESPONSE_HPP
// Path: tests/mock_srcs/MockResponse.cpp
