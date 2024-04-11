#include "../includes/constants/RequestHelper.hpp"

/*
 * RequestHelper.hpp
 *
 * The RequestHelper class provides functionality for working with HTTP request components,
 * including methods, versions, and headers. It encapsulates instances of helper classes
 * for HTTP methods, versions, and headers, providing convenient access to their functionality.
 *
 * Path: includes/constants/RequestHelper.hpp
 */

// Get string representation of HttpMethod enum value
const std::string &RequestHelper::httpMethodStringMap(HttpMethod method) const
{
    return _methodHelper.httpMethodStringMap(method);
}

// Get HttpMethod enum value from string representation
HttpMethod RequestHelper::stringHttpMethodMap(const std::string &method) const
{
    return _methodHelper.stringHttpMethodMap(method);
}

// Check if a string is a valid HTTP method
bool RequestHelper::isMethod(const std::string &method) const
{
    return _methodHelper.isMethod(method);
}

// Check if a string is a supported HTTP method
bool RequestHelper::isSupportedMethod(const std::string &method) const
{
    return _methodHelper.isSupportedMethod(method);
}

// Get string representation of HttpVersion enum value
const std::string &RequestHelper::httpVersionStringMap(HttpVersion version) const
{
    return _versionHelper.httpVersionStringMap(version);
}

// Get HttpVersion enum value from string representation
HttpVersion RequestHelper::stringHttpVersionMap(const std::string &version) const
{
    return _versionHelper.stringHttpVersionMap(version);
}

// Check if a string represents a valid HTTP version
bool RequestHelper::isHttpVersion(const std::string &version) const
{
    return _versionHelper.isHttpVersion(version);
}

// Get string representation of HttpHeader enum value
const std::string &RequestHelper::httpHeaderStringMap(HttpHeader header) const
{
    return _headerHelper.httpHeaderStringMap(header);
}

// Get HttpHeader enum value from string representation
HttpHeader RequestHelper::stringHttpHeaderMap(const std::string &header) const
{
    return _headerHelper.stringHttpHeaderMap(header);
}

// Check if a string represents a valid HTTP header
bool RequestHelper::isHeaderName(const std::string &header) const
{
    return _headerHelper.isHeaderName(header);
}

// Get string representation of HttpStatusCode enum value
const std::string &RequestHelper::httpStatusCodeStringMap(HttpStatusCode statusCode) const
{
    return _statusCodeHelper.httpStatusCodeStringMap(statusCode);
}

// Get HttpStatusCode enum value from string representation
HttpStatusCode RequestHelper::stringHttpStatusCodeMap(const std::string &statusCode) const
{
    return _statusCodeHelper.stringHttpStatusCodeMap(statusCode);
}

// Get HTML page for a given HttpStatusCode
std::string RequestHelper::getHtmlPage(HttpStatusCode statusCode) const
{
    return _statusCodeHelper.getHtmlPage(statusCode);
}

// Path: srcs/constants/HttpHeaderHelper.cpp