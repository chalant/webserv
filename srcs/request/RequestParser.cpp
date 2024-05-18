#include "../../includes/request/RequestParser.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include "../../includes/constants/HttpStatusCodeHelper.hpp"
#include <cstdlib>
#include <algorithm>
#include <cctype>

/*
 * RequestParser.cpp
 *
 * The RequestParser class provides functionality for parsing raw HTTP requests and converting
 * them into IRequest objects.
 *
 */

// Constructor to initialize the RequestParser with required references
RequestParser::RequestParser(const IConfiguration &configuration, ILogger &logger)
    : _logger(logger),
      _configuration(configuration) {}

// Function to parse a raw HTTP request and convert it into a Request object
void RequestParser::parseRequest(const std::vector<char> &rawRequest, IRequest &parsedRequest) const
{
    this->_logger.log(DEBUG, "[REQUESTPARSER] Parsing request...");
    // Iterator to traverse the raw request
    std::vector<char>::const_iterator it = rawRequest.begin();

    // Parse the request line
    this->_parseRequestLine(it, rawRequest, parsedRequest);

    // Check for whitespace between request-line and first header field
    if (this->_isWhitespace(*it))
    {
        // throw '400' status error
        throw HttpStatusCodeException(BAD_REQUEST, "whitespace between the start-line and the first header field");
    }

    // Parse the headers
    this->_parseHeaders(it, rawRequest, parsedRequest);

    // Parse the body
    this->_parseBody(it, rawRequest, parsedRequest);

    // Parse the Upload BodyParameters
    if (parsedRequest.getHeaderValue(CONTENT_TYPE).find("multipart/form-data") != std::string::npos)
    {
        this->_parseBodyParameters(parsedRequest);
    }

    this->_logger.log(DEBUG, "[REQUESTPARSER] ...request parsed successfully");
}

// Function to parse the request line of an HTTP request
void RequestParser::_parseRequestLine(std::vector<char>::const_iterator &requestIterator,
                                      const std::vector<char> &rawRequest,
                                      IRequest &parsedRequest) const
{
    // Parse method, URI, and HTTP version
    std::string method = this->_parseMethod(requestIterator, rawRequest);
    std::string uri = this->_parseUri(requestIterator, rawRequest);
    std::string httpVersion = this->_parseHttpVersion(requestIterator, rawRequest);

    // Set method, URI, and HTTP version in the parsed request
    parsedRequest.setMethod(method);
    parsedRequest.setUri(uri);
    parsedRequest.setHttpVersion(httpVersion);
}

// Function to parse the HTTP method from the request line
std::string RequestParser::_parseMethod(std::vector<char>::const_iterator &requestIterator,
                                        const std::vector<char> &rawRequest) const
{
    std::string method;

    // Skip leading CRLF
    while (this->_isCRLF(requestIterator))
    {
        requestIterator += 2;
    }

    // Build 'method' string until space is found
    while (requestIterator != rawRequest.end() && *requestIterator != ' ')
    {
        method += *requestIterator;
        ++requestIterator;
    }

    // Check for unexpected end of request
    if (requestIterator == rawRequest.end())
    {
        throw HttpStatusCodeException(BAD_REQUEST, "Unexpected end of request"); // throw '400' status error
    }

    // Move marker to next character
    ++requestIterator;

    // Log 'method'
    this->_logger.log(VERBOSE, "[REQUESTPARSER] Method: \"" + method + "\"");

    // Return 'method' string
    return method;
}

// Function to parse the URI from the request line
std::string RequestParser::_parseUri(std::vector<char>::const_iterator &requestIterator,
                                     const std::vector<char> &rawRequest) const
{
    std::string uri;

    // Build 'uri' string until space is found
    while (requestIterator != rawRequest.end() && *requestIterator != ' ')
    {
        uri += *requestIterator;
        ++requestIterator;
    }

    // Check for unexpected end of request
    if (requestIterator == rawRequest.end())
    {
        throw HttpStatusCodeException(BAD_REQUEST, "Unexpected end of request"); // throw '400' status error
    }

    // Move marker to next character
    ++requestIterator;

    // Log 'uri'
    this->_logger.log(VERBOSE, "[REQUESTPARSER] URI: \"" + uri + "\"");

    // Return 'uri' string
    return uri;
}

// Function to parse the HTTP version from the request line
std::string RequestParser::_parseHttpVersion(std::vector<char>::const_iterator &requestIterator,
                                             const std::vector<char> &rawRequest) const
{
    std::string httpVersion;

    // Build 'httpVersion' string
    while (requestIterator != rawRequest.end())
    {
        // End of 'httpVersion' string, break loop
        if (this->_isCRLF(requestIterator))
            break;

        // Check for invalid characters
        if (this->_isCharInSet(requestIterator, "\r\n"))
        {
            throw HttpStatusCodeException(BAD_REQUEST, "Invalid characters in HTTP version"); // throw '400' status error
        }

        // Append character to 'httpVersion' string
        httpVersion += *requestIterator;
        ++requestIterator;
    }

    // Check for unexpected end of request
    if (requestIterator == rawRequest.end())
    {
        throw HttpStatusCodeException(BAD_REQUEST, "Unexpected end of request"); // throw '400' status error
    }

    // Move marker passed CRLF
    requestIterator += 2;

    // Log 'httpVersion'
    this->_logger.log(VERBOSE, "[REQUESTPARSER] HTTP Version: \"" + httpVersion + "\"");

    // Return 'httpVersion' string
    return httpVersion;
}

// Function to parse the headers of an HTTP request
void RequestParser::_parseHeaders(std::vector<char>::const_iterator &requestIterator,
                                  const std::vector<char> &rawRequest,
                                  IRequest &parsedRequest) const
{
    // Parse headers
    while (requestIterator != rawRequest.end())
    {
        // End of headers, break loop
        if (this->_isCRLF(requestIterator))
            break;

        // Check for invalid characters
        if (this->_isCharInSet(requestIterator, "\r\n"))
        {
            throw HttpStatusCodeException(BAD_REQUEST, "Invalid characters in header");
        }

        // Parse individual header
        this->_parseHeader(requestIterator, rawRequest, parsedRequest);
    }

    // Check for unexpected end of request
    if (requestIterator == rawRequest.end())
    {
        throw HttpStatusCodeException(BAD_REQUEST, "Unexpected end of request");
    }

    // Set authority in parsed request
    parsedRequest.setAuthority();

    // Move marker passed CRLF
    requestIterator += 2;
}

// Function to parse an individual header
void RequestParser::_parseHeader(std::vector<char>::const_iterator &requestIterator,
                                 const std::vector<char> &rawRequest,
                                 IRequest &parsedRequest) const
{
    // Parse header name and value
    std::string headerName;
    std::string headerValue;

    // Set start value for client header buffer size
    int clientHeaderBufferSize = this->_configuration.getInt("ClientHeaderBufferSize");

    // Find colon to separate header name and value
    while (requestIterator != rawRequest.end() && *requestIterator != ':')
    {
        headerName += std::tolower(*requestIterator); // Convert to lowercase
        clientHeaderBufferSize--;
        ++requestIterator;
    }

    // Check if colon was found
    if (requestIterator == rawRequest.end())
    {
        throw HttpStatusCodeException(BAD_REQUEST, "Colon not found in header");
    }

    // Skip colon
    ++requestIterator;

    // Skip optional space
    if (requestIterator != rawRequest.end() && *requestIterator == ' ')
    {
        ++requestIterator;
    }

    // Find end of header value
    while (requestIterator != rawRequest.end() && !this->_isCRLF(requestIterator))
    {
        headerValue += std::tolower(*requestIterator); // Convert to lowercase
        clientHeaderBufferSize--;
        ++requestIterator;
    }

    // Check for unexpected end of request
    if (requestIterator == rawRequest.end())
    {
        throw HttpStatusCodeException(BAD_REQUEST, "Unexpected end of request");
    }
    // Check if header size exceeds client header buffer size
    if (clientHeaderBufferSize < 0)
    {
        throw HttpStatusCodeException(REQUEST_HEADER_FIELDS_TOO_LARGE, "Header fields too large");
    }

    // Move marker passed CRLF
    requestIterator += 2;

    // Log header
    this->_logger.log(VERBOSE, "[REQUESTPARSER] Header: \"" + headerName + ": " + headerValue + "\"");

    // Add header to parsed request
    try
    {
        parsedRequest.addHeader(headerName, headerValue);
    }
    catch (const UnknownHeaderError &e)
    {
        // Log and continue
        this->_logger.log(WARN, e.what());
    }

    // Parse cookies
    if (headerName == "cookie")
    {
        this->_parseCookie(headerValue, parsedRequest);
    }
}
// Function to parse the body of an HTTP request
void RequestParser::_parseBody(std::vector<char>::const_iterator &requestIterator,
                               const std::vector<char> &rawRequest,
                               IRequest &parsedRequest) const
{
    // If method is not POST or PUT, no need to parse body
    if (parsedRequest.getMethod() != POST && parsedRequest.getMethod() != PUT)
        return; // No need to parse body for other methods

    // Check if 'Transfer-Encoding' is chunked
    std::string transferEncoding = parsedRequest.getHeaderValue(TRANSFER_ENCODING);
    if (transferEncoding == "chunked")
    {
        // Handle chunked encoding
        std::vector<char> body = this->_unchunkBody(requestIterator, rawRequest);

        // Set unchunked body in parsed request
        parsedRequest.setBody(body);

        return;
    }

    // Check if 'content-length' header is missing
    // Get body size
    std::string contentLengthString = parsedRequest.getHeaderValue(CONTENT_LENGTH);
    if (contentLengthString.empty())
    {
        // throw '411' status error
        throw HttpStatusCodeException(LENGTH_REQUIRED, "no content-length header found");
    }

    // Check if conversion was successful
    // Get 'content-length' value
    size_t bodySize = atoi(contentLengthString.c_str());
    if (bodySize <= 0)
    {
        // throw '400' status error
        throw HttpStatusCodeException(BAD_REQUEST, "content-length header conversion failed (" + contentLengthString + ")");
    }

    // Check if body size exceeds client body buffer size
    if (bodySize > this->_configuration.getSize_t("ClientBodyBufferSize"))
    {
        // throw '413' status error
        throw HttpStatusCodeException(PAYLOAD_TOO_LARGE);
    }

    // Check if body size exceeds remaining request size
    size_t remainingRequestSize = rawRequest.end() - requestIterator;
    if (remainingRequestSize < bodySize)
    {
        // throw '400' status error
        throw HttpStatusCodeException(BAD_REQUEST, "body size exceeds remaining request size");
    }

    // Extract body
    std::vector<char> body(requestIterator, requestIterator + bodySize);

    // Log body
    this->_logger.log(VERBOSE, "[REQUESTPARSER] Body: \"" + std::string(body.begin(), body.end()) + "\"");

    // Set body in parsed request
    parsedRequest.setBody(body);
}

// Function to unchunk the body of an HTTP request
std::vector<char> RequestParser::_unchunkBody(std::vector<char>::const_iterator &requestIterator,
                                              const std::vector<char> &rawRequest) const
{
    // Initialize body vector
    std::vector<char> body;

    // Set Max Size
    size_t remainingRequestSize = this->_configuration.getSize_t("ClientBodyBufferSize");

    // Parse chunks
    while (requestIterator != rawRequest.end())
    {
        // Get chunk size
        std::string chunkSizeString;
        while (*requestIterator != '\r')
        {
            chunkSizeString += *requestIterator;
            ++requestIterator;
        }

        // Check for last chunk
        if (chunkSizeString == "0")
            break;

        // Move marker passed CRLF
        requestIterator += 2;

        // Check for unexpected end of request
        if (requestIterator == rawRequest.end())
        {
            // throw '400' status error
            throw HttpStatusCodeException(BAD_REQUEST, "Unexpected end of request");
        }

        // Get chunk size
        size_t chunkSize = strtol(chunkSizeString.c_str(), NULL, 16);

        // Check if chunk size is valid
        if (chunkSize <= 0)
        {
            // throw '400' status error
            throw HttpStatusCodeException(BAD_REQUEST, "chunk size conversion failed (" + chunkSizeString + ")");
        }

        // Check if body size exceeds maximum client body buffer size
        // remainingRequestSize -= chunkSize;
        if (remainingRequestSize < chunkSize)
        {
            // throw '413' status error
            throw HttpStatusCodeException(PAYLOAD_TOO_LARGE);
        }
        remainingRequestSize -= chunkSize;
        // Append chunk to body
        body.insert(body.end(), requestIterator, requestIterator + chunkSize);

        // Move marker passed CRLF
        requestIterator += 2;
    }

    // Log body
    this->_logger.log(VERBOSE, "[REQUESTPARSER] Body: \"" + std::string(body.begin(), body.end()) + "\"");

    // Return body
    return body;
}

// Function to parse cookies from the request
void RequestParser::_parseCookie(std::string &cookieHeaderValue,
                                 IRequest &parsedRequest) const
{
    // Parse cookies
    std::string cookieName;
    std::string cookieValue;

    // Create a stream from the cookie header value
    std::istringstream cookieStream(cookieHeaderValue);

    // Parse cookie name and value
    while (std::getline(cookieStream, cookieName, '=') && std::getline(cookieStream, cookieValue, ';'))
    {
        // Trim surrounding whitespace
        cookieName = this->_trimWhitespace(cookieName);
        cookieValue = this->_trimWhitespace(cookieValue);

        // Add cookie to parsed request
        parsedRequest.addCookie(cookieName, cookieValue);
    }
}

void RequestParser::_parseBodyParameters(IRequest &parsedRequest) const
{
    // Get the boundary string
    std::string contentType = parsedRequest.getHeaderValue(CONTENT_TYPE);
    std::string boundary = "--" + contentType.substr(contentType.find("boundary=") + 9);

    // Get body stream
    std::vector<char> body = parsedRequest.getBody();
    std::string bodyString(body.begin(), body.end());
    std::istringstream bodyStream(bodyString);

    // Declare a line string
    std::string line;

    // Parse BodyParameters
    while (std::getline(bodyStream, line))
    {
        // Skip leading newline
        if (line.empty())
            continue;

        // Skip boundary
        if (line.find(boundary) != std::string::npos)
            continue;

        BodyParameter bodyParameter;

        // Parse BodyParameter headers
        do
        {
            std::string::size_type pos = line.find(':');
            if (pos != std::string::npos)
            {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                key = this->_trimWhitespace(key);
                value = this->_trimWhitespace(value);
                // lower cases the key
                std::transform(key.begin(), key.end(), key.begin(), static_cast<int (*)(int)>(std::tolower));

                bodyParameter.headers[key] = value;

                // Parse dispositionType, contentType, and fieldName
                if (key == "content-disposition")
                {
                    std::istringstream iss(value);
                    std::string token;
                    while (std::getline(iss, token, ';'))
                    {
                        size_t pos = token.find('=');
                        if (pos != std::string::npos)
                        {
                            std::string param = token.substr(0, pos);
                            std::string paramValue = token.substr(pos + 1);
                            param = this->_trimWhitespace(param);
                            paramValue = this->_trimWhitespace(paramValue);

                            if (param == "form-data")
                                bodyParameter.dispositionType = paramValue;
                            else if (param == "name")
                                bodyParameter.fieldName = paramValue;
                        }
                    }
                }
                else if (key == "content-type")
                    bodyParameter.contentType = value;
            }
        } while (std::getline(bodyStream, line) && !line.empty());

        // Parse BodyParameter data
        while (std::getline(bodyStream, line) && line != boundary)
        {
            bodyParameter.data += line + '\n'; // Append the line to the BodyParameter data
        }

        // Trim whitespace after loop
        bodyParameter.data = this->_trimWhitespace(bodyParameter.data);

        // Add BodyParameter to vector
        parsedRequest.addBodyParameter(bodyParameter);
    }

    // Mark the request as an upload request
    parsedRequest.setUploadRequest(true);
}

// Function to check if a character is whitespace
bool RequestParser::_isWhitespace(char c) const
{
    return c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r' || c == '\n';
}

// Function to check if an iterator points to CRLF (carriage return + line feed)
bool RequestParser::_isCRLF(std::vector<char>::const_iterator it) const
{
    return *it == '\r' && *(it + 1) == '\n';
}

// Function to check if an iterator points to a character that is in a given set
bool RequestParser::_isCharInSet(std::vector<char>::const_iterator it, const std::string &set) const
{
    return set.find(*it) != std::string::npos;
}

// Function to trim whitespace
std::string RequestParser::_trimWhitespace(const std::string &string) const
{
    std::string result = string;

    // Trim leading whitespace
    size_t start = result.find_first_not_of(" \t");
    if (start != std::string::npos)
    {
        result = result.substr(start);
    }

    // Trim trailing whitespace
    size_t end = result.find_last_not_of(" \t");
    if (end != std::string::npos)
    {
        result = result.substr(0, end + 1);
    }

    // Return trimmed string
    return result;
}

// path: srcs/RequestHandler.cpp
