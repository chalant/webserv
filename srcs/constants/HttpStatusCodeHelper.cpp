#include "../../includes/constants/HttpStatusCodeHelper.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include "../../includes/utils/Converter.hpp"

/*
 * HttpStatusCodeHelper.hpp
 *
 * The HttpStatusCodeHelper class provides functionality for working with HTTP
 * status codes. It defines an enum HttpStatusCode to represent standard HTTP
 * status codes and provides methods to retrieve string representations of
 * status codes and vice versa.
 *
 */

// Constructor initializes member variables using helper functions
HttpStatusCodeHelper::HttpStatusCodeHelper()
    : m_status_code_list(m_setStatusCodeList()),
      m_string_http_status_code_map(m_setStringHttpStatusCodeMap()),
      m_http_status_code_string_map(m_setHttpStatusCodeStringMap())
{
}

// Get string representation of HttpStatusCode enum value
const std::string &
HttpStatusCodeHelper::httpStatusCodeStringMap(HttpStatusCode status_code) const
{
    if (m_http_status_code_string_map.find(status_code) !=
        m_http_status_code_string_map.end())
    {
        return m_http_status_code_string_map.at(status_code);
    }
    else
    {
        throw UnknownHttpStatusCodeError();
    }
}

// Get HttpStatusCode enum value from string representation
HttpStatusCode HttpStatusCodeHelper::stringHttpStatusCodeMap(
    const std::string &status_code) const
{
    if (m_string_http_status_code_map.find(status_code) !=
        m_string_http_status_code_map.end())
    {
        return m_string_http_status_code_map.at(status_code);
    }
    else
    {
        throw UnknownHttpStatusCodeError(status_code);
    }
}

// Get HttpStatusCode enum value from string representation
HttpStatusCode
HttpStatusCodeHelper::intHttpStatusCodeMap(const int &status_code) const
{
    if (status_code >= 100 && status_code <= 511)
    {
        return static_cast<HttpStatusCode>(status_code);
    }
    else
    {
        throw UnknownHttpStatusCodeError(Converter::toString(status_code));
    }
}

// Generate a status line string for an HTTP response
std::string HttpStatusCodeHelper::getStatusLine(HttpStatusCode status_code) const
{
    return "HTTP/1.1 " + Converter::toString(static_cast<size_t>(status_code)) +
           " " + this->httpStatusCodeStringMap(status_code) + "\r\n";
}

// Generate a complete error response for an HTTP status code
std::string
HttpStatusCodeHelper::getErrorResponse(HttpStatusCode status_code) const
{
    std::string body = this->getHtmlPage(status_code);

    return this->getStatusLine(status_code) + "\n" +
           "content-type: text/html\r\n" +
           "content-length: " + Converter::toString(body.length()) + "\r\n" +
           "Connection: close\r\n" + "Server: webserv/1.0\r\n" + "\r\n" + body;
}

// Generate an HTML page with the specified HTTP status code
std::string HttpStatusCodeHelper::getHtmlPage(HttpStatusCode status_code) const
{
    // Create a string stream to build the HTML page.
    std::stringstream html_page;

    // Write the HTML page
    html_page
        << "<!DOCTYPE html>\n"
           "<html lang=\"en\">\n"
           "<head>\n"
           "    <meta charset=\"UTF-8\">\n"
           "    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n"
           "    <meta name=\"viewport\" content=\"width=device-width, "
           "initial-scale=1.0\">\n"
           "    <title>Webserv | "
        << static_cast<size_t>(status_code)
        << "</title>\n"
           "    <style>\n"
           "        body {\n"
           "            font-family: Arial, sans-serif;\n"
           "            background-color: #333;\n"
           "            color: #f4f4f4;\n"
           "            margin: 0;\n"
           "            padding: 0;\n"
           "            display: flex;\n"
           "            align-items: center;\n"
           "            justify-content: center;\n"
           "            height: 100vh;\n"
           "        }\n"
           "        .container {\n"
           "            width: 80%;\n"
           "            text-align: center;\n"
           "            position: relative;\n"
           "        }\n"
           "        .error {\n"
           "            background: #444;\n"
           "            border: #333 solid 1px;\n"
           "            padding: 20px;\n"
           "            position: relative;\n"
           "        }\n"
           "        .error h1 {\n"
           "            color: #f90;\n"
           "            font-size: 33px;\n"
           "            margin-bottom: 5px;\n"
           "        }\n"
           "        .error p {\n"
           "            font-size: 21px;\n"
           "        }\n"
           "        .version {\n"
           "            position: absolute;\n"
           "            bottom: 5px;\n"
           "            right: 5px;\n"
           "            font-size: 14px;\n"
           "            color: #999;\n"
           "        }\n"
           "    </style>\n"
           "</head>\n"
           "<body>\n"
           "    <div class=\"container\">\n"
           "        <div class=\"error\">\n"
           "            <h1>Error <span style=\"color: #f90;\">"
        << static_cast<size_t>(status_code)
        << "</span></h1>\n"
           "            <p> "
        << m_http_status_code_string_map.at(status_code)
        << "</p>\n"
           "            <div class=\"version\">webserv/1.0</div>\n"
           "        </div>\n"
           "    </div>\n"
           "</body>\n"
           "</html>\n";

    // Return the generated HTML page.
    return html_page.str();
}

// Helper function to initialize m_status_code_list with string representations of
// HTTP status codes
std::vector<std::string> HttpStatusCodeHelper::m_setStatusCodeList()
{
    std::vector<std::string> status_code_list;

    // Add string representations of HTTP status codes to status_code_list
    status_code_list.push_back("100 Continue");
    status_code_list.push_back("101 Switching Protocols");
    status_code_list.push_back("102 Processing");
    status_code_list.push_back("103 Early Hints");
    status_code_list.push_back("200 OK");
    status_code_list.push_back("201 Created");
    status_code_list.push_back("202 Accepted");
    status_code_list.push_back("203 Non-Authoritative Information");
    status_code_list.push_back("204 No Content");
    status_code_list.push_back("205 Reset Content");
    status_code_list.push_back("206 Partial Content");
    status_code_list.push_back("207 Multi-Status");
    status_code_list.push_back("208 Already Reported");
    status_code_list.push_back("226 IM Used");
    status_code_list.push_back("300 Multiple Choices");
    status_code_list.push_back("301 Moved Permanently");
    status_code_list.push_back("302 Found");
    status_code_list.push_back("303 See Other");
    status_code_list.push_back("304 Not Modified");
    status_code_list.push_back("305 Use Proxy");
    status_code_list.push_back("306 Switch Proxy");
    status_code_list.push_back("307 Temporary Redirect");
    status_code_list.push_back("308 Permanent Redirect");
    status_code_list.push_back("400 Bad Request");
    status_code_list.push_back("401 Unauthorized");
    status_code_list.push_back("402 Payment Required");
    status_code_list.push_back("403 Forbidden");
    status_code_list.push_back("404 Not Found");
    status_code_list.push_back("405 Method Not Allowed");
    status_code_list.push_back("406 Not Acceptable");
    status_code_list.push_back("407 Proxy Authentication Required");
    status_code_list.push_back("408 Request Timeout");
    status_code_list.push_back("409 Conflict");
    status_code_list.push_back("410 Gone");
    status_code_list.push_back("411 Length Required");
    status_code_list.push_back("412 Precondition Failed");
    status_code_list.push_back("413 Payload Too Large");
    status_code_list.push_back("414 URI Too Long");
    status_code_list.push_back("415 Unsupported Media Type");
    status_code_list.push_back("416 Range Not Satisfiable");
    status_code_list.push_back("417 Expectation Failed");
    status_code_list.push_back("418 I'm a teapot");
    status_code_list.push_back("421 Misdirected Request");
    status_code_list.push_back("422 Unprocessable Entity");
    status_code_list.push_back("423 Locked");
    status_code_list.push_back("424 Failed Dependency");
    status_code_list.push_back("425 Too Early");
    status_code_list.push_back("426 Upgrade Required");
    status_code_list.push_back("428 Precondition Required");
    status_code_list.push_back("429 Too Many Requests");
    status_code_list.push_back("431 Request Header Fields Too Large");
    status_code_list.push_back("451 Unavailable For Legal Reasons");
    status_code_list.push_back("500 Internal Server Error");
    status_code_list.push_back("501 Not Implemented");
    status_code_list.push_back("502 Bad Gateway");
    status_code_list.push_back("503 Service Unavailable");
    status_code_list.push_back("504 Gateway Timeout");
    status_code_list.push_back("505 HTTP Version Not Supported");
    status_code_list.push_back("506 Variant Also Negotiates");
    status_code_list.push_back("507 Insufficient Storage");
    status_code_list.push_back("508 Loop Detected");
    status_code_list.push_back("510 Not Extended");
    status_code_list.push_back("511 Network Authentication Required");

    return status_code_list;
}

// Helper function to initialize m_string_http_status_code_map with mappings from
// string representations to HttpStatusCode enum values
std::map<std::string, HttpStatusCode>
HttpStatusCodeHelper::m_setStringHttpStatusCodeMap()
{
    std::map<std::string, HttpStatusCode> string_http_status_code_map;

    // Add mappings from string representations to HttpStatusCode enum values
    string_http_status_code_map[ "100 Continue" ] = CONTINUE;
    string_http_status_code_map[ "101 Switching Protocols" ] = SWITCHING_PROTOCOLS;
    string_http_status_code_map[ "102 Processing" ] = PROCESSING;
    string_http_status_code_map[ "103 Early Hints" ] = EARLY_HINTS;
    string_http_status_code_map[ "200 OK" ] = OK;
    string_http_status_code_map[ "201 Created" ] = CREATED;
    string_http_status_code_map[ "202 Accepted" ] = ACCEPTED;
    string_http_status_code_map[ "203 Non-Authoritative Information" ] =
        NON_AUTHORITATIVE_INFORMATION;
    string_http_status_code_map[ "204 No Content" ] = NO_CONTENT;
    string_http_status_code_map[ "205 Reset Content" ] = RESET_CONTENT;
    string_http_status_code_map[ "206 Partial Content" ] = PARTIAL_CONTENT;
    string_http_status_code_map[ "207 Multi-Status" ] = MULTI_STATUS;
    string_http_status_code_map[ "208 Already Reported" ] = ALREADY_REPORTED;
    string_http_status_code_map[ "226 IM Used" ] = IM_USED;
    string_http_status_code_map[ "300 Multiple Choices" ] = MULTIPLE_CHOICES;
    string_http_status_code_map[ "301 Moved Permanently" ] = MOVED_PERMANENTLY;
    string_http_status_code_map[ "302 Found" ] = FOUND;
    string_http_status_code_map[ "303 See Other" ] = SEE_OTHER;
    string_http_status_code_map[ "304 Not Modified" ] = NOT_MODIFIED;
    string_http_status_code_map[ "305 Use Proxy" ] = USE_PROXY;
    string_http_status_code_map[ "306 Switch Proxy" ] = SWITCH_PROXY;
    string_http_status_code_map[ "307 Temporary Redirect" ] = TEMPORARY_REDIRECT;
    string_http_status_code_map[ "308 Permanent Redirect" ] = PERMANENT_REDIRECT;
    string_http_status_code_map[ "400 Bad Request" ] = BAD_REQUEST;
    string_http_status_code_map[ "401 Unauthorized" ] = UNAUTHORIZED;
    string_http_status_code_map[ "402 Payment Required" ] = PAYMENT_REQUIRED;
    string_http_status_code_map[ "403 Forbidden" ] = FORBIDDEN;
    string_http_status_code_map[ "404 Not Found" ] = NOT_FOUND;
    string_http_status_code_map[ "405 Method Not Allowed" ] = METHOD_NOT_ALLOWED;
    string_http_status_code_map[ "406 Not Acceptable" ] = NOT_ACCEPTABLE;
    string_http_status_code_map[ "407 Proxy Authentication Required" ] =
        PROXY_AUTHENTICATION_REQUIRED;
    string_http_status_code_map[ "408 Request Timeout" ] = REQUEST_TIMEOUT;
    string_http_status_code_map[ "409 Conflict" ] = CONFLICT;
    string_http_status_code_map[ "410 Gone" ] = GONE;
    string_http_status_code_map[ "411 Length Required" ] = LENGTH_REQUIRED;
    string_http_status_code_map[ "412 Precondition Failed" ] = PRECONDITION_FAILED;
    string_http_status_code_map[ "413 Payload Too Large" ] = PAYLOAD_TOO_LARGE;
    string_http_status_code_map[ "414 URI Too Long" ] = URI_TOO_LONG;
    string_http_status_code_map[ "415 Unsupported Media Type" ] =
        UNSUPPORTED_MEDIA_TYPE;
    string_http_status_code_map[ "416 Range Not Satisfiable" ] =
        RANGE_NOT_SATISFIABLE;
    string_http_status_code_map[ "417 Expectation Failed" ] = EXPECTATION_FAILED;
    string_http_status_code_map[ "418 I'm a teapot" ] = IM_A_TEAPOT;
    string_http_status_code_map[ "421 Misdirected Request" ] = MISDIRECTED_REQUEST;
    string_http_status_code_map[ "422 Unprocessable Entity" ] =
        UNPROCESSABLE_ENTITY;
    string_http_status_code_map[ "423 Locked" ] = LOCKED;
    string_http_status_code_map[ "424 Failed Dependency" ] = FAILED_DEPENDENCY;
    string_http_status_code_map[ "425 Too Early" ] = TOO_EARLY;
    string_http_status_code_map[ "426 Upgrade Required" ] = UPGRADE_REQUIRED;
    string_http_status_code_map[ "428 Precondition Required" ] =
        PRECONDITION_REQUIRED;
    string_http_status_code_map[ "429 Too Many Requests" ] = TOO_MANY_REQUESTS;
    string_http_status_code_map[ "431 Request Header Fields Too Large" ] =
        REQUEST_HEADER_FIELDS_TOO_LARGE;
    string_http_status_code_map[ "451 Unavailable For Legal Reasons" ] =
        UNAVAILABLE_FOR_LEGAL_REASONS;
    string_http_status_code_map[ "500 Internal Server Error" ] =
        INTERNAL_SERVER_ERROR;
    string_http_status_code_map[ "501 Not Implemented" ] = NOT_IMPLEMENTED;
    string_http_status_code_map[ "502 Bad Gateway" ] = BAD_GATEWAY;
    string_http_status_code_map[ "503 Service Unavailable" ] = SERVICE_UNAVAILABLE;
    string_http_status_code_map[ "504 Gateway Timeout" ] = GATEWAY_TIMEOUT;
    string_http_status_code_map[ "505 HTTP Version Not Supported" ] =
        HTTP_VERSION_NOT_SUPPORTED;
    string_http_status_code_map[ "506 Variant Also Negotiates" ] =
        VARIANT_ALSO_NEGOTIATES;
    string_http_status_code_map[ "507 Insufficient Storage" ] =
        INSUFFICIENT_STORAGE;
    string_http_status_code_map[ "508 Loop Detected" ] = LOOP_DETECTED;
    string_http_status_code_map[ "510 Not Extended" ] = NOT_EXTENDED;
    string_http_status_code_map[ "511 Network Authentication Required" ] =
        NETWORK_AUTHENTICATION_REQUIRED;

    return string_http_status_code_map;
}

// Helper function to initialize m_http_status_code_string_map with mappings from
// HttpStatusCode enum values to string representations
std::map<HttpStatusCode, std::string>
HttpStatusCodeHelper::m_setHttpStatusCodeStringMap()
{
    std::map<HttpStatusCode, std::string> http_status_code_string_map;

    // Add mappings from HttpStatusCode enum values to string representations
    http_status_code_string_map[ CONTINUE ] = "Continue";
    http_status_code_string_map[ SWITCHING_PROTOCOLS ] = "Switching Protocols";
    http_status_code_string_map[ PROCESSING ] = "Processing";
    http_status_code_string_map[ EARLY_HINTS ] = "Early Hints";
    http_status_code_string_map[ OK ] = "OK";
    http_status_code_string_map[ CREATED ] = "Created";
    http_status_code_string_map[ ACCEPTED ] = "Accepted";
    http_status_code_string_map[ NON_AUTHORITATIVE_INFORMATION ] =
        "Non-Authoritative Information";
    http_status_code_string_map[ NO_CONTENT ] = "No Content";
    http_status_code_string_map[ RESET_CONTENT ] = "Reset Content";
    http_status_code_string_map[ PARTIAL_CONTENT ] = "Partial Content";
    http_status_code_string_map[ MULTI_STATUS ] = "Multi-Status";
    http_status_code_string_map[ ALREADY_REPORTED ] = "Already Reported";
    http_status_code_string_map[ IM_USED ] = "IM Used";
    http_status_code_string_map[ MULTIPLE_CHOICES ] = "Multiple Choices";
    http_status_code_string_map[ MOVED_PERMANENTLY ] = "Moved Permanently";
    http_status_code_string_map[ FOUND ] = "Found";
    http_status_code_string_map[ SEE_OTHER ] = "See Other";
    http_status_code_string_map[ NOT_MODIFIED ] = "Not Modified";
    http_status_code_string_map[ USE_PROXY ] = "Use Proxy";
    http_status_code_string_map[ SWITCH_PROXY ] = "Switch Proxy";
    http_status_code_string_map[ TEMPORARY_REDIRECT ] = "Temporary Redirect";
    http_status_code_string_map[ PERMANENT_REDIRECT ] = "Permanent Redirect";
    http_status_code_string_map[ BAD_REQUEST ] = "Bad Request";
    http_status_code_string_map[ UNAUTHORIZED ] = "Unauthorized";
    http_status_code_string_map[ PAYMENT_REQUIRED ] = "Payment Required";
    http_status_code_string_map[ FORBIDDEN ] = "Forbidden";
    http_status_code_string_map[ NOT_FOUND ] = "Not Found";
    http_status_code_string_map[ METHOD_NOT_ALLOWED ] = "Method Not Allowed";
    http_status_code_string_map[ NOT_ACCEPTABLE ] = "Not Acceptable";
    http_status_code_string_map[ PROXY_AUTHENTICATION_REQUIRED ] =
        "Proxy Authentication Required";
    http_status_code_string_map[ REQUEST_TIMEOUT ] = "Request Timeout";
    http_status_code_string_map[ CONFLICT ] = "Conflict";
    http_status_code_string_map[ GONE ] = "Gone";
    http_status_code_string_map[ LENGTH_REQUIRED ] = "Length Required";
    http_status_code_string_map[ PRECONDITION_FAILED ] = "Precondition Failed";
    http_status_code_string_map[ PAYLOAD_TOO_LARGE ] = "Payload Too Large";
    http_status_code_string_map[ URI_TOO_LONG ] = "URI Too Long";
    http_status_code_string_map[ UNSUPPORTED_MEDIA_TYPE ] =
        "Unsupported Media Type";
    http_status_code_string_map[ RANGE_NOT_SATISFIABLE ] = "Range Not Satisfiable";
    http_status_code_string_map[ EXPECTATION_FAILED ] = "Expectation Failed";
    http_status_code_string_map[ IM_A_TEAPOT ] = "I'm a teapot";
    http_status_code_string_map[ MISDIRECTED_REQUEST ] = "Misdirected Request";
    http_status_code_string_map[ UNPROCESSABLE_ENTITY ] = "Unprocessable Entity";
    http_status_code_string_map[ LOCKED ] = "Locked";
    http_status_code_string_map[ FAILED_DEPENDENCY ] = "Failed Dependency";
    http_status_code_string_map[ TOO_EARLY ] = "Too Early";
    http_status_code_string_map[ UPGRADE_REQUIRED ] = "Upgrade Required";
    http_status_code_string_map[ PRECONDITION_REQUIRED ] = "Precondition Required";
    http_status_code_string_map[ TOO_MANY_REQUESTS ] = "Too Many Requests";
    http_status_code_string_map[ REQUEST_HEADER_FIELDS_TOO_LARGE ] =
        "Request Header Fields Too Large";
    http_status_code_string_map[ UNAVAILABLE_FOR_LEGAL_REASONS ] =
        "Unavailable For Legal Reasons";
    http_status_code_string_map[ INTERNAL_SERVER_ERROR ] = "Internal Server Error";
    http_status_code_string_map[ NOT_IMPLEMENTED ] = "Not Implemented";
    http_status_code_string_map[ BAD_GATEWAY ] = "Bad Gateway";
    http_status_code_string_map[ SERVICE_UNAVAILABLE ] = "Service Unavailable";
    http_status_code_string_map[ GATEWAY_TIMEOUT ] = "Gateway Timeout";
    http_status_code_string_map[ HTTP_VERSION_NOT_SUPPORTED ] =
        "HTTP Version Not Supported";
    http_status_code_string_map[ VARIANT_ALSO_NEGOTIATES ] =
        "Variant Also Negotiates";
    http_status_code_string_map[ INSUFFICIENT_STORAGE ] = "Insufficient Storage";
    http_status_code_string_map[ LOOP_DETECTED ] = "Loop Detected";
    http_status_code_string_map[ NOT_EXTENDED ] = "Not Extended";
    http_status_code_string_map[ NETWORK_AUTHENTICATION_REQUIRED ] =
        "Network Authentication Required";

    return http_status_code_string_map;
}

// Path: includes/constants/HttpStatusCodeHelper.hpp
