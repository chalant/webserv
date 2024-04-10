#ifndef REQUEST_HPP
#define REQUEST_HPP

/*
 * Request: Represents an HTTP request.
 *
 * This class encapsulates the components of an HTTP request, including the request method, Uri,
 * HTTP version, requestHeaders, and an optional body. It provides getter and setter methods to
 * access and modify these components, following the HTTP protocol specifications.
 *
 * It takes responsability to verifying the validity of its components and throws exceptions if
 * any component is invalid.
 *
 * Instances of this class are typically created by the RequestParser class, which parses raw
 * HTTP request strings and constructs Request objects from them.
 *
 * Usage:
 * - Use getter methods to retrieve the properties of the request.
 *
 * Example:
 * request.getMethod(); // Get the request method
 * request.getUri(); // Get the request URI
 * request.getHttpVersion(); // Get the HTTP version
 * request.getHeaders(); // Get the request headers
 * request.getBody(); // Get the request body
 *
 * NOTE: for string representations use the string getters where available:
 * request.getMethodString(); // Get the request method as a string
 * request.getHttpVersionString(); // Get the HTTP version as a string
 * request.getHeadersStrings(); // Get the request headers as a string-keyed map
 *
 */

#include <string>
#include <vector>
#include <map>
#include "constants/RequestHelper.hpp"
#include "WebservExceptions.hpp"
#include "Configuration.hpp"

class Request
{
private:
    // Request line components
    HttpMethod _method;
    std::string _uri;
    HttpVersion _httpVersion;
    // Request headers
    std::map<HttpHeader, std::string> _headers;
    // Request body
    std::vector<char> _body;
    // Included Request parameters
    std::map<std::string, std::string> _queryParameters;
    std::pair<std::string, std::string> _remoteAddress;
    time_t _requestTimestamp;
    std::map<std::string, std::string> _cookies;
    std::map<std::string, std::string> _bodyParameters;
    std::string _requestId;
    std::string _rawRequest;
    const Configuration &_configuration;
    // Helper;
    const RequestHelper &_requestHelper;

public:
    Request(const RequestHelper &requestHelper, const Configuration &configuration);
    Request &operator=(const Request &src);
    ~Request();
    HttpMethod getMethod() const;
    std::string getMethodString() const;
    std::string getUri() const;
    HttpVersion getHttpVersion() const;
    std::string getHttpVersionString() const;
    const std::map<HttpHeader, std::string> getHeaders() const;
    const std::map<std::string, std::string> getHeadersStrings() const;
    const std::vector<char> getBody() const;
    std::string getBodyString() const;
    void setMethod(const std::string &method);
    void setUri(const std::string &uri);
    void setHttpVersion(const std::string &httpVersion);
    void addHeader(const std::string key, std::string value);
    void setBody(const std::vector<char> &body);
};

#endif // REQUEST_HPP
       // Path: includes/Response.hpp
