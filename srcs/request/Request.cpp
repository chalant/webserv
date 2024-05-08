#include "../../includes/request/Request.hpp"
#include "../../includes/exception/WebservExceptions.hpp"

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

// Constructor initializes the Request object with a HttpHelper and a IConfiguration object
Request::Request(const IConfiguration &configuration, const HttpHelper &httpHelper)
    : _configuration(configuration),
      _httpHelper(httpHelper) {}

// Copy assignment operator
Request &Request::operator=(const Request &src)
{
    // Assignment operator for copying the contents of another Request object
    if (this == &src)
        return *this; // Check for self-assignment
    // Copy individual member variables from src to this object
    this->_method = src._method;
    this->_uri = src._uri;
    this->_httpVersion = src._httpVersion;
    this->_headers = src._headers;
    this->_body = src._body;
    return *this;
}

// Copy constructor
Request::Request(const Request &src) : _configuration(src._configuration),
                                       _httpHelper(src._httpHelper)
{
    // Copy constructor for copying the contents of another Request object
    *this = src; // Use the assignment operator to copy the contents
}

// Destructor
Request::~Request() {}

// Getter function for retrieving the HTTP method of the request
HttpMethod Request::getMethod() const
{
    return this->_method;
}

// Getter function for retrieving the string representation of the HTTP method
std::string Request::getMethodString() const
{
    return this->_httpHelper.httpMethodStringMap(this->_method);
}

// Getter function for retrieving the URI of the request
std::string Request::getUri() const
{
    return this->_uri;
}

// Getter function for retrieving the HTTP version of the request
HttpVersion Request::getHttpVersion() const
{
    return this->_httpVersion;
}

// Getter function for retrieving the string representation of the HTTP version
std::string Request::getHttpVersionString() const
{
    return this->_httpHelper.httpVersionStringMap(this->_httpVersion);
}

// Getter function for retrieving the headers of the request
const std::map<HttpHeader, std::string> Request::getHeaders() const
{
    return this->_headers;
}

// Getter function for retrieving the headers of the request as string-keyed map
std::map<std::string, std::string> Request::getHeadersStringMap() const
{
    std::map<std::string, std::string> headersStrings;
    for (std::map<HttpHeader, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); ++it)
    {
        // Convert enum keys to string keys and copy values
        headersStrings[this->_httpHelper.httpHeaderStringMap(it->first)] = it->second;
    }
    return headersStrings;
}

// Getter function for retrieving the query parameters
std::map<std::string, std::string> Request::getQueryParameters() const
{
    return this->_queryParameters;
}

// Getter function for retrieving all the cookies
std::map<std::string, std::string> Request::getCookies() const
{
    return this->_cookies;
}

// Getter function for retrieving a specific cookie
std::string Request::getCookie(const std::string &key) const
{
    // Check if the cookie exists
    if (this->_cookies.find(key) != this->_cookies.end())
        return this->_cookies.at(key); // Return the value of the cookie
    else
        return ""; // Return an empty string if the cookie does not exist
}

// Getter function for retrieving the value of a specific header
std::string Request::getHeaderValue(HttpHeader header) const
{
    // Check if the header exists in the map
    if (this->_headers.find(header) != this->_headers.end())
        return this->_headers.at(header); // Return the value of the header
    else
        return ""; // Return an empty string if the header does not exist
}

// Getter function for retrieving the body of the request
const std::vector<char> Request::getBody() const
{
    return this->_body;
}

// Getter function for retrieving the body of the request as a string
std::string Request::getBodyString() const
{
    return std::string(this->_body.begin(), this->_body.end());
}

// Getter function for retrieving the query string
std::string Request::getQueryString() const
{
    // Check if the URI contains a query string
    size_t queryPos = this->_uri.find('?');
    if (queryPos != std::string::npos)
    {
        // Return the query string
        return this->_uri.substr(queryPos + 1);
    }
    else
    {
        return ""; // Return an empty string if the URI does not contain a query string
    }
}

// Getter function for retrieving the content length
std::string Request::getContentLength() const
{
    // Check if the 'Content-Length' header exists in the map
    if (this->_headers.find(CONTENT_LENGTH) != this->_headers.end())
    {
        // Return the value of the 'Content-Length' header
        return this->_headers.at(CONTENT_LENGTH);
    }
    else
    {
        return ""; // Return an empty string if the 'Content-Length' header does not exist
    }
}

// Getter function for retrieving the content type
std::string Request::getContentType() const
{
    // Check if the 'Content-Type' header exists in the map
    if (this->_headers.find(CONTENT_TYPE) != this->_headers.end())
    {
        // Return the value of the 'Content-Type' header
        return this->_headers.at(CONTENT_TYPE);
    }
    else
    {
        return ""; // Return an empty string if the 'Content-Type' header does not exist
    }
}

// Getter function for retrieving the path info
std::string Request::getPathInfo(const std::string &scriptName) const
{
    // Check if the URI contains the script name
    size_t scriptNamePos = this->_uri.find(scriptName);
    if (scriptNamePos == std::string::npos)
        return ""; // Return an empty string if the URI does not contain the script name

    // Calculate the start of the path info
    size_t pathInfoStart = scriptNamePos + scriptName.size();

    // Get the start of the query string, if present
    size_t queryPos = this->_uri.find('?', pathInfoStart);

    // Return the path info
    if (queryPos != std::string::npos)
    {
        // Return the path info excluding the query string
        return this->_uri.substr(pathInfoStart, queryPos - pathInfoStart);
    }
    else
    {
        // Return the remaining part of the URI as path info
        return this->_uri.substr(pathInfoStart);
    }
}

// Getter function for retrieving the client IP address
std::string Request::getClientIp() const
{
    // Check if the 'X-Forwarded-For' header exists in the map
    if (this->_headers.find(X_FORWARDED_FOR) != this->_headers.end())
    {
        // Return the value of the 'X-Forwarded-For' header
        return this->_headers.at(X_FORWARDED_FOR);
    }
    else
    {
        // Return the value of the 'REMOTE_ADDR' header
        return this->_remoteAddress.second;
    }
}

// Getter function for retrieving the host name
std::string Request::getHostName() const
{
    return this->_hostName;
}

// Getter function for retrieving the host port
std::string Request::getHostPort() const
{
    return this->_hostPort;
}

// Getter function for retrieving the authority of the request
std::string Request::getAuthority() const
{
    return this->_authority;
}

// Getter function for retrieving the body parameters
const std::vector<BodyParameter> &Request::getBodyParameters() const
{
    return this->_bodyParameters;
}

// Getter function for checking if the request is an upload request
bool Request::isUploadRequest() const
{
    return this->_uploadRequest;
}

// Setter function for setting the method of the request
void Request::setMethod(const std::string &method)
{
    if (this->_httpHelper.isMethod(method) == false)
        throw HttpStatusCodeException(METHOD_NOT_ALLOWED, // Throw '405' status error
                                      "unknown method: \"" + method + "\"");
    else if (this->_httpHelper.isSupportedMethod(method) == false)
        throw HttpStatusCodeException(NOT_IMPLEMENTED, // Throw '501' status error
                                      "unsupported method: \"" + method + "\"");

    // Set the method of the request
    this->_method = this->_httpHelper.stringHttpMethodMap(method);
}

// Setter function for setting the URI of the request
void Request::setUri(const std::string &uri)
{
    // Check if the URI size exceeds the maximum allowed URI size
    if (uri.size() > this->_configuration.getSize_t("ClientMaxUriSize"))
        throw HttpStatusCodeException(URI_TOO_LONG); // Throw '414' status error

    // Check if the URI contains any whitespace characters
    if (uri.find_first_of(" \t") != std::string::npos)
        throw HttpStatusCodeException(BAD_REQUEST, // Throw '400' status error
                                      "whitespace in URI");

    // Set the URI
    this->_uri = uri;
}

// Setter function for setting the HTTP version of the request
void Request::setHttpVersion(const std::string &httpVersion)
{
    // Set the HTTP version of the request
    // Use the HttpHelper to map the string representation of the HTTP version to an HttpVersion enum value
    // If the HTTP version is not recognized, an UnknownHttpVersionError is thrown
    this->_httpVersion = this->_httpHelper.stringHttpVersionMap(httpVersion);
}

// Function for adding a header to the request
void Request::addHeader(const std::string &key, const std::string &value)
{
    // Check if the key contains trailing whitespace
    if (!key.empty() && (key.back() == ' ' || key.back() == '\t'))
    {
        throw HttpStatusCodeException(BAD_REQUEST, "trailing whitespace in header key");
    }

    // Convert the key to lowercase
    std::string lowercaseKey = key;
    for (std::string::iterator it = lowercaseKey.begin(); it != lowercaseKey.end(); ++it)
    {
        *it = std::tolower(static_cast<unsigned char>(*it));
    }

    // Check if key exists in the map
    HttpHeader name;
    try
    {
        name = this->_httpHelper.stringHttpHeaderMap(lowercaseKey);
        // Add the header to the internal headers map
        this->_headers[name] = value;
    }
    catch (const UnknownHeaderError &e)
    {
        // Re-throw with original key
        throw UnknownHeaderError(key); // We will skip to the next header
    }
}

// Setter function for setting the body of the request
void Request::setBody(const std::vector<char> &body)
{
    // Check if the body is empty
    if (body.empty())
        return; // If empty, do nothing (no body to set)

    // Check if the body size exceeds the maximum allowed body size
    if (body.size() > this->_configuration.getSize_t("ClientMaxBodySize"))
        throw HttpStatusCodeException(PAYLOAD_TOO_LARGE); // Throw '413' status error

    // Set the body of the request
    this->_body = body;
}

// Setter function for setting the body of the request as a string
void Request::setBody(const std::string &body)
{
    // Convert the body string to a vector of characters
    std::vector<char> bodyVector(body.begin(), body.end());

    // Set the body of the request
    this->setBody(bodyVector);
}

// Function for adding a cookie to the request
void Request::addCookie(const std::string &key, const std::string &value)
{
    // Add the cookie to the internal cookies map
    this->_cookies[key] = value;
}

// Setter function for setting the authority of the request
void Request::setAuthority()
{
    // Check if the 'Host' header exists in the map
    if (this->_headers[HOST] == "")
        throw HttpStatusCodeException(BAD_REQUEST, // Throw '400' status error
                                      "missing Host header");
    std::istringstream hostStream(this->_headers[HOST]);
    if (std::getline(hostStream, this->_hostName, ':'))
    {
        // If the host header contains a port number, set the host port
        std::getline(hostStream, this->_hostPort);
    }
    else
    {
        // If the host header does not contain a port number, set the host port to the default port
        this->_hostPort = this->_configuration.getString("DefaultPort");
    }

    // Set the authority of the request
    this->_authority = this->_hostName + ":" + this->_hostPort;
}

// Function for adding a body parameter to the request
void Request::addBodyParameter(const BodyParameter &bodyParameter)
{
    // Add the body parameter to the internal body parameters vector
    this->_bodyParameters.push_back(bodyParameter);
}

// Setter function for setting the upload request flag
void Request::setUploadRequest(bool uploadRequest)
{
    this->_uploadRequest = uploadRequest;
}

// path: srcs/request/Request.cpp
