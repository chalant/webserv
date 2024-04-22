#include "../includes/constants/HttpHelper.hpp"

/*
 * HttpHelper.hpp
 *
 * The HttpHelper class provides functionality for working with HTTP request components,
 * including methods, versions, and headers. It encapsulates instances of helper classes
 * for HTTP methods, versions, and headers, providing convenient access to their functionality.
 *
 * Path: includes/constants/HttpHelper.hpp
 */

// Constructor to initialize helper classes
HttpHelper::HttpHelper(const IConfiguration &configuration)
    : _methodHelper(configuration),
      _versionHelper(),
      _headerHelper(),
      _statusCodeHelper() {}

// Get string representation of HttpMethod enum value
const std::string &HttpHelper::httpMethodStringMap(HttpMethod method) const
{
    return this->_methodHelper.httpMethodStringMap(method);
}

// Get HttpMethod enum value from string representation
HttpMethod HttpHelper::stringHttpMethodMap(const std::string &method) const
{
    return this->_methodHelper.stringHttpMethodMap(method);
}

// Check if a string is a valid HTTP method
bool HttpHelper::isMethod(const std::string &method) const
{
    return this->_methodHelper.isMethod(method);
}

// Check if a string is a supported HTTP method
bool HttpHelper::isSupportedMethod(const std::string &method) const
{
    return this->_methodHelper.isSupportedMethod(method);
}

// Get string representation of HttpVersion enum value
const std::string &HttpHelper::httpVersionStringMap(HttpVersion version) const
{
    return this->_versionHelper.httpVersionStringMap(version);
}

// Get HttpVersion enum value from string representation
HttpVersion HttpHelper::stringHttpVersionMap(const std::string &version) const
{
    return this->_versionHelper.stringHttpVersionMap(version);
}

// Check if a string represents a valid HTTP version
bool HttpHelper::isHttpVersion(const std::string &version) const
{
    return this->_versionHelper.isHttpVersion(version);
}

// Get string representation of HttpHeader enum value
const std::string &HttpHelper::httpHeaderStringMap(HttpHeader header) const
{
    return this->_headerHelper.httpHeaderStringMap(header);
}

// Get HttpHeader enum value from string representation
HttpHeader HttpHelper::stringHttpHeaderMap(const std::string &header) const
{
    return this->_headerHelper.stringHttpHeaderMap(header);
}

// Check if a string represents a valid HTTP header
bool HttpHelper::isHeaderName(const std::string &header) const
{
    return this->_headerHelper.isHeaderName(header);
}

// Get string representation of HttpStatusCode enum value
const std::string &HttpHelper::httpStatusCodeStringMap(HttpStatusCode statusCode) const
{
    return this->_statusCodeHelper.httpStatusCodeStringMap(statusCode);
}

// Get HttpStatusCode enum value from string representation
HttpStatusCode HttpHelper::stringHttpStatusCodeMap(const std::string &statusCode) const
{
    return this->_statusCodeHelper.stringHttpStatusCodeMap(statusCode);
}

// Get HttpStatusCode enum value from integer representation
HttpStatusCode HttpHelper::intHttpStatusCodeMap(int statusCode) const
{
    return this->_statusCodeHelper.intHttpStatusCodeMap(statusCode);
}

// Generate a status line with the specified HTTP status code
std::string HttpHelper::getStatusLine(HttpStatusCode statusCode) const
{
    return this->_statusCodeHelper.getStatusLine(statusCode);
}

// Generate an HTML page with the specified HTTP status code
std::string HttpHelper::getHtmlPage(HttpStatusCode statusCode) const
{
    return this->_statusCodeHelper.getHtmlPage(statusCode);
}

// Generate an HTML page with the specified HTTP status code
std::string HttpHelper::getErrorResponse(HttpStatusCode statusCode) const
{
    return this->_statusCodeHelper.getErrorResponse(statusCode);
}

// Path: srcs/constants/HttpHeaderHelper.cpp