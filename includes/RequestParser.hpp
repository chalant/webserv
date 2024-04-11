#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

/*
 * RequestParser.hpp
 *
 * The RequestParser class provides functionality for parsing raw HTTP requests and converting
 * them into Request objects.
 *
 */

#include <vector>
#include <string>
#include "Configuration.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Logger.hpp"
#include "ExceptionHandler.hpp"
#include "WebservExceptions.hpp"
#include "../constants/RequestHelper.hpp"

class RequestParser
{
private:
    const Logger &_errorLogger;                // Reference to the error logger
    const ExceptionHandler &_exceptionHandler; // Reference to the exception handler
    const Configuration &_configuration;       // Reference to the server configuration
    const RequestHelper _requestHelper;        // Helper class for requests
    
    // Function to parse the request line of an HTTP request
    void _parseRequestLine(std::vector<char>::const_iterator& it,
                          const std::vector<char>& rawRequest,
                          Request& parsedRequest) const;
    
    // Functions to parse the components of the request line
    std::string RequestParser::_parseMethod(std::vector<char>::const_iterator &it,
                                       const std::vector<char> &rawRequest) const;
    std::string RequestParser::_parseUri(std::vector<char>::const_iterator &it,
                                    const std::vector<char> &rawRequest) const;
    std::string RequestParser::_parseHttpVersion(std::vector<char>::const_iterator &it,
                                            const std::vector<char> &rawRequest) const;

    // Function to parse the headers of an HTTP request
    void _parseHeaders(std::vector<char>::const_iterator& it,
                      const std::vector<char>& rawRequest,
                      Request& parsedRequest) const;

    // Function to parse the body of an HTTP request
    void _parseBody(std::vector<char>::const_iterator& it,
                   const std::vector<char>& rawRequest,
                   Request& parsedRequest) const;

public:
    // Constructor to initialize the RequestParser with required references
    RequestParser(const Configuration &configuration, Logger &errorLogger, const ExceptionHandler &exceptionHandler);

    // Function to parse a raw HTTP request and convert it into a Request object
    Request parseRequest(const std::vector<char> &rawRequest) const;
};

#endif // REQUESTPARSER_HPP
// Path: srcs/RequestParser.cpp
