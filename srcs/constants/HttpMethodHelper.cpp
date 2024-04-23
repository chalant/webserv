#include "constants/HttpMethodHelper.hpp"

/*
 * HttpMethodHelper.hpp
 *
 * The HttpMethodHelper class provides functionality for working with HTTP methods.
 * It defines an enum HttpMethod to represent standard HTTP methods and provides methods
 * to retrieve string representations of methods and vice versa.
 *
 */

// Constructor initializes member variables using helper functions
HttpMethodHelper::HttpMethodHelper(const IConfiguration &configuration)
    : _methodList(_setMethodList()),
      _supportedMethods(configuration.getStringVector("Methods")),
      _stringHttpMethodMap(_setStringHttpMethodMap()),
      _httpMethodStringMap(_setHttpMethodStringMap()) {}

// Get string representation of HttpMethod enum value
const std::string &HttpMethodHelper::httpMethodStringMap(HttpMethod method) const
{
    if (_httpMethodStringMap.find(method) != _httpMethodStringMap.end())
    {
        return _httpMethodStringMap.at(method);
    }
    else
    {
        throw UnknownMethodError();
    }
}

// Get HttpMethod enum value from string representation
HttpMethod HttpMethodHelper::stringHttpMethodMap(const std::string &method) const
{
    if (_stringHttpMethodMap.find(method) != _stringHttpMethodMap.end())
    {
        return _stringHttpMethodMap.at(method);
    }
    else
    {
        throw UnknownMethodError(method);
    }
}

// Member function to check if a string is a valid HTTP method
bool HttpMethodHelper::isMethod(const std::string &method) const
{
    return _stringHttpMethodMap.find(method) != _stringHttpMethodMap.end();
}

// Member function to check if a string is a supported HTTP method
bool HttpMethodHelper::isSupportedMethod(const std::string &method) const
{
    // Iterate through the vector to find the method
    for (std::vector<std::string>::const_iterator it = this->_supportedMethods.begin();
         it != this->_supportedMethods.end();
         ++it)
    {
        // Compare the current method with the target method
        if (*it == method)
        {
            // Method found, return true
            return true;
        }
    }
    // Method not found, return false
    return false;
}

// Helper function to initialize MethodList with string representations of HTTP methods
std::vector<std::string> HttpMethodHelper::_setMethodList()
{
    std::vector<std::string> MethodList;

    // Add string representations of HTTP methods to MethodList
    MethodList.push_back("GET");
    MethodList.push_back("POST");
    MethodList.push_back("PUT");
    MethodList.push_back("DELETE");
    MethodList.push_back("HEAD");
    MethodList.push_back("OPTIONS");
    MethodList.push_back("PATCH");
    MethodList.push_back("TRACE");
    MethodList.push_back("CONNECT");

    return MethodList;
}

// Helper function to initialize stringHttpMethodMap with mappings from string representations to HttpMethod enum values
std::map<std::string, HttpMethod> HttpMethodHelper::_setStringHttpMethodMap()
{
    std::map<std::string, HttpMethod> stringHttpMethodMap;

    // Add mappings from string representations to HttpMethod enum values
    stringHttpMethodMap["GET"] = HttpMethod::GET;
    stringHttpMethodMap["POST"] = HttpMethod::POST;
    stringHttpMethodMap["PUT"] = HttpMethod::PUT;
    stringHttpMethodMap["DELETE"] = HttpMethod::DELETE;
    stringHttpMethodMap["HEAD"] = HttpMethod::HEAD;
    stringHttpMethodMap["OPTIONS"] = HttpMethod::OPTIONS;
    stringHttpMethodMap["PATCH"] = HttpMethod::PATCH;
    stringHttpMethodMap["TRACE"] = HttpMethod::TRACE;
    stringHttpMethodMap["CONNECT"] = HttpMethod::CONNECT;

    return stringHttpMethodMap;
}

// Helper function to initialize httpMethodStringMap with mappings from HttpMethod enum values to string representations
std::map<HttpMethod, std::string> HttpMethodHelper::_setHttpMethodStringMap()
{
    std::map<HttpMethod, std::string> httpMethodStringMap;

    // Add mappings from HttpMethod enum values to string representations
    httpMethodStringMap[HttpMethod::GET] = "GET";
    httpMethodStringMap[HttpMethod::POST] = "POST";
    httpMethodStringMap[HttpMethod::PUT] = "PUT";
    httpMethodStringMap[HttpMethod::DELETE] = "DELETE";
    httpMethodStringMap[HttpMethod::HEAD] = "HEAD";
    httpMethodStringMap[HttpMethod::OPTIONS] = "OPTIONS";
    httpMethodStringMap[HttpMethod::PATCH] = "PATCH";
    httpMethodStringMap[HttpMethod::TRACE] = "TRACE";
    httpMethodStringMap[HttpMethod::CONNECT] = "CONNECT";

    return httpMethodStringMap;
}

// Path: includes/constants/HttpMethodHelper.hpp
