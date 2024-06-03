#include "../../includes/request/RequestParser.hpp"
#include "../../includes/constants/HttpStatusCodeHelper.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdlib>

/*
 * RequestParser.cpp
 *
 * The RequestParser class provides functionality for parsing raw HTTP requests
 * and converting them into IRequest objects.
 *
 */

#include "../../includes/constants/HttpHeaderHelper.hpp"
#include "../../includes/utils/Converter.hpp"

// Constructor to initialize the RequestParser with required references
RequestParser::RequestParser(const IConfiguration &configuration,
                             ILogger &logger)
    : m_logger(logger), m_configuration(configuration)
{
}

void RequestParser::parseRequestHeader(IRequest &request) const
{
    const std::vector<char> raw_request = request.getBuffer();
    std::vector<char>::const_iterator it = raw_request.begin();

    // Parse the request line
    m_parseRequestLine(it, raw_request, request);

    // Check for whitespace between request-line and first header field
    if (m_isWhitespace(*it))
    {
        // throw '400' status error
        throw HttpStatusCodeException(
            BAD_REQUEST,
            "whitespace between the start-line and the first header field");
    }

    m_parseHeaders(it, raw_request, request);
    // std::string content_length_string =
    //     request.getHeaderValue(CONTENT_LENGTH);
    // if (content_length_string.empty() &&
    // 	request.getHeaderValue(TRANSFER_ENCODING) != "chunked")
    // {
    //     m_logger.log(DEBUG, "\t\t[REQUESTPARSER] Content-Length is empty");
    //     // throw '411' status error
    //     throw HttpStatusCodeException(LENGTH_REQUIRED,
    //                                   "no content-length header found");
    // }
    // request.getState().setContentLength(atoi(content_length_string.c_str()));
    m_parseBody(it, raw_request, request);
}

void RequestParser::parsePartialBody(const std::vector<char> raw_request,
                                     IRequest &request) const
{
    std::vector<char>::const_iterator it = raw_request.begin();
    m_parseBody(it, raw_request, request);
}

// Function to parse a raw HTTP request and convert it into a Request object
void RequestParser::parseRequest(const std::vector<char> &raw_request,
                                 IRequest &parsed_request) const
{
    m_logger.log(DEBUG, "[REQUESTPARSER] Parsing request...");
    // Iterator to traverse the raw request
    std::vector<char>::const_iterator it = raw_request.begin();

    // Parse the request line
    m_parseRequestLine(it, raw_request, parsed_request);

    // Check for whitespace between request-line and first header field
    if (m_isWhitespace(*it))
    {
        // throw '400' status error
        throw HttpStatusCodeException(
            BAD_REQUEST,
            "whitespace between the start-line and the first header field");
    }

    // Parse the headers
    m_parseHeaders(it, raw_request, parsed_request);

    // Parse the body
    m_parseBody(it, raw_request, parsed_request);

    // Parse the Upload BodyParameters
    // if (parsed_request.getHeaderValue(CONTENT_TYPE)
    //         .find("multipart/form-data") != std::string::npos)
    // {
    //     parseBodyParameters(parsed_request);
    // }
    parseBodyParameters(parsed_request);
    m_logger.log(DEBUG, "[REQUESTPARSER] ...request parsed successfully");
}

// Function to parse the request line of an HTTP request
void RequestParser::m_parseRequestLine(
    std::vector<char>::const_iterator &request_iterator,
    const std::vector<char> &raw_request, IRequest &parsed_request) const
{
    // Parse method, URI, and HTTP version
    std::string method = m_parseMethod(request_iterator, raw_request);
    std::string uri = m_parseUri(request_iterator, raw_request);
    std::string http_version =
        m_parseHttpVersion(request_iterator, raw_request);

    // Set method, URI, and HTTP version in the parsed request
    parsed_request.setMethod(method);
    parsed_request.setUri(uri);
    parsed_request.setHttpVersion(http_version);
}

// Function to parse the HTTP method from the request line
std::string RequestParser::m_parseMethod(
    std::vector<char>::const_iterator &request_iterator,
    const std::vector<char> &raw_request) const
{
    std::string method;
    // Skip leading CRLF
    while (raw_request.end() - request_iterator >= 2 &&
           m_isCRLF(request_iterator))
    {
        request_iterator += 2;
    }

    // Build 'method' string until space is found
    while (request_iterator != raw_request.end() && *request_iterator != ' ')
    {
        method += *request_iterator;
        ++request_iterator;
    }

    // Check for unexpected end of request
    /*     if (request_iterator == raw_request.end())
        {
            throw HttpStatusCodeException(
                BAD_REQUEST,
                "Unexpected end of request while parsing method"); // throw
       '400' status error
        } */

    // Move marker to next character
    ++request_iterator;

    // Log 'method'
    m_logger.log(VERBOSE, "[REQUESTPARSER] Method: \"" + method + "\"");

    // Return 'method' string
    return method;
}

// Function to parse the URI from the request line
std::string
RequestParser::m_parseUri(std::vector<char>::const_iterator &request_iterator,
                          const std::vector<char> &raw_request) const
{
    std::string uri;

    // Build 'uri' string until space is found
    while (request_iterator != raw_request.end() && *request_iterator != ' ')
    {
        uri += *request_iterator;
        ++request_iterator;
    }

    // Check for unexpected end of request
    // if (request_iterator == raw_request.end())
    // {
    //     throw HttpStatusCodeException(
    //         BAD_REQUEST,
    //         "Unexpected end of request"); // throw '400' status error
    // }

    // Move marker to next character
    ++request_iterator;

    // Log 'uri'
    m_logger.log(VERBOSE, "[REQUESTPARSER] URI: \"" + uri + "\"");

    // Return 'uri' string
    return uri;
}

// Function to parse the HTTP version from the request line
std::string RequestParser::m_parseHttpVersion(
    std::vector<char>::const_iterator &request_iterator,
    const std::vector<char> &raw_request) const
{
    std::string http_version;

    // Build 'http_version' string
    while (request_iterator != raw_request.end())
    {
        // End of 'http_version' string, break loop
        if (m_isCRLF(request_iterator))
            break;

        // Check for invalid characters
        if (m_isCharInSet(request_iterator, "\r\n"))
        {
            throw HttpStatusCodeException(
                BAD_REQUEST,
                "Invalid characters in HTTP version"); // throw '400' status
                                                       // error
        }

        // Append character to 'http_version' string
        http_version += *request_iterator;
        ++request_iterator;
    }

    // Check for unexpected end of request
    // if (request_iterator == raw_request.end())
    // {
    //     throw HttpStatusCodeException(
    //         BAD_REQUEST,
    //         "Unexpected end of request"); // throw '400' status error
    // }

    // Move marker passed CRLF
    request_iterator += 2;

    // Log 'http_version'
    m_logger.log(VERBOSE,
                 "[REQUESTPARSER] HTTP Version: \"" + http_version + "\"");

    // Return 'http_version' string
    return http_version;
}

// Function to parse the headers of an HTTP request
void RequestParser::m_parseHeaders(
    std::vector<char>::const_iterator &request_iterator,
    const std::vector<char> &raw_request, IRequest &parsed_request) const
{
    // Parse headers
    while (request_iterator != raw_request.end())
    {
        // End of headers, break loop
        if (m_isCRLF(request_iterator))
            break;
        // Check for invalid characters
        if (m_isCharInSet(request_iterator, "\r\n"))
        {
            throw HttpStatusCodeException(BAD_REQUEST,
                                          "Invalid characters in header");
        }

        // Parse individual header
        m_parseHeader(request_iterator, raw_request, parsed_request);
    }

    // Check for unexpected end of request
    // if (request_iterator == raw_request.end())
    // {
    //     throw HttpStatusCodeException(BAD_REQUEST, "Unexpected end of
    //     request");
    // }

    // Set authority in parsed request
    parsed_request.setAuthority();

    // Move marker passed CRLF
    request_iterator += 2;
}

// Function to parse an individual header
void RequestParser::m_parseHeader(
    std::vector<char>::const_iterator &request_iterator,
    const std::vector<char> &raw_request, IRequest &parsed_request) const
{
    // Parse header name and value
    std::string header_name;
    std::string header_value;

    // Set start value for client header buffer size
    int client_header_buffer_size =
        m_configuration.getInt("client_header_buffer_size");

    // Find colon to separate header name and value
    while (request_iterator != raw_request.end() && *request_iterator != ':')
    {
        header_name += std::tolower(*request_iterator); // Convert to lowercase
        client_header_buffer_size--;
        ++request_iterator;
    }

    // Check if colon was found
    if (request_iterator == raw_request.end())
    {
        throw HttpStatusCodeException(BAD_REQUEST, "Colon not found in header");
    }

    // Skip colon
    ++request_iterator;

    // Skip optional space
    if (request_iterator != raw_request.end() && *request_iterator == ' ')
    {
        ++request_iterator;
    }

    // Find end of header value
    while (request_iterator != raw_request.end() && !m_isCRLF(request_iterator))
    {
        header_value += *request_iterator;
        client_header_buffer_size--;
        ++request_iterator;
    }

    // Check for unexpected end of request
    // if (request_iterator == raw_request.end())
    // {
    //     throw HttpStatusCodeException(BAD_REQUEST, "Unexpected end of
    //     request");
    // }
    // Check if header size exceeds client header buffer size
    if (client_header_buffer_size < 0)
    {
        throw HttpStatusCodeException(REQUEST_HEADER_FIELDS_TOO_LARGE,
                                      "Header fields too large");
    }

    // Move marker passed CRLF
    request_iterator += 2;

    // Log header
    m_logger.log(VERBOSE, "[REQUESTPARSER] Header: \"" + header_name + ": " +
                              header_value + "\"");

    // Add header to parsed request
    try
    {
        parsed_request.addHeader(header_name, header_value);
    }
    catch (const UnknownHeaderError &e)
    {
        // Log and continue
        m_logger.log(WARN, e.what());
    }

    // Parse cookies
    if (header_name == "cookie")
    {
        m_parseCookie(header_value, parsed_request);
    }
}

#include <iostream>
// Function to parse the body of an HTTP request
void RequestParser::m_parseBody(
    std::vector<char>::const_iterator &request_iterator,
    const std::vector<char> &raw_request, IRequest &parsed_request) const
{
    RequestState &state = parsed_request.getState();
    // If method is not POST or PUT, no need to parse body
    if (parsed_request.getMethodString() != "POST" &&
        parsed_request.getMethod() != PUT)
    {
        state.finished(true);
        return; // No need to parse body for other methods
    }
    // Check if 'Transfer-Encoding' is chunked
    std::string transfer_encoding =
        parsed_request.getHeaderValue(TRANSFER_ENCODING);
    if (transfer_encoding == "chunked")
    {
        // Handle chunked encoding
        m_unchunkBody(request_iterator, raw_request, parsed_request);
        return;
    }

    // Check if 'content-length' header is required and missing
    std::string content_length_string =
        parsed_request.getHeaderValue(CONTENT_LENGTH);
    if (content_length_string.empty() &&
        parsed_request.getHeaderValue(TRANSFER_ENCODING) != "chunked")
    {
        m_logger.log(DEBUG, "\t\t[REQUESTPARSER] Content-Length is empty");
        // throw '411' status error
        throw HttpStatusCodeException(LENGTH_REQUIRED,
                                      "no content-length header found");
    }

    // Check if conversion was successful
    // Get 'content-length' value
    size_t body_size = atoi(content_length_string.c_str());
    if (body_size <= 0)
    {
        // throw '400' status error
        throw HttpStatusCodeException(
            BAD_REQUEST, "content-length header conversion failed (" +
                             content_length_string + ")");
    }

    state.setContentRed(state.getContentRed() +
                        (raw_request.end() - request_iterator));
    // m_logger.log(DEBUG, "Cuurent content red: " +
    // Converter::toString(state.getContentRed()));
    // Check if body size exceeds client body buffer size
    if (static_cast<size_t>(state.getContentRed()) >
        m_configuration.getSize_t("client_body_buffer_size"))
    {
        // throw '413' status error
        throw HttpStatusCodeException(PAYLOAD_TOO_LARGE);
    }

    // Check if body size exceeds remaining request size
    // size_t remaining_request_size = raw_request.end() - request_iterator;
    // if (remaining_request_size < body_size)
    // {
    //     // throw '400' status error
    //     throw HttpStatusCodeException(
    //         BAD_REQUEST, "body size exceeds remaining request size");
    // }

    // Extract body
    // std::vector<char> body(request_iterator, request_iterator + body_size);
    std::vector<char> &body = parsed_request.getBody();
    body.insert(body.end(), request_iterator, raw_request.end());
    // set the request state to finished once all the content has been red.
    if (static_cast<size_t>(state.getContentRed()) == body_size)
    {
        state.finished(true);
    }
    // Set body in parsed request
    // parsed_request.setBody(body);
}

// Function to unchunk the body of an HTTP request
void RequestParser::m_unchunkBody(
    std::vector<char>::const_iterator &request_iterator,
    const std::vector<char> &raw_request, IRequest &request) const
{
    RequestState &state = request.getState();
    // Initialize body vector
    std::vector<char> &body = request.getBody();

    // Set Max Size
    size_t remaining_request_size =
        m_configuration.getSize_t("client_body_buffer_size");

    // Parse chunks
    while (request_iterator != raw_request.end())
    {
        // Get chunk size
        std::string chunk_size_string;
        while (request_iterator != raw_request.end() && *request_iterator != '\r')
        {
            chunk_size_string += *request_iterator;
            request_iterator++;
        }

		// // Check if we reached the end without finding CR
        // if (request_iterator == raw_request.end() || (request_iterator + 1) == raw_request.end() || *(request_iterator + 1) != '\n')
        // {
        //     throw HttpStatusCodeException(BAD_REQUEST, "Unexpected end of request");
        // }

		//sleep(100);
        // Check for last chunk and set the request state to finished.
        if (chunk_size_string == "0")
        {
            state.finished(true);
            break;
        }

        // Move marker passed CRLF
        request_iterator += 2;

        //Check for unexpected end of request
        if (request_iterator == raw_request.end())
        {
            // throw '400' status error
            throw HttpStatusCodeException(BAD_REQUEST,
                                          "Unexpected end of request");
        }

        // Get chunk size
        size_t chunk_size = strtol(chunk_size_string.c_str(), NULL, 16);

		std::cout << "Body Size: " << body.size() << " Remaining Request Size: " << remaining_request_size << " Chunk Size: " << chunk_size << " Chunk Size String: " << chunk_size_string << std::endl;
		//sleep(100);
        //Check if chunk size is valid
        if (chunk_size <= 0)
        {
            // throw '400' status error
            throw HttpStatusCodeException(BAD_REQUEST,
                                          "chunk size conversion failed (" +
                                              chunk_size_string + ")");
        }

        // Check if body size exceeds maximum client body buffer size
        if (remaining_request_size < chunk_size)
        {
            // throw '413' status error
            throw HttpStatusCodeException(PAYLOAD_TOO_LARGE);
        }

        // Decrement remaining request size
        remaining_request_size -= chunk_size;

		// Check if the chunk data is within bounds
        if (raw_request.end() - request_iterator < static_cast<ptrdiff_t>(chunk_size + 2))
        {
            throw HttpStatusCodeException(BAD_REQUEST, "Chunk data exceeds request size");
        }

        // Append chunk to body
        body.insert(body.end(), request_iterator,
                    request_iterator + chunk_size);

		// Move marker passed CRLF
		request_iterator += chunk_size + 2;

		// Validate the CRLF after the chunk data
        if (request_iterator == raw_request.end() || (request_iterator + 1) == raw_request.end() || *request_iterator != '\r' || *(request_iterator + 1) != '\n')
        {
            throw HttpStatusCodeException(BAD_REQUEST, "Invalid chunk data ending");
        }
    }
}

// Function to parse cookies from the request
void RequestParser::m_parseCookie(std::string &cookie_header_value,
                                  IRequest &parsed_request) const
{
    // Parse cookies
    std::string cookie_name;
    std::string cookie_value;

    // Create a stream from the cookie header value
    std::istringstream cookie_stream(cookie_header_value);

    // Parse cookie name and value
    while (std::getline(cookie_stream, cookie_name, '=') &&
           std::getline(cookie_stream, cookie_value, ';'))
    {
        // Trim surrounding whitespace
        cookie_name = m_trimWhitespace(cookie_name);
        cookie_value = m_trimWhitespace(cookie_value);

        // Add cookie to parsed request
        parsed_request.addCookie(cookie_name, cookie_value);
    }
}

void RequestParser::parseBodyParameters(IRequest &parsed_request) const
{
    if (parsed_request.getHeaderValue(CONTENT_TYPE)
            .find("multipart/form-data") == std::string::npos)
    {
        return;
    }
    // Log the start of the body parameter parsing
    m_logger.log(VERBOSE, "[REQUESTPARSER] Parsing multipart request...");

    // Get the boundary string
    std::string content_type = parsed_request.getHeaderValue(CONTENT_TYPE);
    std::string boundary =
        "--" + content_type.substr(content_type.find("boundary=") + 9);

    // Get body stream
    std::vector<char> &body = parsed_request.getBody();
    std::string body_string(body.begin(), body.end());
    std::istringstream body_stream(body_string);

    // Declare a line string
    std::string line;

    // Parse BodyParameters
    while (m_getlineNoCr(body_stream, line))
    {
        // Skip leading newline
        if (line.empty())
        {
            // Move to next line
            continue;
        }

        // Skip boundary
        if (line.find(boundary) != std::string::npos)
        {
            // Move to next line
            continue;
        }

        BodyParameter body_parameter;

        // Process each header line
        std::string key;
        std::string value;
        do
        {
            std::string::size_type pos = line.find(':');
            if (pos != std::string::npos)
            {
                // Extract and trim key and value
                key = line.substr(0, pos);
                value = line.substr(pos + 1);
                key = m_trimWhitespace(key);
                value = m_trimWhitespace(value);

                // lower cases the key
                std::transform(key.begin(), key.end(), key.begin(),
                               static_cast<int (*)(int)>(std::tolower));

                // Store the header in the map
                body_parameter.headers[ key ] = value;

                // Parse disposition_type, content_type, and field_name
                if (key == "content-disposition")
                {
                    // extract the disposition_type
                    size_t pos = value.find(';');

                    // declare remaining value
                    std::string remaining_value;

                    if (pos != std::string::npos)
                    {
                        body_parameter.disposition_type = value.substr(0, pos);
                        remaining_value = value.substr(pos + 1);
                    }

                    // extract the filename and field_name
                    std::istringstream iss(remaining_value);
                    std::string token;
                    while (std::getline(iss, token, ';'))
                    {
                        size_t pos = token.find('=');
                        if (pos != std::string::npos)
                        {
                            // extract and trim the parameter and its value
                            std::string param = token.substr(0, pos);
                            std::string param_value = token.substr(pos + 1);
                            param = m_trimWhitespace(param);
                            param_value = m_trimWhitespace(param_value);

                            // Remove surrounding quotes if they exist
                            m_removeQuotes(param_value);

                            // Store the parameter
                            if (param == "filename")
                                body_parameter.filename = param_value;
                            else if (param == "name")
                                body_parameter.field_name = param_value;
                        }
                    }
                }
                else if (key == "content-type")
                    body_parameter.content_type = value;
            }
            // Log the header
            m_logger.log(VERBOSE, "[REQUESTPARSER]  Header: " + key + ": \"" +
                                      value + "\"");
        } while (m_getlineNoCr(body_stream, line) && !line.empty());

        // Assign the data to the body_parameter data member
        m_assignUntilBoundary(
            body_stream, boundary, body_parameter.data);

        // Log the first character of the BodyParameter data
        if (!body_parameter.data.empty())
            m_logger.log(VERBOSE,
                         "[REQUESTPARSER]  Data: \"" +
                             Converter::toString(body_parameter.data[ 0 ]) +
                             "...\"");
        else
            m_logger.log(VERBOSE,
                         "[REQUESTPARSER] No data found in BodyParameter");

        // Add BodyParameter to vector
        parsed_request.addBodyParameter(body_parameter);
    }

    // Log the end of the BodyParameter
    m_logger.log(VERBOSE, "[REQUESTPARSER] ...done parsing multipart request");

    // Mark the request as an upload request
    parsed_request.setUploadRequest(true);
}

// Function to check if a character is whitespace
bool RequestParser::m_isWhitespace(char c) const
{
    return c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r' ||
           c == '\n';
}

// Function to check if an iterator points to CRLF (carriage return + line feed)
bool RequestParser::m_isCRLF(std::vector<char>::const_iterator it) const
{
    return *it == '\r' && *(it + 1) == '\n';
}

// Function to check if an iterator points to a character that is in a given set
bool RequestParser::m_isCharInSet(std::vector<char>::const_iterator it,
                                  const std::string &set) const
{
    return set.find(*it) != std::string::npos;
}

// Function to trim whitespace
std::string RequestParser::m_trimWhitespace(const std::string &string) const
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

// Function to remove quotes from a string
void RequestParser::m_removeQuotes(std::string &string) const
{
    if (string.length() >= 2 && string.at(0) == '"' &&
        string.at(string.length() - 1) == '"')
        string = string.substr(1, string.length() - 2);
}

// Custom getline function to remove carriage return
std::istream &RequestParser::m_getlineNoCr(std::istream &is,
                                           std::string &line) const
{
    if (std::getline(is, line))
    {
        if (!line.empty() && line[ line.size() - 1 ] == '\r')
        {
            line.erase(line.size() - 1);
        }
    }
    return is;
}

// Function to read a stringstream into a char vector until the boundary is found
// NOTE - This function will currently fail when the boundary is split between two buffer reads
void RequestParser::m_assignUntilBoundary(std::istringstream &iss,
                                            const std::string &boundary,
                                            std::vector<char> &dest) const
{
    const size_t buffer_size = 1024;
    char buffer[ buffer_size ];
    size_t boundary_length = boundary.length();
    size_t bytes_read;
    size_t pos;

    // Read the buffer until the boundary is found
    while (!iss.eof())
    {
        // Get the current position
        pos = iss.tellg();

        // Read the buffer
        iss.read(buffer, buffer_size);

        // Get the number of bytes read
        bytes_read = iss.gcount();

        // Check if the boundary is found in the buffer
        for (size_t i = 0; i < bytes_read; i++)
        {
            if (buffer[ i ] == boundary[ 0 ])
            {
                // Check if the boundary is found in the buffer
                if (strncmp(&buffer[ i ], boundary.c_str(), boundary_length) ==
                    0)
                {
                    // Add the buffer to the destination minus the CRLF
                    dest.insert(dest.end(), buffer, buffer + i - 2);

                    // Move the stringstream to the end of the boundary
                    // and passed the CRLF
                    iss.clear(); // Clear the eof flag if necessary
                    iss.seekg(pos + i + boundary_length + 2);

                    // Return from the function
                    return;
                }
            }
        }

        // Add the buffer to the destination and continue reading
        dest.insert(dest.end(), buffer, buffer + bytes_read);
    }
}
// path: srcs/RequestHandler.cpp
