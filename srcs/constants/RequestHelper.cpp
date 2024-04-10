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

// Path: srcs/constants/HttpHeaderHelper.cpp