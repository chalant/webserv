#include "../../includes/response/StaticFileResponseGenerator.hpp"
#include "../../includes/utils/Converter.hpp"
#include <fstream>

// Constructor
StaticFileResponseGenerator::StaticFileResponseGenerator(ILogger &logger)
    : m_mime_types(m_initialiseMimeTypes()), m_logger(logger)
{
}

// Destructor
StaticFileResponseGenerator::~StaticFileResponseGenerator() {}

// Generate response
Triplet_t StaticFileResponseGenerator::generateResponse(
    const IRoute &route, const IRequest &request, IResponse &response,
    IConfiguration &configuration, const std::string &script_name)
{
    // void the unused parameters
    (void)configuration;
    (void)script_name;

    // Get the file path
    std::string root = route.getRoot();
    std::string uri = request.getUri();

    // if root does not end with a slash and uri does not start with a slash
    if (root[ root.size() - 1 ] != '/' && uri[ 0 ] != '/')
    {
        // add a slash to the root
        root += "/";
    }
    std::string file_path = root + uri;

    // check if the file_path is a directory
    if (file_path[ file_path.size() - 1 ] == '/')
    {
        // append the default file name
        file_path += route.getIndex();
    }

    // open the file in binary mode, in read mode and at the end
    std::ifstream file(file_path.c_str(),
                       std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        // log the error
        m_logger.log(ERROR, "Could not open file: " + file_path);

        // set the response
        response.setErrorResponse(NOT_FOUND);
    }
    else
    {
        // log the file being served
        m_logger.log(VERBOSE, "Serving file: " + file_path);

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
            m_logger.log(ERROR, "Error reading file: " + file_path);

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
            response.addHeader(CONTENT_TYPE, m_getMimeType(file_path));
            response.addHeader(CONTENT_LENGTH,
                               Converter::toString(body.size()));
        }
    }

    // return -1
    return Triplet_t(-1,
                     std::make_pair(0, 0)); // -1 currently just means no CGI
}

// Resolve extension
std::string
StaticFileResponseGenerator::m_getMimeType(const std::string &file_path) const
{
    std::string extension;

    // Find the last dot
    size_t dot_position = file_path.find_last_of(".");

    // Get the extension
    if (dot_position == std::string::npos)
        extension = "unknown";
    else
        extension = file_path.substr(dot_position + 1);

    // Return the mime type
    return m_mime_types.at(extension);
}

// Set the mime types
std::map<std::string, std::string>
StaticFileResponseGenerator::m_initialiseMimeTypes() const
{
    std::map<std::string, std::string> mime_types;
    mime_types[ "txt" ] = "text/plain";
    mime_types[ "html" ] = "text/html";
    mime_types[ "css" ] = "text/css";
    mime_types[ "js" ] = "text/javascript";
    mime_types[ "jpg" ] = "image/jpeg";
    mime_types[ "jpeg" ] = "image/jpeg";
    mime_types[ "png" ] = "image/png";
    mime_types[ "gif" ] = "image/gif";
    mime_types[ "ico" ] = "image/x-icon";
    mime_types[ "json" ] = "application/json";
    mime_types[ "pdf" ] = "application/pdf";
    mime_types[ "zip" ] = "application/zip";
    mime_types[ "tar" ] = "application/x-tar";
    mime_types[ "xml" ] = "application/xml";
    mime_types[ "mp3" ] = "audio/mpeg";
    mime_types[ "wav" ] = "audio/wav";
    mime_types[ "mp4" ] = "video/mp4";
    mime_types[ "mpeg" ] = "video/mpeg";
    mime_types[ "webm" ] = "video/webm";
    mime_types[ "woff" ] = "font/woff";
    mime_types[ "woff2" ] = "font/woff2";
    mime_types[ "ttf" ] = "font/ttf";
    mime_types[ "otf" ] = "font/otf";
    mime_types[ "eot" ] = "font/eot";
    mime_types[ "svg" ] = "image/svg+xml";
    mime_types[ "unknown" ] = "application/octet-stream";
    return mime_types;
}

// Path: srcs/response/Response.cpp
