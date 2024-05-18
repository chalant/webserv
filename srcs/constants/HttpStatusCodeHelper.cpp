#include "../../includes/constants/HttpStatusCodeHelper.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include "../../includes/utils/Converter.hpp"

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
        throw UnknownHttpStatusCodeError(Converter::toString(statusCode));
    }
}

// Generate a status line string for an HTTP response
std::string HttpStatusCodeHelper::getStatusLine(HttpStatusCode statusCode) const
{
    return "HTTP/1.1 " + Converter::toString(static_cast<size_t>(statusCode)) + " " + this->httpStatusCodeStringMap(statusCode) + "\r\n";
}

// Generate a complete error response for an HTTP status code
std::string HttpStatusCodeHelper::getErrorResponse(HttpStatusCode statusCode) const
{
    std::string body = this->getHtmlPage(statusCode);

    return this->getStatusLine(statusCode) + "\n" + "content-type: text/html\r\n" + "content-length: " + Converter::toString(body.length()) + "\r\n" + "Connection: close\r\n" + "Server: webserv/1.0\r\n" + "\r\n" + body;
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
                "    <title>Webserv | "
                << static_cast<size_t>(statusCode)
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
    stringHttpStatusCodeMap["100 Continue"] = CONTINUE;
    stringHttpStatusCodeMap["101 Switching Protocols"] = SWITCHING_PROTOCOLS;
    stringHttpStatusCodeMap["102 Processing"] = PROCESSING;
    stringHttpStatusCodeMap["103 Early Hints"] = EARLY_HINTS;
    stringHttpStatusCodeMap["200 OK"] = OK;
    stringHttpStatusCodeMap["201 Created"] = CREATED;
    stringHttpStatusCodeMap["202 Accepted"] = ACCEPTED;
    stringHttpStatusCodeMap["203 Non-Authoritative Information"] = NON_AUTHORITATIVE_INFORMATION;
    stringHttpStatusCodeMap["204 No Content"] = NO_CONTENT;
    stringHttpStatusCodeMap["205 Reset Content"] = RESET_CONTENT;
    stringHttpStatusCodeMap["206 Partial Content"] = PARTIAL_CONTENT;
    stringHttpStatusCodeMap["207 Multi-Status"] = MULTI_STATUS;
    stringHttpStatusCodeMap["208 Already Reported"] = ALREADY_REPORTED;
    stringHttpStatusCodeMap["226 IM Used"] = IM_USED;
    stringHttpStatusCodeMap["300 Multiple Choices"] = MULTIPLE_CHOICES;
    stringHttpStatusCodeMap["301 Moved Permanently"] = MOVED_PERMANENTLY;
    stringHttpStatusCodeMap["302 Found"] = FOUND;
    stringHttpStatusCodeMap["303 See Other"] = SEE_OTHER;
    stringHttpStatusCodeMap["304 Not Modified"] = NOT_MODIFIED;
    stringHttpStatusCodeMap["305 Use Proxy"] = USE_PROXY;
    stringHttpStatusCodeMap["306 Switch Proxy"] = SWITCH_PROXY;
    stringHttpStatusCodeMap["307 Temporary Redirect"] = TEMPORARY_REDIRECT;
    stringHttpStatusCodeMap["308 Permanent Redirect"] = PERMANENT_REDIRECT;
    stringHttpStatusCodeMap["400 Bad Request"] = BAD_REQUEST;
    stringHttpStatusCodeMap["401 Unauthorized"] = UNAUTHORIZED;
    stringHttpStatusCodeMap["402 Payment Required"] = PAYMENT_REQUIRED;
    stringHttpStatusCodeMap["403 Forbidden"] = FORBIDDEN;
    stringHttpStatusCodeMap["404 Not Found"] = NOT_FOUND;
    stringHttpStatusCodeMap["405 Method Not Allowed"] = METHOD_NOT_ALLOWED;
    stringHttpStatusCodeMap["406 Not Acceptable"] = NOT_ACCEPTABLE;
    stringHttpStatusCodeMap["407 Proxy Authentication Required"] = PROXY_AUTHENTICATION_REQUIRED;
    stringHttpStatusCodeMap["408 Request Timeout"] = REQUEST_TIMEOUT;
    stringHttpStatusCodeMap["409 Conflict"] = CONFLICT;
    stringHttpStatusCodeMap["410 Gone"] = GONE;
    stringHttpStatusCodeMap["411 Length Required"] = LENGTH_REQUIRED;
    stringHttpStatusCodeMap["412 Precondition Failed"] = PRECONDITION_FAILED;
    stringHttpStatusCodeMap["413 Payload Too Large"] = PAYLOAD_TOO_LARGE;
    stringHttpStatusCodeMap["414 URI Too Long"] = URI_TOO_LONG;
    stringHttpStatusCodeMap["415 Unsupported Media Type"] = UNSUPPORTED_MEDIA_TYPE;
    stringHttpStatusCodeMap["416 Range Not Satisfiable"] = RANGE_NOT_SATISFIABLE;
    stringHttpStatusCodeMap["417 Expectation Failed"] = EXPECTATION_FAILED;
    stringHttpStatusCodeMap["418 I'm a teapot"] = IM_A_TEAPOT;
    stringHttpStatusCodeMap["421 Misdirected Request"] = MISDIRECTED_REQUEST;
    stringHttpStatusCodeMap["422 Unprocessable Entity"] = UNPROCESSABLE_ENTITY;
    stringHttpStatusCodeMap["423 Locked"] = LOCKED;
    stringHttpStatusCodeMap["424 Failed Dependency"] = FAILED_DEPENDENCY;
    stringHttpStatusCodeMap["425 Too Early"] = TOO_EARLY;
    stringHttpStatusCodeMap["426 Upgrade Required"] = UPGRADE_REQUIRED;
    stringHttpStatusCodeMap["428 Precondition Required"] = PRECONDITION_REQUIRED;
    stringHttpStatusCodeMap["429 Too Many Requests"] = TOO_MANY_REQUESTS;
    stringHttpStatusCodeMap["431 Request Header Fields Too Large"] = REQUEST_HEADER_FIELDS_TOO_LARGE;
    stringHttpStatusCodeMap["451 Unavailable For Legal Reasons"] = UNAVAILABLE_FOR_LEGAL_REASONS;
    stringHttpStatusCodeMap["500 Internal Server Error"] = INTERNAL_SERVER_ERROR;
    stringHttpStatusCodeMap["501 Not Implemented"] = NOT_IMPLEMENTED;
    stringHttpStatusCodeMap["502 Bad Gateway"] = BAD_GATEWAY;
    stringHttpStatusCodeMap["503 Service Unavailable"] = SERVICE_UNAVAILABLE;
    stringHttpStatusCodeMap["504 Gateway Timeout"] = GATEWAY_TIMEOUT;
    stringHttpStatusCodeMap["505 HTTP Version Not Supported"] = HTTP_VERSION_NOT_SUPPORTED;
    stringHttpStatusCodeMap["506 Variant Also Negotiates"] = VARIANT_ALSO_NEGOTIATES;
    stringHttpStatusCodeMap["507 Insufficient Storage"] = INSUFFICIENT_STORAGE;
    stringHttpStatusCodeMap["508 Loop Detected"] = LOOP_DETECTED;
    stringHttpStatusCodeMap["510 Not Extended"] = NOT_EXTENDED;
    stringHttpStatusCodeMap["511 Network Authentication Required"] = NETWORK_AUTHENTICATION_REQUIRED;

    return stringHttpStatusCodeMap;
}

// Helper function to initialize httpStatusCodeStringMap with mappings from HttpStatusCode enum values to string representations
std::map<HttpStatusCode, std::string> HttpStatusCodeHelper::_setHttpStatusCodeStringMap()
{
    std::map<HttpStatusCode, std::string> httpStatusCodeStringMap;

    // Add mappings from HttpStatusCode enum values to string representations
    httpStatusCodeStringMap[CONTINUE] = "Continue";
    httpStatusCodeStringMap[SWITCHING_PROTOCOLS] = "Switching Protocols";
    httpStatusCodeStringMap[PROCESSING] = "Processing";
    httpStatusCodeStringMap[EARLY_HINTS] = "Early Hints";
    httpStatusCodeStringMap[OK] = "OK";
    httpStatusCodeStringMap[CREATED] = "Created";
    httpStatusCodeStringMap[ACCEPTED] = "Accepted";
    httpStatusCodeStringMap[NON_AUTHORITATIVE_INFORMATION] = "Non-Authoritative Information";
    httpStatusCodeStringMap[NO_CONTENT] = "No Content";
    httpStatusCodeStringMap[RESET_CONTENT] = "Reset Content";
    httpStatusCodeStringMap[PARTIAL_CONTENT] = "Partial Content";
    httpStatusCodeStringMap[MULTI_STATUS] = "Multi-Status";
    httpStatusCodeStringMap[ALREADY_REPORTED] = "Already Reported";
    httpStatusCodeStringMap[IM_USED] = "IM Used";
    httpStatusCodeStringMap[MULTIPLE_CHOICES] = "Multiple Choices";
    httpStatusCodeStringMap[MOVED_PERMANENTLY] = "Moved Permanently";
    httpStatusCodeStringMap[FOUND] = "Found";
    httpStatusCodeStringMap[SEE_OTHER] = "See Other";
    httpStatusCodeStringMap[NOT_MODIFIED] = "Not Modified";
    httpStatusCodeStringMap[USE_PROXY] = "Use Proxy";
    httpStatusCodeStringMap[SWITCH_PROXY] = "Switch Proxy";
    httpStatusCodeStringMap[TEMPORARY_REDIRECT] = "Temporary Redirect";
    httpStatusCodeStringMap[PERMANENT_REDIRECT] = "Permanent Redirect";
    httpStatusCodeStringMap[BAD_REQUEST] = "Bad Request";
    httpStatusCodeStringMap[UNAUTHORIZED] = "Unauthorized";
    httpStatusCodeStringMap[PAYMENT_REQUIRED] = "Payment Required";
    httpStatusCodeStringMap[FORBIDDEN] = "Forbidden";
    httpStatusCodeStringMap[NOT_FOUND] = "Not Found";
    httpStatusCodeStringMap[METHOD_NOT_ALLOWED] = "Method Not Allowed";
    httpStatusCodeStringMap[NOT_ACCEPTABLE] = "Not Acceptable";
    httpStatusCodeStringMap[PROXY_AUTHENTICATION_REQUIRED] = "Proxy Authentication Required";
    httpStatusCodeStringMap[REQUEST_TIMEOUT] = "Request Timeout";
    httpStatusCodeStringMap[CONFLICT] = "Conflict";
    httpStatusCodeStringMap[GONE] = "Gone";
    httpStatusCodeStringMap[LENGTH_REQUIRED] = "Length Required";
    httpStatusCodeStringMap[PRECONDITION_FAILED] = "Precondition Failed";
    httpStatusCodeStringMap[PAYLOAD_TOO_LARGE] = "Payload Too Large";
    httpStatusCodeStringMap[URI_TOO_LONG] = "URI Too Long";
    httpStatusCodeStringMap[UNSUPPORTED_MEDIA_TYPE] = "Unsupported Media Type";
    httpStatusCodeStringMap[RANGE_NOT_SATISFIABLE] = "Range Not Satisfiable";
    httpStatusCodeStringMap[EXPECTATION_FAILED] = "Expectation Failed";
    httpStatusCodeStringMap[IM_A_TEAPOT] = "I'm a teapot";
    httpStatusCodeStringMap[MISDIRECTED_REQUEST] = "Misdirected Request";
    httpStatusCodeStringMap[UNPROCESSABLE_ENTITY] = "Unprocessable Entity";
    httpStatusCodeStringMap[LOCKED] = "Locked";
    httpStatusCodeStringMap[FAILED_DEPENDENCY] = "Failed Dependency";
    httpStatusCodeStringMap[TOO_EARLY] = "Too Early";
    httpStatusCodeStringMap[UPGRADE_REQUIRED] = "Upgrade Required";
    httpStatusCodeStringMap[PRECONDITION_REQUIRED] = "Precondition Required";
    httpStatusCodeStringMap[TOO_MANY_REQUESTS] = "Too Many Requests";
    httpStatusCodeStringMap[REQUEST_HEADER_FIELDS_TOO_LARGE] = "Request Header Fields Too Large";
    httpStatusCodeStringMap[UNAVAILABLE_FOR_LEGAL_REASONS] = "Unavailable For Legal Reasons";
    httpStatusCodeStringMap[INTERNAL_SERVER_ERROR] = "Internal Server Error";
    httpStatusCodeStringMap[NOT_IMPLEMENTED] = "Not Implemented";
    httpStatusCodeStringMap[BAD_GATEWAY] = "Bad Gateway";
    httpStatusCodeStringMap[SERVICE_UNAVAILABLE] = "Service Unavailable";
    httpStatusCodeStringMap[GATEWAY_TIMEOUT] = "Gateway Timeout";
    httpStatusCodeStringMap[HTTP_VERSION_NOT_SUPPORTED] = "HTTP Version Not Supported";
    httpStatusCodeStringMap[VARIANT_ALSO_NEGOTIATES] = "Variant Also Negotiates";
    httpStatusCodeStringMap[INSUFFICIENT_STORAGE] = "Insufficient Storage";
    httpStatusCodeStringMap[LOOP_DETECTED] = "Loop Detected";
    httpStatusCodeStringMap[NOT_EXTENDED] = "Not Extended";
    httpStatusCodeStringMap[NETWORK_AUTHENTICATION_REQUIRED] = "Network Authentication Required";

    return httpStatusCodeStringMap;
}

// Path: includes/constants/HttpStatusCodeHelper.hpp