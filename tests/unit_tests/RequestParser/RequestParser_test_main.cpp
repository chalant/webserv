#include <cassert>

// include the header file for the class under test
#include "../../../includes/request/RequestParser.hpp"
#include "../../../includes/exception/WebservExceptions.hpp"
#include "../../../includes/constants/HttpStatusCodeHelper.hpp"

#include "../../../includes/configuration/ConfigurationLoader.hpp"

// include the header files for the mock classes
// #include "MockConfigurationBlock.hpp"
#include "../../mock_includes/MockLogger.hpp"
#include "../../mock_includes/MockRequest.hpp"

// Method to initialize a std::vector<char> from a string
std::vector<char> stringToVector(const std::string &str);

int main()
{
    // Mock objects
    //MockConfigurationBlock mockConfigurationBlock;
    MockLogger mockErrorLogger;
	ConfigurationLoader	confLoader(mockErrorLogger);

    // mockConfigurationBlock.setInt("ClientHeaderBufferSize", 6000);

	const IConfiguration&	mockConfigurationBlock = confLoader.loadConfiguration("test_configuration.conf");
    // Request Parser instance
    RequestParser requestParser(mockConfigurationBlock, mockErrorLogger);

    // Recipient of the Request Parser's output
    MockRequest mockRequest;

    // test case 1: A Valid IRequest
    // GET /index.html HTTP/1.1
    // Host: 42.fr
    // content-length: 0
    std::vector<char> rawRequest = stringToVector("GET /index.html HTTP/1.1\r\nHost: 42.fr\r\ncontent-length: 0\r\nblabla: yo\r\n\r\n");

    // Parse the raw request
    requestParser.parseRequest(rawRequest, mockRequest);
    // Verify the parsed Request object
    // If the request is invalid, an exception will be thrown
    // otherwise the test will pass
    assert(mockRequest.getTestMethod() == "GET");
    assert(mockRequest.getUri() == "/index.html");
    assert(mockRequest.getTestHttpVersion() == "HTTP/1.1");
    std::map<std::string, std::string> expectedHeaders;
    expectedHeaders.insert(std::make_pair("host", "42.fr"));
    expectedHeaders.insert(std::make_pair("content-length", "0"));
    //assert(mockRequest.getHeadersStringMap() == expectedHeaders);
    // Clear the contents of the Request object for the next test
    mockRequest.clear();

    // test case 2: An incomplete Request
    // GET
    rawRequest = stringToVector("GET");
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

// Method to initialize a std::vector<char> from a string
std::vector<char> stringToVector(const std::string &str)
{
    std::vector<char> vec;
    for (size_t i = 0; i < str.size(); i++)
    {
        vec.push_back(str[i]);
    }
    return vec;
}
