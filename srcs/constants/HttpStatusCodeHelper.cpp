#include "../includes/constants/HttpStatusCodeHelper.hpp"

/*
 * HttpStatusCodeHelper.hpp
 *
 * The HttpStatusCodeHelper class provides functionality for working with HTTP status codes.
 * It defines an enum HttpStatusCode to represent standard HTTP status codes and provides methods
 * to retrieve string representations of status codes and vice versa.
 *
 */

// Constructor initializes member variables using helper functions
HttpStatusCodeHelper::HttpStatusCodeHelper()
    : _statusCodeList(_setStatusCodeList()),
      _stringHttpStatusCodeMap(_setStringHttpStatusCodeMap()),
      _httpStatusCodeStringMap(_setHttpStatusCodeStringMap()) {}

// Get string representation of HttpStatusCode enum value
const std::string &HttpStatusCodeHelper::httpStatusCodeStringMap(HttpStatusCode statusCode) const
{
    if (_httpStatusCodeStringMap.find(statusCode) != _httpStatusCodeStringMap.end())
    {
        return _httpStatusCodeStringMap.at(statusCode);
    }
    else
    {
        throw UnknownHttpStatusCodeError();
    }
}

// Get HttpStatusCode enum value from string representation
HttpStatusCode HttpStatusCodeHelper::stringHttpStatusCodeMap(const std::string &statusCode) const
{
    if (_stringHttpStatusCodeMap.find(statusCode) != _stringHttpStatusCodeMap.end())
    {
        return _stringHttpStatusCodeMap.at(statusCode);
    }
    else
    {
        throw UnknownHttpStatusCodeError(statusCode);
    }
}

// Get HttpStatusCode enum value from string representation
HttpStatusCode HttpStatusCodeHelper::intHttpStatusCodeMap(const int &statusCode) const
{
    if (statusCode >= 100 && statusCode <= 511)
    {
        return static_cast<HttpStatusCode>(statusCode);
    }
    else
    {
        throw UnknownHttpStatusCodeError(std::to_string(statusCode));
    }
}

// Generate a status line string for an HTTP response
std::string HttpStatusCodeHelper::getStatusLine(HttpStatusCode statusCode) const
{
    return "HTTP/1.1 " + std::to_string(static_cast<size_t>(statusCode)) + " " + this->httpStatusCodeStringMap(statusCode) + "\r\n";
}

// Generate a complete error response for an HTTP status code
std::string HttpStatusCodeHelper::getErrorResponse(HttpStatusCode statusCode) const
{
    std::string body = this->getHtmlPage(statusCode);

    return this->getStatusLine(statusCode) + "\n" + "Content-Type: text/html\r\n" + "Content-Length: " + std::to_string(body.length()) + "\r\n" + "Connection: close\r\n" + "Server: webserv/1.0\r\n" + "\r\n" + body;
}

// Generate an HTML page with the specified HTTP status code
std::string HttpStatusCodeHelper::getHtmlPage(HttpStatusCode statusCode) const
{
    // Create a string stream to build the HTML page.
    std::stringstream htmlPage;

    // Write the HTML page
    htmlPage << "<!DOCTYPE html>\n"
                "<html lang=\"en\">\n"
                "<head>\n"
                "    <meta charset=\"UTF-8\">\n"
                "    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n"
                "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                "    <title>Error Page</title>\n"
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
             << static_cast<size_t>(statusCode)
             << "</span></h1>\n"
                "            <p> "
             << this->_httpStatusCodeStringMap.at(statusCode)
             << "</p>\n"
                "            <div class=\"version\">webserv/1.0</div>\n"
                "        </div>\n"
                "    </div>\n"
                "</body>\n"
                "</html>\n";

    // Return the generated HTML page.
    return htmlPage.str();
}

// Helper function to initialize StatusCodeList with string representations of HTTP status codes
std::vector<std::string> HttpStatusCodeHelper::_setStatusCodeList()
{
    std::vector<std::string> StatusCodeList;

    // Add string representations of HTTP status codes to StatusCodeList
    StatusCodeList.push_back("100 Continue");
    StatusCodeList.push_back("101 Switching Protocols");
    StatusCodeList.push_back("102 Processing");
    StatusCodeList.push_back("103 Early Hints");
    StatusCodeList.push_back("200 OK");
    StatusCodeList.push_back("201 Created");
    StatusCodeList.push_back("202 Accepted");
    StatusCodeList.push_back("203 Non-Authoritative Information");
    StatusCodeList.push_back("204 No Content");
    StatusCodeList.push_back("205 Reset Content");
    StatusCodeList.push_back("206 Partial Content");
    StatusCodeList.push_back("207 Multi-Status");
    StatusCodeList.push_back("208 Already Reported");
    StatusCodeList.push_back("226 IM Used");
    StatusCodeList.push_back("300 Multiple Choices");
    StatusCodeList.push_back("301 Moved Permanently");
    StatusCodeList.push_back("302 Found");
    StatusCodeList.push_back("303 See Other");
    StatusCodeList.push_back("304 Not Modified");
    StatusCodeList.push_back("305 Use Proxy");
    StatusCodeList.push_back("306 Switch Proxy");
    StatusCodeList.push_back("307 Temporary Redirect");
    StatusCodeList.push_back("308 Permanent Redirect");
    StatusCodeList.push_back("400 Bad Request");
    StatusCodeList.push_back("401 Unauthorized");
    StatusCodeList.push_back("402 Payment Required");
    StatusCodeList.push_back("403 Forbidden");
    StatusCodeList.push_back("404 Not Found");
    StatusCodeList.push_back("405 Method Not Allowed");
    StatusCodeList.push_back("406 Not Acceptable");
    StatusCodeList.push_back("407 Proxy Authentication Required");
    StatusCodeList.push_back("408 Request Timeout");
    StatusCodeList.push_back("409 Conflict");
    StatusCodeList.push_back("410 Gone");
    StatusCodeList.push_back("411 Length Required");
    StatusCodeList.push_back("412 Precondition Failed");
    StatusCodeList.push_back("413 Payload Too Large");
    StatusCodeList.push_back("414 URI Too Long");
    StatusCodeList.push_back("415 Unsupported Media Type");
    StatusCodeList.push_back("416 Range Not Satisfiable");
    StatusCodeList.push_back("417 Expectation Failed");
    StatusCodeList.push_back("418 I'm a teapot");
    StatusCodeList.push_back("421 Misdirected Request");
    StatusCodeList.push_back("422 Unprocessable Entity");
    StatusCodeList.push_back("423 Locked");
    StatusCodeList.push_back("424 Failed Dependency");
    StatusCodeList.push_back("425 Too Early");
    StatusCodeList.push_back("426 Upgrade Required");
    StatusCodeList.push_back("428 Precondition Required");
    StatusCodeList.push_back("429 Too Many Requests");
    StatusCodeList.push_back("431 Request Header Fields Too Large");
    StatusCodeList.push_back("451 Unavailable For Legal Reasons");
    StatusCodeList.push_back("500 Internal Server Error");
    StatusCodeList.push_back("501 Not Implemented");
    StatusCodeList.push_back("502 Bad Gateway");
    StatusCodeList.push_back("503 Service Unavailable");
    StatusCodeList.push_back("504 Gateway Timeout");
    StatusCodeList.push_back("505 HTTP Version Not Supported");
    StatusCodeList.push_back("506 Variant Also Negotiates");
    StatusCodeList.push_back("507 Insufficient Storage");
    StatusCodeList.push_back("508 Loop Detected");
    StatusCodeList.push_back("510 Not Extended");
    StatusCodeList.push_back("511 Network Authentication Required");

    return StatusCodeList;
}

// Helper function to initialize stringHttpStatusCodeMap with mappings from string representations to HttpStatusCode enum values
std::map<std::string, HttpStatusCode> HttpStatusCodeHelper::_setStringHttpStatusCodeMap()
{
    std::map<std::string, HttpStatusCode> stringHttpStatusCodeMap;

    // Add mappings from string representations to HttpStatusCode enum values
    stringHttpStatusCodeMap["100 Continue"] = HttpStatusCode::CONTINUE;
    stringHttpStatusCodeMap["101 Switching Protocols"] = HttpStatusCode::SWITCHING_PROTOCOLS;
    stringHttpStatusCodeMap["102 Processing"] = HttpStatusCode::PROCESSING;
    stringHttpStatusCodeMap["103 Early Hints"] = HttpStatusCode::EARLY_HINTS;
    stringHttpStatusCodeMap["200 OK"] = HttpStatusCode::OK;
    stringHttpStatusCodeMap["201 Created"] = HttpStatusCode::CREATED;
    stringHttpStatusCodeMap["202 Accepted"] = HttpStatusCode::ACCEPTED;
    stringHttpStatusCodeMap["203 Non-Authoritative Information"] = HttpStatusCode::NON_AUTHORITATIVE_INFORMATION;
    stringHttpStatusCodeMap["204 No Content"] = HttpStatusCode::NO_CONTENT;
    stringHttpStatusCodeMap["205 Reset Content"] = HttpStatusCode::RESET_CONTENT;
    stringHttpStatusCodeMap["206 Partial Content"] = HttpStatusCode::PARTIAL_CONTENT;
    stringHttpStatusCodeMap["207 Multi-Status"] = HttpStatusCode::MULTI_STATUS;
    stringHttpStatusCodeMap["208 Already Reported"] = HttpStatusCode::ALREADY_REPORTED;
    stringHttpStatusCodeMap["226 IM Used"] = HttpStatusCode::IM_USED;
    stringHttpStatusCodeMap["300 Multiple Choices"] = HttpStatusCode::MULTIPLE_CHOICES;
    stringHttpStatusCodeMap["301 Moved Permanently"] = HttpStatusCode::MOVED_PERMANENTLY;
    stringHttpStatusCodeMap["302 Found"] = HttpStatusCode::FOUND;
    stringHttpStatusCodeMap["303 See Other"] = HttpStatusCode::SEE_OTHER;
    stringHttpStatusCodeMap["304 Not Modified"] = HttpStatusCode::NOT_MODIFIED;
    stringHttpStatusCodeMap["305 Use Proxy"] = HttpStatusCode::USE_PROXY;
    stringHttpStatusCodeMap["306 Switch Proxy"] = HttpStatusCode::SWITCH_PROXY;
    stringHttpStatusCodeMap["307 Temporary Redirect"] = HttpStatusCode::TEMPORARY_REDIRECT;
    stringHttpStatusCodeMap["308 Permanent Redirect"] = HttpStatusCode::PERMANENT_REDIRECT;
    stringHttpStatusCodeMap["400 Bad Request"] = HttpStatusCode::BAD_REQUEST;
    stringHttpStatusCodeMap["401 Unauthorized"] = HttpStatusCode::UNAUTHORIZED;
    stringHttpStatusCodeMap["402 Payment Required"] = HttpStatusCode::PAYMENT_REQUIRED;
    stringHttpStatusCodeMap["403 Forbidden"] = HttpStatusCode::FORBIDDEN;
    stringHttpStatusCodeMap["404 Not Found"] = HttpStatusCode::NOT_FOUND;
    stringHttpStatusCodeMap["405 Method Not Allowed"] = HttpStatusCode::METHOD_NOT_ALLOWED;
    stringHttpStatusCodeMap["406 Not Acceptable"] = HttpStatusCode::NOT_ACCEPTABLE;
    stringHttpStatusCodeMap["407 Proxy Authentication Required"] = HttpStatusCode::PROXY_AUTHENTICATION_REQUIRED;
    stringHttpStatusCodeMap["408 Request Timeout"] = HttpStatusCode::REQUEST_TIMEOUT;
    stringHttpStatusCodeMap["409 Conflict"] = HttpStatusCode::CONFLICT;
    stringHttpStatusCodeMap["410 Gone"] = HttpStatusCode::GONE;
    stringHttpStatusCodeMap["411 Length Required"] = HttpStatusCode::LENGTH_REQUIRED;
    stringHttpStatusCodeMap["412 Precondition Failed"] = HttpStatusCode::PRECONDITION_FAILED;
    stringHttpStatusCodeMap["413 Payload Too Large"] = HttpStatusCode::PAYLOAD_TOO_LARGE;
    stringHttpStatusCodeMap["414 URI Too Long"] = HttpStatusCode::URI_TOO_LONG;
    stringHttpStatusCodeMap["415 Unsupported Media Type"] = HttpStatusCode::UNSUPPORTED_MEDIA_TYPE;
    stringHttpStatusCodeMap["416 Range Not Satisfiable"] = HttpStatusCode::RANGE_NOT_SATISFIABLE;
    stringHttpStatusCodeMap["417 Expectation Failed"] = HttpStatusCode::EXPECTATION_FAILED;
    stringHttpStatusCodeMap["418 I'm a teapot"] = HttpStatusCode::IM_A_TEAPOT;
    stringHttpStatusCodeMap["421 Misdirected Request"] = HttpStatusCode::MISDIRECTED_REQUEST;
    stringHttpStatusCodeMap["422 Unprocessable Entity"] = HttpStatusCode::UNPROCESSABLE_ENTITY;
    stringHttpStatusCodeMap["423 Locked"] = HttpStatusCode::LOCKED;
    stringHttpStatusCodeMap["424 Failed Dependency"] = HttpStatusCode::FAILED_DEPENDENCY;
    stringHttpStatusCodeMap["425 Too Early"] = HttpStatusCode::TOO_EARLY;
    stringHttpStatusCodeMap["426 Upgrade Required"] = HttpStatusCode::UPGRADE_REQUIRED;
    stringHttpStatusCodeMap["428 Precondition Required"] = HttpStatusCode::PRECONDITION_REQUIRED;
    stringHttpStatusCodeMap["429 Too Many Requests"] = HttpStatusCode::TOO_MANY_REQUESTS;
    stringHttpStatusCodeMap["431 Request Header Fields Too Large"] = HttpStatusCode::REQUEST_HEADER_FIELDS_TOO_LARGE;
    stringHttpStatusCodeMap["451 Unavailable For Legal Reasons"] = HttpStatusCode::UNAVAILABLE_FOR_LEGAL_REASONS;
    stringHttpStatusCodeMap["500 Internal Server Error"] = HttpStatusCode::INTERNAL_SERVER_ERROR;
    stringHttpStatusCodeMap["501 Not Implemented"] = HttpStatusCode::NOT_IMPLEMENTED;
    stringHttpStatusCodeMap["502 Bad Gateway"] = HttpStatusCode::BAD_GATEWAY;
    stringHttpStatusCodeMap["503 Service Unavailable"] = HttpStatusCode::SERVICE_UNAVAILABLE;
    stringHttpStatusCodeMap["504 Gateway Timeout"] = HttpStatusCode::GATEWAY_TIMEOUT;
    stringHttpStatusCodeMap["505 HTTP Version Not Supported"] = HttpStatusCode::HTTP_VERSION_NOT_SUPPORTED;
    stringHttpStatusCodeMap["506 Variant Also Negotiates"] = HttpStatusCode::VARIANT_ALSO_NEGOTIATES;
    stringHttpStatusCodeMap["507 Insufficient Storage"] = HttpStatusCode::INSUFFICIENT_STORAGE;
    stringHttpStatusCodeMap["508 Loop Detected"] = HttpStatusCode::LOOP_DETECTED;
    stringHttpStatusCodeMap["510 Not Extended"] = HttpStatusCode::NOT_EXTENDED;
    stringHttpStatusCodeMap["511 Network Authentication Required"] = HttpStatusCode::NETWORK_AUTHENTICATION_REQUIRED;

    return stringHttpStatusCodeMap;
}

// Helper function to initialize httpStatusCodeStringMap with mappings from HttpStatusCode enum values to string representations
std::map<HttpStatusCode, std::string> HttpStatusCodeHelper::_setHttpStatusCodeStringMap()
{
    std::map<HttpStatusCode, std::string> httpStatusCodeStringMap;

    // Add mappings from HttpStatusCode enum values to string representations
    httpStatusCodeStringMap[HttpStatusCode::CONTINUE] = "Continue";
    httpStatusCodeStringMap[HttpStatusCode::SWITCHING_PROTOCOLS] = "Switching Protocols";
    httpStatusCodeStringMap[HttpStatusCode::PROCESSING] = "Processing";
    httpStatusCodeStringMap[HttpStatusCode::EARLY_HINTS] = "Early Hints";
    httpStatusCodeStringMap[HttpStatusCode::OK] = "OK";
    httpStatusCodeStringMap[HttpStatusCode::CREATED] = "Created";
    httpStatusCodeStringMap[HttpStatusCode::ACCEPTED] = "Accepted";
    httpStatusCodeStringMap[HttpStatusCode::NON_AUTHORITATIVE_INFORMATION] = "Non-Authoritative Information";
    httpStatusCodeStringMap[HttpStatusCode::NO_CONTENT] = "No Content";
    httpStatusCodeStringMap[HttpStatusCode::RESET_CONTENT] = "Reset Content";
    httpStatusCodeStringMap[HttpStatusCode::PARTIAL_CONTENT] = "Partial Content";
    httpStatusCodeStringMap[HttpStatusCode::MULTI_STATUS] = "Multi-Status";
    httpStatusCodeStringMap[HttpStatusCode::ALREADY_REPORTED] = "Already Reported";
    httpStatusCodeStringMap[HttpStatusCode::IM_USED] = "IM Used";
    httpStatusCodeStringMap[HttpStatusCode::MULTIPLE_CHOICES] = "Multiple Choices";
    httpStatusCodeStringMap[HttpStatusCode::MOVED_PERMANENTLY] = "Moved Permanently";
    httpStatusCodeStringMap[HttpStatusCode::FOUND] = "Found";
    httpStatusCodeStringMap[HttpStatusCode::SEE_OTHER] = "See Other";
    httpStatusCodeStringMap[HttpStatusCode::NOT_MODIFIED] = "Not Modified";
    httpStatusCodeStringMap[HttpStatusCode::USE_PROXY] = "Use Proxy";
    httpStatusCodeStringMap[HttpStatusCode::SWITCH_PROXY] = "Switch Proxy";
    httpStatusCodeStringMap[HttpStatusCode::TEMPORARY_REDIRECT] = "Temporary Redirect";
    httpStatusCodeStringMap[HttpStatusCode::PERMANENT_REDIRECT] = "Permanent Redirect";
    httpStatusCodeStringMap[HttpStatusCode::BAD_REQUEST] = "Bad Request";
    httpStatusCodeStringMap[HttpStatusCode::UNAUTHORIZED] = "Unauthorized";
    httpStatusCodeStringMap[HttpStatusCode::PAYMENT_REQUIRED] = "Payment Required";
    httpStatusCodeStringMap[HttpStatusCode::FORBIDDEN] = "Forbidden";
    httpStatusCodeStringMap[HttpStatusCode::NOT_FOUND] = "Not Found";
    httpStatusCodeStringMap[HttpStatusCode::METHOD_NOT_ALLOWED] = "Method Not Allowed";
    httpStatusCodeStringMap[HttpStatusCode::NOT_ACCEPTABLE] = "Not Acceptable";
    httpStatusCodeStringMap[HttpStatusCode::PROXY_AUTHENTICATION_REQUIRED] = "Proxy Authentication Required";
    httpStatusCodeStringMap[HttpStatusCode::REQUEST_TIMEOUT] = "Request Timeout";
    httpStatusCodeStringMap[HttpStatusCode::CONFLICT] = "Conflict";
    httpStatusCodeStringMap[HttpStatusCode::GONE] = "Gone";
    httpStatusCodeStringMap[HttpStatusCode::LENGTH_REQUIRED] = "Length Required";
    httpStatusCodeStringMap[HttpStatusCode::PRECONDITION_FAILED] = "Precondition Failed";
    httpStatusCodeStringMap[HttpStatusCode::PAYLOAD_TOO_LARGE] = "Payload Too Large";
    httpStatusCodeStringMap[HttpStatusCode::URI_TOO_LONG] = "URI Too Long";
    httpStatusCodeStringMap[HttpStatusCode::UNSUPPORTED_MEDIA_TYPE] = "Unsupported Media Type";
    httpStatusCodeStringMap[HttpStatusCode::RANGE_NOT_SATISFIABLE] = "Range Not Satisfiable";
    httpStatusCodeStringMap[HttpStatusCode::EXPECTATION_FAILED] = "Expectation Failed";
    httpStatusCodeStringMap[HttpStatusCode::IM_A_TEAPOT] = "I'm a teapot";
    httpStatusCodeStringMap[HttpStatusCode::MISDIRECTED_REQUEST] = "Misdirected Request";
    httpStatusCodeStringMap[HttpStatusCode::UNPROCESSABLE_ENTITY] = "Unprocessable Entity";
    httpStatusCodeStringMap[HttpStatusCode::LOCKED] = "Locked";
    httpStatusCodeStringMap[HttpStatusCode::FAILED_DEPENDENCY] = "Failed Dependency";
    httpStatusCodeStringMap[HttpStatusCode::TOO_EARLY] = "Too Early";
    httpStatusCodeStringMap[HttpStatusCode::UPGRADE_REQUIRED] = "Upgrade Required";
    httpStatusCodeStringMap[HttpStatusCode::PRECONDITION_REQUIRED] = "Precondition Required";
    httpStatusCodeStringMap[HttpStatusCode::TOO_MANY_REQUESTS] = "Too Many Requests";
    httpStatusCodeStringMap[HttpStatusCode::REQUEST_HEADER_FIELDS_TOO_LARGE] = "Request Header Fields Too Large";
    httpStatusCodeStringMap[HttpStatusCode::UNAVAILABLE_FOR_LEGAL_REASONS] = "Unavailable For Legal Reasons";
    httpStatusCodeStringMap[HttpStatusCode::INTERNAL_SERVER_ERROR] = "Internal Server Error";
    httpStatusCodeStringMap[HttpStatusCode::NOT_IMPLEMENTED] = "Not Implemented";
    httpStatusCodeStringMap[HttpStatusCode::BAD_GATEWAY] = "Bad Gateway";
    httpStatusCodeStringMap[HttpStatusCode::SERVICE_UNAVAILABLE] = "Service Unavailable";
    httpStatusCodeStringMap[HttpStatusCode::GATEWAY_TIMEOUT] = "Gateway Timeout";
    httpStatusCodeStringMap[HttpStatusCode::HTTP_VERSION_NOT_SUPPORTED] = "HTTP Version Not Supported";
    httpStatusCodeStringMap[HttpStatusCode::VARIANT_ALSO_NEGOTIATES] = "Variant Also Negotiates";
    httpStatusCodeStringMap[HttpStatusCode::INSUFFICIENT_STORAGE] = "Insufficient Storage";
    httpStatusCodeStringMap[HttpStatusCode::LOOP_DETECTED] = "Loop Detected";
    httpStatusCodeStringMap[HttpStatusCode::NOT_EXTENDED] = "Not Extended";
    httpStatusCodeStringMap[HttpStatusCode::NETWORK_AUTHENTICATION_REQUIRED] = "Network Authentication Required";

    return httpStatusCodeStringMap;
}

// Path: includes/constants/HttpStatusCodeHelper.hpp