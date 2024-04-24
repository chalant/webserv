#include "MockRequest.hpp"

/*
 * MockRequest class is a mock implementation of the IRequest interface
 * This mock implementation is used for testing purposes to simulate the behavior of the IRequest interface
 * during unit testing without relying on the actual implementation.
 */

// Constructor
MockRequest::MockRequest() : IRequest(){};

// Destructor
MockRequest::~MockRequest(){};

// Copy constructor
MockRequest::MockRequest(const MockRequest &src) { static_cast<void>(src); };

// Copy assignment operator
MockRequest &MockRequest::operator=(const MockRequest &src)
{
    static_cast<void>(src);
    return *this;
};

// Getters
HttpMethod MockRequest::getMethod() const { return HttpMethod::GET; };

std::string MockRequest::getMethodString() const { return _testMethod; };

std::string MockRequest::getTestMethod() const { return _testMethod; };

std::string MockRequest::getUri() const { return _testUri; };

HttpVersion MockRequest::getHttpVersion() const { return HttpVersion::HTTP_1_1; };

std::string MockRequest::getTestHttpVersion() const { return _testHttpVersion; };

std::string MockRequest::getHttpVersionString() const { return _testHttpVersion; };

const std::map<HttpHeader, std::string> MockRequest::getHeaders() const { return std::map<HttpHeader, std::string>(); };

std::string MockRequest::getHeaderValue(HttpHeader header) const
{
    static_cast<void>(header);
    return "";
};

std::map<std::string, std::string> MockRequest::getHeadersStringMap() const { return _testHeaders; };

std::map<std::string, std::string> MockRequest::getTestHeadersStringMap() const { return _testHeaders; };

std::map<std::string, std::string> MockRequest::getQueryParameters() const { return std::map<std::string, std::string>(); };

std::map<std::string, std::string> MockRequest::getCookies() const { return std::map<std::string, std::string>(); };

const std::string &MockRequest::getCookie(const std::string &key) const
{
    static_cast<void>(key);
    static const std::string emptyString;
    return emptyString;
};

const std::vector<char> MockRequest::getBody() const { return _testBody; };

std::string MockRequest::getBodyString() const { return std::string(_testBody.begin(), _testBody.end()); };

std::string MockRequest::getClientIp() const { return ""; };

std::string MockRequest::getHostName() const { return ""; };

std::string MockRequest::getHostPort() const { return ""; };

std::string MockRequest::getAuthority() const { return ""; };

// Setters
void MockRequest::setMethod(const std::string &method) { _testMethod = method; };

void MockRequest::setUri(const std::string &uri) { _testUri = uri; };

void MockRequest::setHttpVersion(const std::string &httpVersion) { _testHttpVersion = httpVersion; };

#include <iostream>
void MockRequest::addHeader(const std::string &key, const std::string &value) { 
    _testHeaders[key]=value; };

void MockRequest::setBody(const std::vector<char> &body) { _testBody = body; };

void MockRequest::addCookie(const std::string &key, const std::string &value) { _testHeaders.insert({key, value}); };

void MockRequest::setAuthority() {};

// Clear the contents of the MockRequest object
void MockRequest::clear()
{
    _testMethod = "";
    _testUri = "";
    _testHttpVersion = "";
    _testHeaders.clear();
    _testBody.clear();
};

// Path: tests/mock_srcs/MockRequest.cpp