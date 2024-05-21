#include <cassert>
#include <sys/socket.h>
#include <unistd.h>

// include the header file for the class under test
#include "../../../includes/response/StaticFileResponseGenerator.hpp"

#include "../../../includes/configuration/ConfigurationLoader.hpp"

// include the header files for the mock classes
#include "../../mock_includes/MockLogger.hpp"
#include "../../mock_includes/MockRequest.hpp"
#include "../../mock_includes/MockResponse.hpp"
#include "../../mock_includes/MockRoute.hpp"

int main()
{
    // Mock objects
    MockRoute mockRoute;
    MockRequest mockRequest;
    MockResponse mockResponse;
    MockLogger mockLogger;
    ConfigurationLoader conf_loader(mockLogger);

    // Set Mock Route Variables
    std::string root = "../../mock_files";
    std::string prefix = "/";
    mockRoute.setRoot(root);
    mockRoute.setPrefix(prefix);

    const IConfiguration &mockConfigurationBlock =
        conf_loader.loadConfiguration("test_configuration.conf");

    // Instantiate the staticFileResponseGenerator object
    StaticFileResponseGenerator staticFileResponseGenerator(mockLogger);

    // Test case 1: Request a non-existent file
    //***************************************************
    // Set the request variables
    mockRequest.setMethod("GET");
    mockRequest.setUri("non_existant_file");

    // Generate static file response
    Triplet_t response = staticFileResponseGenerator.generateResponse(
        mockRoute, mockRequest, mockResponse, mockConfigurationBlock);

    // Check the response status code
    assert(response.first == -1);

    // Check the response body
    assert(mockResponse.getStatusLine() == "HTTP/1.1 404 Not Found\r\n");

    // Test case 2: Request the mock file MockFile1.txt
    //******************************************************************
    // Set the request variables
    mockRequest.setMethod("GET");
    mockRequest.setUri("MockFile1.txt");

    // Generate static file response
    response = staticFileResponseGenerator.generateResponse(
        mockRoute, mockRequest, mockResponse, mockConfigurationBlock);

    // Check the response status code
    assert(response.first == -1);

    // Verify that the content of the file was added to the response body
    assert(mockResponse.getBody() == "This is the content of MockFile1.txt\n");

    return 0;
}

// Path:
// tests/unit_tests/CgiResponseGenerator/CgiResponseGenerator_test_main.cpp
