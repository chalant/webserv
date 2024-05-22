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
    MockRoute mock_route;
    MockRequest mock_request;
    MockResponse mock_response;
    MockLogger mock_logger;
    ConfigurationLoader conf_loader(mock_logger);

    // Set Mock Route Variables
    std::string root = "../../mock_files";
    std::string prefix = "/";
    mock_route.setRoot(root);
    mock_route.setPath(prefix);

    const IConfiguration &mock_configuration_block =
        conf_loader.loadConfiguration("test_configuration.conf");

    // Instantiate the static_file_response_generator object
    StaticFileResponseGenerator static_file_response_generator(mock_logger);

    // Test case 1: Request a non-existent file
    //***************************************************
    // Set the request variables
    mock_request.setMethod("GET");
    mock_request.setUri("non_existant_file");

    // Generate static file response
    Triplet_t response = static_file_response_generator.generateResponse(
        mock_route, mock_request, mock_response, mock_configuration_block);

    // Check the response status code
    assert(response.first == -1);

    // Check the response body
    assert(mock_response.getStatusLine() == "HTTP/1.1 404 Not Found\r\n");

    // Test case 2: Request the mock file MockFile1.txt
    //******************************************************************
    // Set the request variables
    mock_request.setMethod("GET");
    mock_request.setUri("MockFile1.txt");

    // Generate static file response
    response = static_file_response_generator.generateResponse(
        mock_route, mock_request, mock_response, mock_configuration_block);

    // Check the response status code
    assert(response.first == -1);

    // Verify that the content of the file was added to the response body
    assert(mock_response.getBodyString() ==
           "This is the content of MockFile1.txt");

    return 0;
}

// Path:
// tests/unit_tests/CgiResponseGenerator/CgiResponseGenerator_test_main.cpp
