#ifndef REQUESTHELPER_HPP
#define REQUESTHELPER_HPP

/*
 * RequestHelper.hpp
 *
 * The RequestHelper class provides functionality for working with HTTP request components,
 * including methods, versions, and headers. It encapsulates instances of helper classes
 * for HTTP methods, versions, and headers, providing convenient access to their functionality.
 *
 * Path: includes/constants/RequestHelper.hpp
 */

#include <string>
#include "HttpMethodHelper.hpp"
#include "HttpVersionHelper.hpp"
#include "HttpHeaderHelper.hpp"

class RequestHelper
{
private:
    // Private member variables for helper classes
    HttpMethodHelper _methodHelper;   // Helper for HTTP methods
    HttpVersionHelper _versionHelper; // Helper for HTTP versions
    HttpHeaderHelper _headerHelper;   // Helper for HTTP headers

public:
    // Http Method Helper Functions
    const std::string &httpMethodStringMap(HttpMethod method) const; // Get string representation of HttpMethod
    HttpMethod stringHttpMethodMap(const std::string &method) const; // Get HttpMethod enum value from string representation

    // Http Version Helper Functions
    const std::string &httpVersionStringMap(HttpVersion version) const; // Get string representation of HttpVersion
    HttpVersion stringHttpVersionMap(const std::string &version) const; // Get HttpVersion enum value from string representation

    // Http Header Helper Functions
    const std::string &httpHeaderStringMap(HttpHeader header) const; // Get string representation of HttpHeader
    HttpHeader stringHttpHeaderMap(const std::string &header) const; // Get HttpHeader enum value from string representation
};

#endif // REQUESTHELPER_HPP
       // Path: includes/constants/RequestHelper.hpp