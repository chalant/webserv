#include <cassert>
#include <iostream>
// include the header file for the class under test
#include "../../includes/RequestParser.hpp"
// include the header files for the mock class
#include "../../mock_includes/MockConfiguration.hpp"
#include "../../mock_includes/MockLogger.hpp"
#include "../../mock_includes/MockRequest.hpp"
#include "../../mock_includes/MockExceptionHandler.hpp"

int main()
{
    // Mock objects
    MockConfiguration mockConfiguration;
    MockLogger mockErrorLogger;
    MockExceptionHandler mockExceptionHandler;

    // Request Parser instance
    RequestParser requestParser(mockConfiguration, mockErrorLogger, mockExceptionHandler);

    // Recipient of the Request Parser's output
    MockRequest mockRequest;

    // test case 1: A Valid IRequest
    // GET /index.html HTTP/1.1
    // Host: example.com
    std::vector<char> rawRequest = {
        'G', 'E', 'T', ' ', '/', 'i', 'n', 'd', 'e', 'x', '.', 'h', 't', 'm', 'l', ' ', 'H', 'T', 'T', 'P', '/', '1', '.', '1', '\r', '\n',
        'H', 'o', 's', 't', ':', ' ', 'e', 'x', 'a', 'm', 'p', 'l', 'e', '.', 'c', 'o', 'm', '\r', '\n',
        '\r', '\n'};
    // Parse the raw request
    requestParser.parseRequest(rawRequest, mockRequest);
    // Verify the parsed Request object
    // If the request is invalid, an exception will be thrown
    // otherwise the test will pass
    assert(mockRequest.getTestMethod() == "GET");
    assert(mockRequest.getUri() == "/index.html");
    assert(mockRequest.getTestHttpVersion() == "HTTP/1.1");
    std::map<std::string, std::string> expectedHeaders = {{"Host", "example.com"}};
    assert(mockRequest.getHeaders() == expectedHeaders);
    // Clear the contents of the Request object for the next test
    mockRequest.clear();

    // test case 2: An incomplete Request
    // GET
    rawRequest = {'G', 'E', 'T'};
    try
    {
        // Parse the raw request
        requestParser.parseRequest(rawRequest, mockRequest);
        // If the request is invalid, an exception will be thrown
        // otherwise the test will fail
        assert(false);
    }
    catch (HttpStatusCodeException &e)
    {
        // Verify the exception thrown
        assert(e.getErrorCode() == BAD_REQUEST);
    }

    return 0;
}