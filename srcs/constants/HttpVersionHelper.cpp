#include "../includes/constants/HttpVersionHelper.hpp"

/*
 * HttpVersionHelper.hpp
 *
 * The HttpVersionHelper class provides functionality for working with HTTP versions.
 * It defines an enum HttpVersion to represent standard HTTP versions and provides methods
 * to retrieve string representations of versions and vice versa.
 *
 */

// Constructor initializes member variables using helper functions
HttpVersionHelper::HttpVersionHelper() 
    : _versionList(_setVersionList()), _stringHttpVersionMap(_setStringHttpVersionMap()), _httpVersionStringMap(_setHttpVersionStringMap()) {}

// Get string representation of HttpVersion enum value
const std::string &HttpVersionHelper::httpVersionStringMap(HttpVersion version) const
{
    if (_httpVersionStringMap.find(version) != _httpVersionStringMap.end())
    {
        return _httpVersionStringMap.at(version);
    }
    else
    {
        throw UnknownHttpVersionError();
    }
}

// Get HttpVersion enum value from string representation
HttpVersion HttpVersionHelper::stringHttpVersionMap(const std::string &version) const
{
    if (_stringHttpVersionMap.find(version) != _stringHttpVersionMap.end())
    {
        return _stringHttpVersionMap.at(version);
    }
    else
    {
        throw UnknownHttpVersionError(version);
    }
}

// Helper function to initialize VersionList with string representations of HTTP versions
std::vector<std::string> HttpVersionHelper::_setVersionList()
{
    std::vector<std::string> VersionList;

    // Add string representations of HTTP versions to VersionList
    VersionList.push_back("HTTP/1.0");
    VersionList.push_back("HTTP/1.1");
    VersionList.push_back("HTTP/2.0");

    return VersionList;
}

// Helper function to initialize stringHttpVersionMap with mappings from string representations to HttpVersion enum values
std::map<std::string, HttpVersion> HttpVersionHelper::_setStringHttpVersionMap()
{
    std::map<std::string, HttpVersion> stringHttpVersionMap;

    // Add mappings from string representations to HttpVersion enum values
    stringHttpVersionMap["HTTP/1.0"] = HttpVersion::HTTP_1_0;
    stringHttpVersionMap["HTTP/1.1"] = HttpVersion::HTTP_1_1;
    stringHttpVersionMap["HTTP/2.0"] = HttpVersion::HTTP_2_0;

    return stringHttpVersionMap;
}

// Helper function to initialize httpVersionStringMap with mappings from HttpVersion enum values to string representations
std::map<HttpVersion, std::string> HttpVersionHelper::_setHttpVersionStringMap()
{
    std::map<HttpVersion, std::string> httpVersionStringMap;

    // Add mappings from HttpVersion enum values to string representations
    httpVersionStringMap[HttpVersion::HTTP_1_0] = "HTTP/1.0";
    httpVersionStringMap[HttpVersion::HTTP_1_1] = "HTTP/1.1";
    httpVersionStringMap[HttpVersion::HTTP_2_0] = "HTTP/2.0";

    return httpVersionStringMap;
}

// Path: srcs/constants/HttpMethodHelper.cpp