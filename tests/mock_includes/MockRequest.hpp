#ifndef MOCKREQUEST_HPP
#define MOCKREQUEST_HPP

/*
 * MockRequest class is a mock implementation of the IRequest interface
 * This mock implementation is used for testing purposes to simulate the behavior of the IRequest interface
 * during unit testing without relying on the actual implementation.
*/

#include "../../includes/request/IRequest.hpp" // Include the base interface header

class MockRequest : public IRequest
{
private:
    // Request line components
    std::string _testMethod;
    std::string _testUri;
    std::string _testHttpVersion;

    // Request headers
    std::map<std::string, std::string> _testHeaders;

    // Body parameters
    std::vector<BodyParameter> _testBodyParameters;

    // Upload request flag
    bool _testUploadRequest;

    // Request body
    std::vector<char> _testBody;

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
    virtual std::string getPathInfo(const std::string &scriptName) const;
    virtual std::string getClientIp() const;
    virtual std::string getHostName() const;
    virtual std::string getHostPort() const;
    virtual std::string getAuthority() const;
    virtual const std::vector<BodyParameter> &getBodyParameters() const;
    virtual bool isUploadRequest() const;

    // Setters
    virtual void setMethod(const std::string &method);
    virtual void setUri(const std::string &uri);
    virtual void setHttpVersion(const std::string &httpVersion);
    virtual void addHeader(const std::string &key, const std::string &value);
    virtual void setBody(const std::vector<char> &body);
    virtual void setBody(const std::string &body);
    virtual void addCookie(const std::string &key, const std::string &value);
    virtual void setAuthority();
    virtual void addBodyParameter(const BodyParameter &bodyParameter);
    virtual void setUploadRequest(bool uploadRequest);

    // Clear the contents of the MockRequest object
    void clear();
};

#endif // MOCKREQUEST_HPP