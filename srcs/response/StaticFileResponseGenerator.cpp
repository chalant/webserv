#include "../../includes/response/StaticFileResponseGenerator.hpp"
#include <fstream>

// Constructor
StaticFileResponseGenerator::StaticFileResponseGenerator(ILogger &logger)
    : _mimeTypes(_initialiseMimeTypes()), _logger(logger)
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
    if (root[ root.size() - 1 ] != '/' && uri[ 0 ] != '/')
    {
        // add a slash to the root
        root += "/";
    }
    std::string filePath = root + uri;

    // check if the filePath is a directory
    if (filePath[ filePath.size() - 1 ] == '/')
    {
        // append the default file name
        filePath += route.getIndex();
    }

    // open the file in binary mode, in read mode and at the end
    std::ifstream file(filePath,
                       std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        // log the error
        _logger.log(ERROR, "Could not open file: " + filePath);

        // set the response
        response.setErrorResponse(NOT_FOUND);
    }
    else
    {
        // log the file being served
        _logger.log(INFO, "Serving file: " + filePath);

        // get the size of the file
        std::streampos size = file.tellg();

        // set the position of the file to the beginning
        file.seekg(0, std::ios::beg);

        // read the file into the body
        std::vector<char> body(size);
        file.read(&body[ 0 ], size);

        // check if the file was read successfully
        if (!file)
        {
            // log the error
            _logger.log(ERROR, "Error reading file: " + filePath);

            // set the response
            response.setErrorResponse(INTERNAL_SERVER_ERROR);
        }
        else
        {
            // close the file
            file.close();

            // set the response
            response.setBody(body);
            response.setStatusLine(OK);
            response.addHeader(CONTENT_TYPE, this->_getMimeType(filePath));
            response.addHeader(CONTENT_LENGTH, std::to_string(body.size()));
        }
    }

    // return -1
    return Triplet_t(-1,
                     std::make_pair(0, 0)); // -1 currently just means no CGI
}

// Resolve extension
std::string
StaticFileResponseGenerator::_getMimeType(const std::string &filePath) const
{
    std::string extension;

    // Find the last dot
    size_t dotPosition = filePath.find_last_of(".");

    // Get the extension
    if (dotPosition == std::string::npos)
        extension = "unknown";
    else
        extension = filePath.substr(dotPosition + 1);

    // Return the mime type
    return this->_mimeTypes.at(extension);
}

// Set the mime types
std::map<std::string, std::string>
StaticFileResponseGenerator::_initialiseMimeTypes() const
{
    std::map<std::string, std::string> mimeTypes;
    mimeTypes[ "txt" ] = "text/plain";
    mimeTypes[ "html" ] = "text/html";
    mimeTypes[ "css" ] = "text/css";
    mimeTypes[ "js" ] = "text/javascript";
    mimeTypes[ "jpg" ] = "image/jpeg";
    mimeTypes[ "jpeg" ] = "image/jpeg";
    mimeTypes[ "png" ] = "image/png";
    mimeTypes[ "gif" ] = "image/gif";
    mimeTypes[ "ico" ] = "image/x-icon";
    mimeTypes[ "json" ] = "application/json";
    mimeTypes[ "pdf" ] = "application/pdf";
    mimeTypes[ "zip" ] = "application/zip";
    mimeTypes[ "tar" ] = "application/x-tar";
    mimeTypes[ "xml" ] = "application/xml";
    mimeTypes[ "mp3" ] = "audio/mpeg";
    mimeTypes[ "wav" ] = "audio/wav";
    mimeTypes[ "mp4" ] = "video/mp4";
    mimeTypes[ "mpeg" ] = "video/mpeg";
    mimeTypes[ "webm" ] = "video/webm";
    mimeTypes[ "woff" ] = "font/woff";
    mimeTypes[ "woff2" ] = "font/woff2";
    mimeTypes[ "ttf" ] = "font/ttf";
    mimeTypes[ "otf" ] = "font/otf";
    mimeTypes[ "eot" ] = "font/eot";
    mimeTypes[ "svg" ] = "image/svg+xml";
    mimeTypes[ "unknown" ] = "application/octet-stream";
    return mimeTypes;
}

// Path: srcs/response/Response.cpp
