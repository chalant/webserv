#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

/*
 * RequestParser.hpp
 *
 * The RequestParser class provides functionality for parsing raw HTTP requests and converting
 * them into IRequest objects.
 *
 */

#include <vector>
#include <string>
#include "../configuration/IConfiguration.hpp"
#include "../request/IRequest.hpp"
#include "../response/IResponse.hpp"
#include "../logger/ILogger.hpp"
#include "../exception/IExceptionHandler.hpp"
#include "../exception/WebservExceptions.hpp"
#include "../constants/HttpHelper.hpp"

class RequestParser
{
private:
    ILogger &_logger;                // Reference to the error logger
    const IConfiguration &_configuration;       // Reference to the server IConfiguration

    // Function to parse the request line of an HTTP request
    void _parseRequestLine(std::vector<char>::const_iterator &it,
                           const std::vector<char> &rawRequest,
                           IRequest &parsedRequest) const;

    // Functions to parse the components of the request line
    std::string _parseMethod(std::vector<char>::const_iterator &it,
                             const std::vector<char> &rawRequest) const;
    std::string _parseUri(std::vector<char>::const_iterator &it,
                          const std::vector<char> &rawRequest) const;
    std::string _parseHttpVersion(std::vector<char>::const_iterator &it,
                                  const std::vector<char> &rawRequest) const;

    // Function to parse the headers of an HTTP request
    void _parseHeaders(std::vector<char>::const_iterator &it,
                       const std::vector<char> &rawRequest,
                       IRequest &parsedRequest) const;

    // Function to parse an individual header
    void _parseHeader(std::vector<char>::const_iterator &it,
                      const std::vector<char> &rawRequest,
                      IRequest &parsedRequest) const;

    // Function to parse the body of an HTTP request
    void _parseBody(std::vector<char>::const_iterator &it,
                    const std::vector<char> &rawRequest,
                    IRequest &parsedRequest) const;

    // Function to parse a Cookie header
    void _parseCookie(std::string &cookieHeaderValue,
                      IRequest &parsedRequest) const;

    // Function to parse Query Parameters
    void _parseQueryParameters(std::vector<char>::const_iterator &it,
                               const std::vector<char> &rawRequest,
                               IRequest &parsedRequest) const;

    // Function to parse url encoded body parameters
    void _parseBodyParameters(std::vector<char>::const_iterator &it,
                             const std::vector<char> &rawRequest,
                             IRequest &parsedRequest) const;

    // Function to parse the url
    void _parseUrl(std::vector<char>::const_iterator &it,
                   const std::vector<char> &rawRequest,
                   IRequest &parsedRequest) const;

    // Function to check if a character is whitespace
    bool _isWhitespace(char c) const;

    // Function to check if an iterator points to CRLF (carriage return + line feed)
    bool _isCRLF(std::vector<char>::const_iterator it) const;

    // Function to check if an iterator points to a character that is in a given set
    bool _isCharInSet(std::vector<char>::const_iterator it, const std::string &set) const;

    // Function to trim leading and trailing whitespace from a string
    std::string _trimWhitespace(const std::string &string) const;

public:
    // Constructor to initialize the RequestParser with required references
    RequestParser(const IConfiguration &configuration, ILogger &logger);

    // Function to parse a raw HTTP request and convert it into a IRequest object
    void parseRequest(const std::vector<char> &rawRequest, IRequest &parsedRequest) const;
};

#endif // REQUESTPARSER_HPP
// Path: srcs/RequestParser.cpp
