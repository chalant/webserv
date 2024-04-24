#include "../../includes/response/Response.hpp"

/*
 * Response class
 *
 * This class represents an HTTP response.
 * It contains the status line, headers, and body of the response.
 *
 */

// Default constructor
Response::Response(const HttpHelper &httpHelper)
    : _httpHelper(httpHelper) {}

// Destructor
Response::~Response() {}

// Getter for status line
std::string Response::getStatusLine() const
{
    return this->_statusLine;
}

// Getter for headers - returns 1 string with all headers
std::string Response::getHeaders() const
{
    std::string headers;
    for (std::map<HttpHeader, std::string>::const_iterator it = _headers.begin();
         it != this->_headers.end();
         it++)
    {
        // Construct each header line in the format "HeaderName: Value\r\n"
        headers += this->_httpHelper.httpHeaderStringMap(it->first) + ": " + it->second + "\r\n";
    }
    return headers;
}

// Getter for body
std::string Response::getBody() const
{
    return this->_body;
}

// Setter for status line - string input
void Response::setStatusLine(std::string statusLine)
{
    this->_statusLine = statusLine;
}

// Setter for status line - based on status code
void Response::setStatusLine(HttpStatusCode statusCode)
{
    // Set status line based on the status code
    this->_statusLine = this->_httpHelper.getStatusLine(statusCode);
}

// Setter for headers - vector of strings input
void Response::setHeaders(std::vector<std::string> headers)
{
    for (std::vector<std::string>::iterator it = headers.begin();
         it != headers.end();
         it++)
    {
        std::string header = *it;
        std::string headerName = header.substr(0, header.find(":"));
        std::string headerValue = header.substr(header.find(":") + 1);
        HttpHeader headerEnum = this->_httpHelper.stringHttpHeaderMap(headerName);
        // Add header to the map
        this->_headers[headerEnum] = headerValue;
    }
}

// Setter for headers - single string input
void Response::setHeaders(std::string headers)
{
    // Parse headers in the format "HeaderName: Value\r\n"
    while (headers.find("\r\n") != std::string::npos)
    {
        std::string header = headers.substr(0, headers.find("\r\n"));
        headers = headers.substr(headers.find("\r\n") + 2);
        std::string headerName = header.substr(0, header.find(":"));
        std::string headerValue = header.substr(header.find(":") + 1);
        HttpHeader headerEnum = this->_httpHelper.stringHttpHeaderMap(headerName);
        // Add header to the map
        this->_headers[headerEnum] = headerValue;
    }
}

// Add a header to the map - Enum, string input
void Response::addHeader(HttpHeader header, std::string value)
{
    this->_headers[header] = value;
}

// Add a header to the map - string, string input
void Response::addHeader(std::string header, std::string value)
{
    HttpHeader headerEnum = this->_httpHelper.stringHttpHeaderMap(header);
    this->_headers[headerEnum] = value;
}

// Add a cookie to the map
void Response::addCookie(std::string key, std::string value)
{
    this->_cookies[key] = value;
}

// Add Cookie Headers to the response
void Response::addCookieHeaders()
{
    for (std::map<std::string, std::string>::const_iterator it = this->_cookies.begin();
         it != this->_cookies.end();
         ++it)
    {
        // Construct the Set-Cookie header for the current cookie
        std::string cookieHeader = it->first + "=" + it->second + "; HttpOnly; Secure; SameSite=Strict;";

        // Add the Set-Cookie header to the response
        this->addHeader(SET_COOKIE, cookieHeader);
    }
}

// Setter for body
void Response::setBody(std::string body)
{
    this->_body = body;
}

// Set all response fields from a status code
void Response::setErrorResponse(HttpStatusCode statusCode)
{
    this->setStatusLine(statusCode);
    std::string body = this->_httpHelper.getHtmlPage(statusCode);
    this->setHeaders("Content-Type: text/html\r\n"
                     "Content-Length: " +
                     std::to_string(body.length()) + "\r\n"
                                                     "Connection: close\r\n"
                                                     "Server: webserv/1.0\r\n"
                                                     "\r\n");
    this->setBody(body);
}

// Set all response fields from a status code - int input
void Response::setErrorResponse(int statusCode)
{
    this->setErrorResponse(static_cast<HttpStatusCode>(statusCode));
}

// Set all response fields from a complete response vector
void Response::setResponse(std::vector<char> response)
{
    // Parse the response vector
    std::string responseString(response.begin(), response.end());
    this->_statusLine = responseString.substr(0, responseString.find("\r\n"));
    responseString = responseString.substr(responseString.find("\r\n") + 2);
    this->setHeaders(responseString.substr(0, responseString.find("\r\n\r\n")));
    this->_body = responseString.substr(responseString.find("\r\n\r\n") + 4);
}

// Extract the status code from the status line
std::string Response::getStatusCodeString() const
{
    return this->_statusLine.substr(this->_statusLine.find(" ") + 1);
}

// Calculate the size of the response
std::string Response::getResponseSizeString() const
{
    return std::to_string(this->_statusLine.length() + this->getHeaders().length() + this->_body.length());
}

// Calculate the size of the response in bytes
size_t Response::getResponseSize() const
{
    return this->_statusLine.length() + this->getHeaders().length() + this->_body.length();
}

// Get the map of cookies
std::map<std::string, std::string> Response::getCookiesMap() const
{
    return this->_cookies;
}

// Get a specific cookie from the map
std::string Response::getCookie(const std::string &key) const
{
    std::map<std::string, std::string>::const_iterator it = this->_cookies.find(key);
    if (it != this->_cookies.end())
    {
        return it->second;
    }
    return "";
}

// Convert headers to a map of strings
std::map<std::string, std::string> Response::getHeadersStringMap() const
{
    std::map<std::string, std::string> headers;
    for (std::map<HttpHeader, std::string>::const_iterator it = this->_headers.begin();
         it != this->_headers.end();
         it++)
    {
        headers[this->_httpHelper.httpHeaderStringMap(it->first)] = it->second;
    }
    return headers;
}

// Serialise the response into a vector of chars
std::vector<char> Response::serialise()
{
    std::vector<char> response;

    // Add status line
    response.insert(response.end(), this->_statusLine.begin(), this->_statusLine.end());

    // Add headers
    this->addCookieHeaders(); // Add cookies to the headers first
    response.insert(response.end(), this->getHeaders().begin(), this->getHeaders().end());

    // Add a blank line
    response.push_back('\r');
    response.push_back('\n');

    // Add body
    response.insert(response.end(), this->_body.begin(), this->_body.end());

    // Return the serialised response
    return response;
}

// Path: srcs/Response.cpp
