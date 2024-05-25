#include "../../includes/response/Response.hpp"
#include "../../includes/utils/Converter.hpp"

/*
 * Response class
 *
 * This class represents an HTTP response.
 * It contains the status line, headers, and body of the response.
 *
 */

// Default constructor
Response::Response(const HttpHelper &httpHelper)
    : m_content_length(0), m_http_helper(httpHelper)
{
}

// Destructor
Response::~Response() {}

// Getter for status line
std::string Response::getStatusLine() const { return m_status_line; }

// Getter for headers - returns 1 string with all headers
std::string Response::getHeaders() const
{
    std::string headers;
    for (std::map<HttpHeader, std::string>::const_iterator it =
             m_headers.begin();
         it != m_headers.end(); it++)
    {
        // Construct each header line in the format "HeaderName: Value\r\n"
        headers += m_http_helper.httpHeaderStringMap(it->first) + ": " +
                   it->second + "\r\n";
    }
    return headers;
}

// Getter for body string
std::string Response::getBodyString() const { return m_body.data(); }

// Getter for body vector
std::vector<char> Response::getBody() const { return m_body; }

// Setter for status line - string input
void Response::setStatusLine(std::string status_line)
{
    m_status_line = status_line;
}

// Setter for status line - based on status code
void Response::setStatusLine(HttpStatusCode status_code)
{
    // Set status line based on the status code
    m_status_line = m_http_helper.getStatusLine(status_code);
}

// Setter for headers - vector of strings input
void Response::setHeaders(std::vector<std::string> headers)
{
    for (std::vector<std::string>::iterator it = headers.begin();
         it != headers.end(); it++)
    {
        std::string header = *it;
        std::string header_name = header.substr(0, header.find(":"));
        std::string header_value = header.substr(header.find(":") + 1);
        HttpHeader header_enum =
            m_http_helper.stringHttpHeaderMap(header_name);
        // Add header to the map
        m_headers[ header_enum ] = header_value;
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
        std::string header_name = header.substr(0, header.find(":"));
        std::string header_value = header.substr(header.find(":") + 1);
        HttpHeader header_enum =
            m_http_helper.stringHttpHeaderMap(header_name);
        // Add header to the map
        m_headers[ header_enum ] = header_value;
    }
}

// Add a header to the map - Enum, string input
void Response::addHeader(HttpHeader header, std::string value)
{
    m_headers[ header ] = value;
}

// Add a header to the map - string, string input
void Response::addHeader(std::string header, std::string value)
{
    HttpHeader header_enum = m_http_helper.stringHttpHeaderMap(header);
    m_headers[ header_enum ] = value;
}

// Add a cookie to the map
void Response::addCookie(std::string key, std::string value)
{
    m_cookies[ key ] = value;
}

// Add Cookie Headers to the response
void Response::addCookieHeaders()
{
    for (std::map<std::string, std::string>::const_iterator it =
             m_cookies.begin();
         it != m_cookies.end(); ++it)
    {
        // Construct the Set-Cookie header for the current cookie
        std::string cookie_header = it->first + "=" + it->second +
                                   "; HttpOnly; Secure; SameSite=Strict;";

        // Add the Set-Cookie header to the response
        this->addHeader(SET_COOKIE, cookie_header);
    }
}

// Setter for body - string input
void Response::setBody(std::string body)
{
    this->setBody(std::vector<char>(body.begin(), body.end()));
}

// Setter for body - vector of chars input
void Response::setBody(std::vector<char> body)
{
    m_body = body;
    m_content_length = body.size();
}

// Set all response fields from a status code
void Response::setErrorResponse(HttpStatusCode status_code)
{
    this->setStatusLine(status_code);
    std::string body = m_http_helper.getHtmlPage(status_code);
    this->setHeaders("content-type: text/html\r\n"
                     "content-length: " +
                     Converter::toString(body.length()) +
                     "\r\n"
                     "connection: close\r\n"
                     "server: webserv/1.0\r\n");
    this->setBody(body);
}

// Set all response fields from a status code - int input
void Response::setErrorResponse(int status_code)
{
    this->setErrorResponse(static_cast<HttpStatusCode>(status_code));
}

// Set all response fields from a complete response vector
void Response::setResponse(std::vector<char> response)
{
    // Parse the response vector
    std::string response_string(response.begin(), response.end());
    m_status_line = response_string.substr(0, response_string.find("\r\n"));
    response_string = response_string.substr(response_string.find("\r\n") + 2);
    this->setHeaders(response_string.substr(0, response_string.find("\r\n\r\n")));
    this->setBody(response_string.substr(response_string.find("\r\n\r\n") + 4));
}

// Extract the status code from the status line
std::string Response::getStatusCodeString() const
{
    return m_status_line.substr(m_status_line.find(" ") + 1);
}

// Calculate the size of the response
std::string Response::getResponseSizeString() const
{
    return Converter::toString(m_status_line.length() +
                               this->getHeaders().length() +
                               m_body.size());
}

// Calculate the size of the response in bytes
size_t Response::getResponseSize() const
{
    return m_status_line.length() + this->getHeaders().length() +
           m_body.size();
}

// Get the map of cookies
std::map<std::string, std::string> Response::getCookiesMap() const
{
    return m_cookies;
}

// Get a specific cookie from the map
std::string Response::getCookie(const std::string &key) const
{
    std::map<std::string, std::string>::const_iterator it =
        m_cookies.find(key);
    if (it != m_cookies.end())
    {
        return it->second;
    }
    return "";
}

// Convert headers to a map of strings
std::map<std::string, std::string> Response::getHeadersStringMap() const
{
    std::map<std::string, std::string> headers;
    for (std::map<HttpHeader, std::string>::const_iterator it =
             m_headers.begin();
         it != m_headers.end(); it++)
    {
        headers[ m_http_helper.httpHeaderStringMap(it->first) ] =
            it->second;
    }
    return headers;
}

// Serialise the response into a vector of chars
std::vector<char> Response::serialise()
{
    std::vector<char> response;

    // Add status line
    response.insert(response.end(), m_status_line.begin(),
                    m_status_line.end());

    // Add headers
    this->addCookieHeaders(); // Add cookies to the headers first
    const std::string &headers = this->getHeaders();
    response.insert(response.end(), headers.begin(), headers.end());

    // Add a blank line
    response.push_back('\r');
    response.push_back('\n');

    // Add body
    response.insert(response.end(), m_body.begin(), m_body.end());

    // Return the serialised response
    return response;
}

// Path: srcs/Response.cpp
