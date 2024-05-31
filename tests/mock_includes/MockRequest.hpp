#ifndef MOCKREQUEST_HPP
#define MOCKREQUEST_HPP

/*
 * MockRequest class is a mock implementation of the IRequest interface
 * This mock implementation is used for testing purposes to simulate the
 * behavior of the IRequest interface during unit testing without relying on the
 * actual implementation.
 */

#include "../../includes/request/IRequest.hpp" // Include the base interface header
#include "../../includes/request/Request.hpp"

class MockRequest : public IRequest
{
private:
    // Request line components
    std::string m_test_method;
    std::string m_test_uri;
    std::string m_test_http_version;

    // Request headers
    std::map<std::string, std::string> m_test_headers;

    // Body parameters
    std::vector<BodyParameter> m_test_body_parameters;

    // Upload request flag
    bool m_test_upload_request;

    // Request body
    std::vector<char> m_test_body;
    RequestState m_state;

public:
    // Constructor
    MockRequest();

    // Destructor
    virtual ~MockRequest();

    // Copy constructor
    MockRequest(const MockRequest &src);

    // Copy assignment operator
    MockRequest &operator=(const MockRequest &src);

    // MockRequest Specific Methods
    std::string getTestMethod() const;
    std::string getTestHttpVersion() const;
    std::map<std::string, std::string> getTestHeadersStringMap() const;

    // Methods implementing the IRequest interface
    // These methods are overridden to provide custom behavior for testing.

    // Getters
    virtual HttpMethod getMethod() const;
    virtual std::string getMethodString() const;
    virtual std::string getUri() const;
    virtual HttpVersion getHttpVersion() const;
    virtual std::string getHttpVersionString() const;
    virtual const std::map<HttpHeader, std::string> getHeaders() const;
    virtual std::string getHeaderValue(HttpHeader header) const;
    virtual std::map<std::string, std::string> getHeadersStringMap() const;
    virtual std::map<std::string, std::string> getQueryParameters() const;
    virtual std::map<std::string, std::string> getCookies() const;
    virtual std::string getCookie(const std::string &key) const;
    virtual const std::vector<char> getBody() const;
    virtual std::string getBodyString() const;
    virtual std::string getQueryString() const;
    virtual std::string getContentLength() const;
    virtual std::string getContentType() const;
    virtual std::string getPathInfo(const std::string &script_name) const;
    virtual std::string getClientIp() const;
    virtual std::string getHostName() const;
    virtual std::string getHostPort() const;
    virtual std::string getAuthority() const;
    virtual const std::vector<BodyParameter> &getBodyParameters() const;
    virtual bool isUploadRequest() const;
    virtual RequestState &getState(void);
    virtual std::vector<char> &getBody(void);
    virtual const std::vector<char> &getBuffer() const;

    // Setters
    virtual void setMethod(const std::string &method);
    virtual void setUri(const std::string &uri);
    virtual void setHttpVersion(const std::string &http_version);
    virtual void addHeader(const std::string &key, const std::string &value);
    virtual void setBody(const std::vector<char> &body);
    virtual void setBody(const std::string &body);
    virtual void addCookie(const std::string &key, const std::string &value);
    virtual void setAuthority();
    virtual void addBodyParameter(const BodyParameter &body_parameter);
    virtual void setUploadRequest(bool upload_request);
    virtual void appendBuffer(const std::vector<char> &raw_request);

    // Clear the contents of the MockRequest object
    void clear();
};

#endif // MOCKREQUEST_HPP
