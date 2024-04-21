#include "../includes/Response.hpp"

Response::Response() {}

Response::~Response() {}

std::string Response::getStatusLine() const
{
    return this->_statusLine;
}

std::string Response::getHeaders() const
{
    std::string headers;
    for (std::map<HttpHeader, std::string>::const_iterator it = _headers.begin();
         it != this->_headers.end();
         it++)
    {
        headers += this->_requestHelper.httpHeaderStringMap(it->first) + ": " + it->second + "\r\n";
    }
    return headers;
}

std::string Response::getBody() const
{
    return this->_body;
}

void Response::setStatusLine(std::string statusLine)
{
    this->_statusLine = statusLine;
}

void Response::setStatusLine(HttpStatusCode statusCode)
{
    this->_statusLine = this->_requestHelper.getStatusLine(statusCode);
}

void Response::setHeaders(std::vector<std::string> headers)
{
    for (std::vector<std::string>::iterator it = headers.begin();
         it != headers.end();
         it++)
    {
        std::string header = *it;
        std::string headerName = header.substr(0, header.find(":"));
        std::string headerValue = header.substr(header.find(":") + 1);
        HttpHeader headerEnum = this->_requestHelper.stringHttpHeaderMap(headerName);
        this->_headers[headerEnum] = headerValue;
    }
}

void Response::setHeaders(std::string headers)
{
    while (headers.find("\r\n") != std::string::npos)
    {
        std::string header = headers.substr(0, headers.find("\r\n"));
        headers = headers.substr(headers.find("\r\n") + 2);
        std::string headerName = header.substr(0, header.find(":"));
        std::string headerValue = header.substr(header.find(":") + 1);
        HttpHeader headerEnum = this->_requestHelper.stringHttpHeaderMap(headerName);
        this->_headers[headerEnum] = headerValue;
    }
}

void Response::addHeader(HttpHeader header, std::string value)
{
    this->_headers[header] = value;
}

void Response::addHeader(std::string header, std::string value)
{
    HttpHeader headerEnum = this->_requestHelper.stringHttpHeaderMap(header);
    this->_headers[headerEnum] = value;
}

void Response::setBody(std::string body)
{
    this->_body = body;
}

void Response::setErrorResponse(HttpStatusCode statusCode)
{
    this->setStatusLine(statusCode);
    std::string body = this->_requestHelper.getHtmlPage(statusCode);
    this->setHeaders("Content-Type: text/html\r\n"
                     "Content-Length: " + std::to_string(body.length()) + "\r\n"
                     "Connection: close\r\n"
                     "Server: webserv/1.0\r\n"
                     "\r\n");
    this->setBody(body);
}

std::string Response::getStatusCodeString() const
{
    return this->_statusLine.substr(this->_statusLine.find(" ") + 1);
}

std::string Response::getResponseSizeString() const
{
    return std::to_string(this->_statusLine.length() + this->getHeaders().length() + this->_body.length());
}

std::map<std::string, std::string> Response::getHeadersStringMap() const
{
    std::map<std::string, std::string> headers;
    for (std::map<HttpHeader, std::string>::const_iterator it = this->_headers.begin();
         it != this->_headers.end();
         it++)
    {
        headers[this->_requestHelper.httpHeaderStringMap(it->first)] = it->second;
    }
    return headers;
}

std::string Response::getHeadersString() const
{
    std::string headers;
    for (std::map<HttpHeader, std::string>::const_iterator it = this->_headers.begin();
         it != this->_headers.end();
         it++)
    {
        headers += this->_requestHelper.httpHeaderStringMap(it->first) + ": " + it->second + "\r\n";
    }
    return headers;
}

// Path: srcs/Response.cpp
