#include "../mock_includes/MockResponse.hpp"
#include "../../includes/constants/HttpHelper.hpp"
#include "../../includes/utils/Converter.hpp"
#include <map>
#include <string>
#include <vector>

/*
 * MockResponse class
 *
 * This class is a mock implementation of the IResponse interface.
 * It is used for testing purposes.
 *
 */

// Default constructor
MockResponse::MockResponse() : m_http_helper(HttpHelper()) {}

// Destructor
MockResponse::~MockResponse() {}

// Getters for status line, headers, and body
std::string MockResponse::getStatusLine() const { return this->m_status_line; }

std::string MockResponse::getHeaders() const { return ""; }

std::string MockResponse::getBodyString() const
{
    return std::string(this->m_body.begin(), this->m_body.end());
}

std::vector<char> MockResponse::getBody() const { return this->m_body; }

// Setters for status line, headers, and body
void MockResponse::setStatusLine(std::string status_line)
{
    this->m_status_line = status_line;
}

void MockResponse::setStatusLine(HttpStatusCode status_code)
{
    // Set status line based on the status code
    this->m_status_line = this->m_http_helper.getStatusLine(status_code);
}

void MockResponse::setHeaders(std::vector<std::string> headers)
{
    for (std::vector<std::string>::iterator it = headers.begin();
         it != headers.end(); it++)
    {
        std::string header = *it;
        std::string header_name = header.substr(0, header.find(":"));
        std::string header_value = header.substr(header.find(":") + 1);
        HttpHeader header_enum =
            this->m_http_helper.stringHttpHeaderMap(header_name);
        // Add header to the map
        this->m_headers[ header_enum ] = header_value;
    }
}

void MockResponse::setHeaders(std::string headers)
{
    // Parse headers in the format "HeaderName: Value\r\n"
    while (headers.find("\r\n") != std::string::npos)
    {
        std::string header = headers.substr(0, headers.find("\r\n"));
        headers = headers.substr(headers.find("\r\n") + 2);
        std::string header_name = header.substr(0, header.find(":"));
        std::string header_value = header.substr(header.find(":") + 1);
        HttpHeader header_enum =
            this->m_http_helper.stringHttpHeaderMap(header_name);
        // Add header to the map
        this->m_headers[ header_enum ] = header_value;
    }
}

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

void MockResponse::setBody(std::string body)
{
    this->setBody(std::vector<char>(body.begin(), body.end()));
}

void MockResponse::setBody(std::vector<char> body) { this->m_body = body; }

// Set error response with appropriate status code
void MockResponse::setErrorResponse(HttpStatusCode status_code)
{
    this->setStatusLine(status_code);
    std::string body = this->m_http_helper.getHtmlPage(status_code);
    this->setHeaders("content-type: text/html\r\n"
                     "content-length: " +
                     Converter::toString(body.length()) +
                     "\r\n"
                     "connection: close\r\n"
                     "server: webserv/1.0\r\n");
    this->setBody(body);
}

void MockResponse::setErrorResponse(int status_code) { (void)status_code; }

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
