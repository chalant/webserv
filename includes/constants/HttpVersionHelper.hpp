#ifndef HTTPVERSIONHELPER_HPP
#define HTTPVERSIONHELPER_HPP

/*
 * HttpVersionHelper.hpp
 *
 * The HttpVersionHelper class provides functionality for working with HTTP versions.
 * It defines an enum HttpVersion to represent standard HTTP versions and provides methods
 * to retrieve string representations of versions and vice versa.
 *
 * Example:
 * 
 * HttpVersionHelper helper;
 * std::string version = "HTTP/1.1";
 * HttpVersion httpVersion = helper.stringHttpVersionMap(version);
 * std::string version = helper.httpVersionStringMap(httpVersion);
 * 
 * std::string version = "HTTP/3.0";
 * if (helper.isHttpVersion(version)) {std::cout << "Valid HTTP version." << std::endl;}
 * 
 * Note: Invalid input to Map searches will throw an UnknownHttpVersionError exception.
 * 
 * IMPORTANT: Version strings representations in this class are all UPPERCASE.
 */

#include <string>
#include <vector>
#include <map>
#include "../WebservExceptions.hpp"

enum HttpVersion
{
    HTTP_1_0 = 10, // HTTP/1.0
    HTTP_1_1 = 11, // HTTP/1.1
    HTTP_2_0 = 20  // HTTP/2.0
};

class HttpVersionHelper
{
private:
    // Member variables
    const std::vector<std::string> _versionList;                    // List of string representations of HTTP versions
    const std::map<std::string, HttpVersion> _stringHttpVersionMap; // Map of string representations to HttpVersion enum values
    const std::map<HttpVersion, std::string> _httpVersionStringMap; // Map of HttpVersion enum values to string representations

    // Private member functions for initialization
    static std::vector<std::string> _setVersionList();
    static std::map<std::string, HttpVersion> _setStringHttpVersionMap();
    static std::map<HttpVersion, std::string> _setHttpVersionStringMap();

public:
    // Constructor
    HttpVersionHelper();

    // Member functions to access data
    const std::string &httpVersionStringMap(HttpVersion version) const; // Get string representation of HttpVersion
    HttpVersion stringHttpVersionMap(const std::string &version) const; // Get HttpVersion enum value from string representation

    // Member function to check if a string represents a valid HTTP version
    bool isHttpVersion(const std::string &version) const;
};

#endif // HTTPVERSIONHELPER_HPP
// Path: includes/constants/HttpVersionHelper.hpp
