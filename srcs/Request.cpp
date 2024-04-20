#include "Request.hpp"

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

// Constructor initializes the Request object with a RequestHelper and a IConfiguration object
Request::Request(const RequestHelper &requestHelper, const IConfiguration &configuration)
    : _configuration(configuration),
      _requestHelper(requestHelper) {}

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
                                       _requestHelper(src._requestHelper)
{
    // Copy constructor for copying the contents of another Request object
    *this = src; // Use the assignment operator to copy the contents
}

// Destructor
Request::~Request() {}

// Clear function to reset the Request object
void Request::clear()
{
    // Reset all member variables to their default values
    this->_method = GET;
    this->_uri = "";
    this->_httpVersion = HTTP_1_1;
    this->_headers.clear();
    this->_body.clear();
}

// Getter function for retrieving the HTTP method of the request
HttpMethod Request::getMethod() const
{
    return this->_method;
}

// Getter function for retrieving the string representation of the HTTP method
std::string Request::getMethodString() const
{
    return this->_requestHelper.httpMethodStringMap(this->_method);
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
    return this->_requestHelper.httpVersionStringMap(this->_httpVersion);
}

// Getter function for retrieving the headers of the request
const std::map<HttpHeader, std::string> Request::getHeaders() const
{
    return this->_headers;
}

// Getter function for retrieving the headers of the request as string-keyed map
std::map<std::string, std::string> Request::getHeadersString() const
{
    std::map<std::string, std::string> headersStrings;
    for (std::map<HttpHeader, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); ++it)
    {
        // Convert enum keys to string keys and copy values
        headersStrings[this->_requestHelper.httpHeaderStringMap(it->first)] = it->second;
    }
    return headersStrings;
}

// Getter function for retrieving the query parameters of the request
std::map<std::string, std::string> Request::getQueryParameters() const
{
    return this->_queryParameters;
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

// Setter function for setting the method of the request
void Request::setMethod(const std::string &method)
{
    if (this->_requestHelper.isMethod(method) == false)
        throw HttpStatusCodeException(METHOD_NOT_ALLOWED, // Throw '405' status error
                                      "unknown method: \"" + method + "\"");
    else if (this->_requestHelper.isSupportedMethod(method) == false)
        throw HttpStatusCodeException(NOT_IMPLEMENTED, // Throw '501' status error
                                      "unsupported method: \"" + method + "\"");

    // Set the method of the request
    this->_method = this->_requestHelper.stringHttpMethodMap(method);
}

// Setter function for setting the URI of the request
void Request::setUri(const std::string &uri)
{
    // Check if the URI size exceeds the maximum allowed URI size
    if (uri.size() > this->_configuration.getInt("ClientMaxUriSize"))
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
    // Use the RequestHelper to map the string representation of the HTTP version to an HttpVersion enum value
    // If the HTTP version is not recognized, an UnknownHttpVersionError is thrown
    this->_httpVersion = this->_requestHelper.stringHttpVersionMap(httpVersion);
}

// Function for adding a header to the request
void Request::addHeader(const std::string &key, const std::string &value)
{
    // Check if the value contains any whitespace characters
    if (value.find_first_of(" \t") != std::string::npos)
    {
        throw HttpStatusCodeException(BAD_REQUEST, // Throw '400' status error
                                      "whitespace in header value");
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
        name = this->_requestHelper.stringHttpHeaderMap(lowercaseKey);
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
    if (body.size() > this->_configuration.getInt("ClientMaxBodySize"))
        throw HttpStatusCodeException(PAYLOAD_TOO_LARGE); // Throw '413' status error

    // Set the body of the request
    this->_body = body;
}
