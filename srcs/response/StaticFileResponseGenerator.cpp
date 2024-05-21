#include "../../includes/response/StaticFileResponseGenerator.hpp"
#include <fstream>

// Constructor
StaticFileResponseGenerator::StaticFileResponseGenerator(ILogger &logger)
    : _logger(logger)
{
}

// Destructor
StaticFileResponseGenerator::~StaticFileResponseGenerator() {}

// Generate response
Triplet_t StaticFileResponseGenerator::generateResponse(
    const IRoute &route, const IRequest &request, IResponse &response,
    const IConfiguration &configuration, const std::string &scriptName)
{
    // void the unused parameters
    (void)configuration;
    (void)scriptName;

    // Get the file path
    std::string root = route.getRoot();
    std::string uri = request.getUri();
    // if root does not end with a slash and uri does not start with a slash
    if (root[root.size() - 1] != '/' && uri[0] != '/')
    {
        // add a slash to the root
        root += "/";
    }
    std::string filePath = root + uri;

    // Open the file
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        // Log the error
        _logger.log(ERROR, "Could not open file: " + filePath);

        // Set the response
        response.setErrorResponse(NOT_FOUND);
    }
    else
    {
        // Declare the variables
        std::string status;
        std::string headers;
        std::string body;

        // Read the file, build the body
        std::string line;
        while (std::getline(file, line))
        {
            body += line + "\n";
        }

        // Close the file
        file.close();

        // Set the response
        response.setBody(body);
        response.setStatusLine(OK);
        response.addHeader(CONTENT_TYPE, "text/html");
        response.addHeader(CONTENT_LENGTH, std::to_string(body.size()));
    }

    // return -1
    return Triplet_t(-1,
                     std::make_pair(0, 0)); // -1 currently just means no CGI
}

// Path: srcs/response/Response.cpp
