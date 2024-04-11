#include "../includes/RequestParser.hpp"

/*
 * RequestParser.cpp
 *
 * The RequestParser class provides functionality for parsing raw HTTP requests and converting
 * them into Request objects.
 *
 */

// Constructor to initialize the RequestParser with required references
RequestParser::RequestParser(const Configuration &configuration, Logger &errorLogger, const ExceptionHandler &exceptionHandler)
    : _configuration(configuration),
      _errorLogger(errorLogger),
      _exceptionHandler(exceptionHandler),
      _requestHelper(configuration) {}

// Function to parse a raw HTTP request and convert it into a Request object
Request RequestParser::parseRequest(const std::vector<char> &rawRequest) const
{
    // Request object to store parsed request
    Request parsedRequest(this->_requestHelper, this->_configuration);

    // Iterator to traverse the raw request
    std::vector<char>::const_iterator it = rawRequest.begin();

    // Parse the request line
    this->_parseRequestLine(it, rawRequest, parsedRequest);

    // Check for whitespace between request-line and first header field
    if (*it == ' ' || *it == '\t' || *it == '\v' || *it == '\f' || *it == '\r' || *it == '\n')
    {
        throw HttpStatusCodeException(BAD_REQUEST, // throw '400' status error
                                      "whitespace between the start-line and the first header field");
    }

    // Parse the headers
    this->_parseHeaders(it, rawRequest, parsedRequest);

    // Parse the body
    this->_parseBody(it, rawRequest, parsedRequest);

    // Return parsed request
    return parsedRequest;
}

// Function to parse the request line of an HTTP request
void RequestParser::_parseRequestLine(std::vector<char>::const_iterator &requestIterator,
                                      const std::vector<char> &rawRequest,
                                      Request &parsedRequest) const
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
    while (*requestIterator == '\r' && *(requestIterator + 1) == '\n')
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

    return uri;
}

// Function to parse the HTTP version from the request line
std::string RequestParser::_parseHttpVersion(std::vector<char>::const_iterator &requestIterator,
                                             const std::vector<char> &rawRequest) const
{
    std::string httpVersion;

    // Build 'httpVersion' string until CRLF is found
    while (requestIterator != rawRequest.end() && !(*requestIterator == '\r' && *(requestIterator + 1) == '\n'))
    {
        // Check for invalid characters
        if (*requestIterator == '\r' || *requestIterator == '\n')
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

    return httpVersion;
}

// Function to parse the headers of an HTTP request
void RequestParser::_parseHeaders(std::vector<char>::const_iterator &requestIterator,
                                  const std::vector<char> &rawRequest,
                                  Request &parsedRequest) const
{
    // Parse headers
    while (requestIterator != rawRequest.end() && !(*requestIterator == '\r' && *(requestIterator + 1) == '\n'))
    {
        // Check for invalid characters
        if (*requestIterator == '\r' || *requestIterator == '\n')
        {
            throw HttpStatusCodeException(BAD_REQUEST, "Invalid characters in header");
        }

        // Parse header name and value
        std::string headerName;
        std::string headerValue;
        int clientHeaderBufferSize = this->_configuration.getClientHeaderBufferSize();

        // Find colon to separate header name and value
        while (requestIterator != rawRequest.end() && *requestIterator != ':')
        {
            headerName += *requestIterator;
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
        while (requestIterator != rawRequest.end() && !(*requestIterator == '\r' && *(requestIterator + 1) == '\n'))
        {
            headerValue += *requestIterator;
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

        // Add header to parsed request
        parsedRequest.addHeader(headerName, headerValue);
    }

    // Check for unexpected end of request
    if (requestIterator == rawRequest.end())
    {
        throw HttpStatusCodeException(BAD_REQUEST, "Unexpected end of request");
    }

    // Move marker passed CRLF
    requestIterator += 2;
}

// Function to parse the body of an HTTP request
void RequestParser::_parseBody(std::vector<char>::const_iterator &requestIterator,
                               const std::vector<char> &rawRequest,
                               Request &parsedRequest) const
{
    // Extract body (if present)
    if (parsedRequest.getMethod() != POST && parsedRequest.getMethod() != PUT)
        return; // No need to parse body for other methods

    // Find body size
    std::string contentLengthString = parsedRequest.getHeader("content-length");

    // Check if 'Content-Length' header is missing
    if (contentLengthString.empty())
        throw HttpStatusCodeException(LENGTH_REQUIRED, // throw '411' status error
                                      "no content-length header found");

    // Convert 'Content-Length' header value to integer
    int bodySize = atoi(contentLengthString.c_str());

    // Check if conversion was successful
    if (bodySize <= 0)
        throw HttpStatusCodeException(BAD_REQUEST, // throw '400' status error
                                      "content-length header conversion failed (" + contentLengthString + ")");

    // Check if body size exceeds client body buffer size
    if (bodySize > this->_configuration.getClientBodyBufferSize())
        throw HttpStatusCodeException(PAYLOAD_TOO_LARGE); // throw '413' status error

    // Check if body size exceeds remaining request size
    int remainingRequestSize = rawRequest.end() - requestIterator;
    if (remainingRequestSize < bodySize)
        throw HttpStatusCodeException(BAD_REQUEST,
                                      "body size exceeds remaining request size"); // throw '400' status error

    // Extract body
    std::vector<char> body(requestIterator, requestIterator + bodySize);
    parsedRequest.setBody(body);
}
