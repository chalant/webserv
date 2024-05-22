#include <cassert>

// include the header file for the class under test
#include "../../../includes/constants/HttpStatusCodeHelper.hpp"
#include "../../../includes/exception/WebservExceptions.hpp"
#include "../../../includes/request/RequestParser.hpp"

#include "../../../includes/configuration/ConfigurationLoader.hpp"

// include the header files for the mock classes
#include "../../mock_includes/MockLogger.hpp"
#include "../../mock_includes/MockRequest.hpp"

// Method to initialize a std::vector<char> from a string
std::vector<char> stringToVector(const std::string &str);

int main()
{
    // Mock objects
    // MockConfigurationBlock mock_configuration_block;
    MockLogger mock_logger;
    ConfigurationLoader conf_loader(mock_logger);

    // mock_configuration_block.setInt("client_header_buffer_size", 6000);

    const IConfiguration &mock_configuration_block =
        conf_loader.loadConfiguration("test_configuration.conf");
    // Request Parser instance
    RequestParser request_parser(mock_configuration_block, mock_logger);

    // Recipient of the Request Parser's output
    MockRequest mock_request;

    // test case 1: A Valid IRequest
    // GET /index.html HTTP/1.1
    // Host: 42.fr
    // content-length: 0
    std::vector<char> raw_request =
        stringToVector("GET /index.html HTTP/1.1\r\nHost: "
                       "42.fr\r\ncontent-length: 0\r\n\r\n");

    // Parse the raw request
    request_parser.parseRequest(raw_request, mock_request);
    // Verify the parsed Request object
    // If the request is invalid, an exception will be thrown
    // otherwise the test will pass
    assert(mock_request.getTestMethod() == "GET");
    assert(mock_request.getUri() == "/index.html");
    assert(mock_request.getTestHttpVersion() == "HTTP/1.1");
    std::map<std::string, std::string> expected_headers;
    expected_headers.insert(std::make_pair("host", "42.fr"));
    expected_headers.insert(std::make_pair("content-length", "0"));
    assert(mock_request.getHeadersStringMap() == expected_headers);
    // Clear the contents of the Request object for the next test
    mock_request.clear();

    // test case 2: An incomplete Request
    // GET
    raw_request = stringToVector("GET");
    try
    {
        // Parse the raw request
        request_parser.parseRequest(raw_request, mock_request);
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
        vec.push_back(str[ i ]);
    }
    return vec;
}
