#include "../mock_includes/MockResponse.hpp"

/*
 * MockResponse class
 *
 * This class is a mock implementation of the IResponse interface.
 * It is used for testing purposes.
 *
 */

// Destructor
MockResponse::~MockResponse() {}

// Getters for status line, headers, and body
std::string MockResponse::getStatusLine() const { return ""; }

std::string MockResponse::getHeaders() const { return ""; }

std::string MockResponse::getBody() const { return ""; }

// Setters for status line, headers, and body
void MockResponse::setStatusLine(std::string statusLine) { (void)statusLine; }

void MockResponse::setStatusLine(HttpStatusCode statusCode)
{
    (void)statusCode;
}

void MockResponse::setHeaders(std::vector<std::string> headers)
{
    (void)headers;
}

void MockResponse::setHeaders(std::string headers) { (void)headers; }

void MockResponse::addHeader(HttpHeader header, std::string value)
{
    (void)header;
    (void)value;
}

void MockResponse::addHeader(std::string header, std::string value)
{
    (void)header;
    (void)value;
}

void MockResponse::addCookie(std::string key, std::string value)
{
    (void)key;
    (void)value;
}

void MockResponse::addCookieHeaders() {}

void MockResponse::setBody(std::string body) { (void)body; }

// Set error response with appropriate status code
void MockResponse::setErrorResponse(HttpStatusCode statusCode)
{
    (void)statusCode;
}

void MockResponse::setErrorResponse(int statusCode) { (void)statusCode; }

// Set response fields from a complete response vector
void MockResponse::setResponse(std::vector<char> response) { (void)response; }

// Getters for specific parts of the response
std::string MockResponse::getStatusCodeString() const { return ""; }

std::string MockResponse::getResponseSizeString() const { return ""; }

size_t MockResponse::getResponseSize() const { return 0; }

std::map<std::string, std::string> MockResponse::getCookiesMap() const
{
    return std::map<std::string, std::string>();
}

std::string MockResponse::getCookie(const std::string &key) const
{
    (void)key;
    return "";
}

// Convert headers to map or string
std::map<std::string, std::string> MockResponse::getHeadersStringMap() const
{
    return std::map<std::string, std::string>();
}

std::vector<char> MockResponse::serialise() { return std::vector<char>(); }

// Path: tests/mock_srcs/MockResponse.cpp
