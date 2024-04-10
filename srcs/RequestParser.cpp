#include "../includes/RequestParser.hpp"

/*
 * RequestParser.cpp
 *
 * The RequestParser class provides functionality for parsing raw HTTP requests and converting
 * them into Request objects.
 *
 */

// Constructor to initialize the RequestParser with required references
RequestParser::RequestParser(Configuration &configuration, Logger &errorLogger, ExceptionHandler &exceptionHandler) 
    : _configuration(configuration), _errorLogger(errorLogger), _exceptionHandler(exceptionHandler), _requestHelper() {}

// Function to parse a raw HTTP request and convert it into a Request object
Request RequestParser::parseRequest(const std::vector<char> &rawRequest) const
{
    std::string request(rawRequest.begin(), rawRequest.end());
    Request parsedRequest(this->_requestHelper, this->_configuration); // Request object to store parsed request

    // Parse request line
    size_t space = request.find(' ');
    if (space == std::string::npos)
        throw InvalidRequestLineError();
    parsedRequest.setMethod(request.substr(0, space));

    request.erase(0, space + 1); // Erase parsed portion

    space = request.find(' ');
    if (space == std::string::npos)
        throw InvalidRequestLineError();
    parsedRequest.setUri(request.substr(0, space));

    request.erase(0, space + 1); // Erase parsed portion

    size_t newline = request.find("\r\n");
    if (newline == std::string::npos)
        throw InvalidHeaderError();
    parsedRequest.setHttpVersion(request.substr(0, newline));

    request.erase(0, newline + 2); // Erase parsed portion

    // Parse headers
    std::string name;
    std::string value;
    size_t colon;
    while (request.find("\r\n") != 0)
    {
        colon = request.find(':');
        if (colon == std::string::npos)
            throw InvalidHeaderError();
        name = request.substr(0, colon);
        request.erase(0, colon + 1);
        if (request.find(' ') == 0)
            request.erase(0, 1); // remove optional space
        parsedRequest.addHeader(name, request.substr(0, request.find("\r\n")));
        request.erase(0, request.find("\r\n") + 2);
        if (request.empty())
            throw InvalidHeaderError();
    }

    // Extract body (if present)
    std::vector<char> body(request.begin(), request.end());
    parsedRequest.setBody(body);
}
