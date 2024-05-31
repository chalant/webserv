#include "../mock_includes/MockRequest.hpp"
#include "../../includes/constants/HttpHelper.hpp"

/*
 * MockRequest class is a mock implementation of the IRequest interface
 * This mock implementation is used for testing purposes to simulate the
 * behavior of the IRequest interface during unit testing without relying on the
 * actual implementation.
 */

// Constructor
MockRequest::MockRequest() : IRequest() {};

// Destructor
MockRequest::~MockRequest() {};

// Copy constructor
MockRequest::MockRequest(const MockRequest &src) { static_cast<void>(src); };

// Copy assignment operator
MockRequest &MockRequest::operator=(const MockRequest &src)
{
    static_cast<void>(src);
    return *this;
};

// Getters
HttpMethod MockRequest::getMethod() const { return GET; };

std::string MockRequest::getMethodString() const { return m_test_method; };

std::string MockRequest::getTestMethod() const { return m_test_method; };

std::string MockRequest::getUri() const { return m_test_uri; };

HttpVersion MockRequest::getHttpVersion() const { return HTTP_1_1; };

std::string MockRequest::getTestHttpVersion() const
{
    return m_test_http_version;
};

std::string MockRequest::getHttpVersionString() const
{
    return m_test_http_version;
};

const std::map<HttpHeader, std::string> MockRequest::getHeaders() const
{
    return std::map<HttpHeader, std::string>();
};

std::string MockRequest::getHeaderValue(HttpHeader header) const
{
    HttpHelper helper;
    std::string header_enum = helper.httpHeaderStringMap(header);
    // Check if the header is present
    if (m_test_headers.find(header_enum) != m_test_headers.end())
        return m_test_headers.at(header_enum);
    else
        return "";
};

std::map<std::string, std::string> MockRequest::getHeadersStringMap() const
{
    return m_test_headers;
};

std::map<std::string, std::string> MockRequest::getTestHeadersStringMap() const
{
    return m_test_headers;
};

std::map<std::string, std::string> MockRequest::getQueryParameters() const
{
    return std::map<std::string, std::string>();
};

std::map<std::string, std::string> MockRequest::getCookies() const
{
    return std::map<std::string, std::string>();
};

std::string MockRequest::getCookie(const std::string &key) const
{
    static_cast<void>(key);
    static const std::string empty_string;
    return empty_string;
};

const std::vector<char> MockRequest::getBody() const { return m_test_body; };

std::string MockRequest::getBodyString() const
{
    return std::string(m_test_body.begin(), m_test_body.end());
};

std::string MockRequest::getQueryString() const
{
    // Check if the URI contains a query string
    size_t query_start = m_test_uri.find("?");
    if (query_start == std::string::npos)
        return "";
    return m_test_uri.substr(query_start + 1);
};

std::string MockRequest::getContentLength() const
{
    // Check if the content-length header is present
    if (m_test_headers.find("content-length") == m_test_headers.end())
        return "";
    return m_test_headers.at("content-length");
};

std::string MockRequest::getContentType() const
{
    // Check if the content-type header is present
    if (m_test_headers.find("content-type") == m_test_headers.end())
        return "";
    return m_test_headers.at("content-type");
};

std::string MockRequest::getPathInfo(const std::string &script_name) const
{
    // Check if the URI contains the script name
    size_t script_name_start = m_test_uri.find(script_name);
    if (script_name_start == std::string::npos)
        return "";

    // Calculate the start of the path info
    size_t path_info_start = script_name_start + script_name.length();

    // Get the start of the query string
    size_t query_start = m_test_uri.find("?");

    // Return the path info
    if (query_start == std::string::npos)
        return m_test_uri.substr(path_info_start);
    return m_test_uri.substr(path_info_start, query_start - path_info_start);
}

std::string MockRequest::getClientIp() const { return ""; };

std::string MockRequest::getHostName() const { return ""; };

std::string MockRequest::getHostPort() const { return ""; };

std::string MockRequest::getAuthority() const { return ""; };

const std::vector<BodyParameter> &MockRequest::getBodyParameters() const
{
    return m_test_body_parameters;
};

bool MockRequest::isUploadRequest() const { return m_test_upload_request; };

RequestState &MockRequest::getState() { return m_state; }
std::vector<char> &MockRequest::getBody() { return m_test_body; }
const std::vector<char> &MockRequest::getBuffer() const
{
    return m_test_buffer;
};

// Setters
void MockRequest::setMethod(const std::string &method)
{
    m_test_method = method;
};

void MockRequest::setUri(const std::string &uri) { m_test_uri = uri; };

void MockRequest::setHttpVersion(const std::string &http_version)
{
    m_test_http_version = http_version;
};

void MockRequest::addHeader(const std::string &key, const std::string &value)
{
    m_test_headers[ key ] = value;
};

void MockRequest::setBody(const std::vector<char> &body)
{
    m_test_body = body;
};

void MockRequest::setBody(const std::string &body)
{
    m_test_body.clear();
    m_test_body.insert(m_test_body.end(), body.begin(), body.end());
};

void MockRequest::addCookie(const std::string &key, const std::string &value)
{
    m_test_headers.insert(std::make_pair(key, value));
};

void MockRequest::setAuthority() {};

void MockRequest::addBodyParameter(const BodyParameter &body_parameter)
{
    m_test_body_parameters.push_back(body_parameter);
};

void MockRequest::setUploadRequest(bool upload_request)
{
    m_test_upload_request = upload_request;
};

void MockRequest::appendBuffer(const std::vector<char> &raw_request)
{
    static_cast<void>(raw_request);
};

// Clear the contents of the MockRequest object
void MockRequest::clear()
{
    m_test_method = "";
    m_test_uri = "";
    m_test_http_version = "";
    m_test_headers.clear();
    m_test_body.clear();
};

// Path: tests/mock_srcs/MockRequest.cpp
