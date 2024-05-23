#include <cassert>
#include <cstring>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

// include the header file for the class under test
#include "../../../includes/response/CgiResponseGenerator.hpp"

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
    std::string root = ".";
    std::string prefix = "/mock-cgi-bin";
    mock_route.setRoot(root);
    mock_route.setPath(prefix);

    IConfiguration &mock_configuration_block =
        conf_loader.loadConfiguration("test_configuration.conf");

    // Set Mock Configuration Variables
    // mock_configuration_block.setString("PhpCgiPath", "/usr/bin/php-cgi");
    // mock_configuration_block.setString("PythonCgiPath", "/usr/bin/python3");

    // Declare cgi info
    Triplet_t cgi_info; // <cgi_pid, <response_read_pipe, request_write_pipe> >
    int response_read_pipe;
    int request_write_pipe;

    // Declare a read buffer
    char read_buffer[ 1024 ];

    // Instantiate the CgiResponseGenerator object
    CgiResponseGenerator cgi_response_generator(mock_logger);

    // Test case 1: Generate a simple cgi python response
    //***************************************************
    // Set the test script name
    std::string script_name = "hello_world.py";

    // Set the request variables
    mock_request.setMethod("GET");
    mock_request.setUri("/mock-cgi-bin/" + script_name);

    // Generate cgi response
    cgi_info = cgi_response_generator.generateResponse(
        mock_route, mock_request, mock_response, mock_configuration_block,
        script_name);

    // Get the read pipe file descriptor
    response_read_pipe = cgi_info.second.first;

    // Read the response
    ssize_t bytes_read;
    while ((bytes_read =
                read(response_read_pipe, read_buffer, sizeof(read_buffer))) <= 0)
        ;                           // wait for the response
    read_buffer[ bytes_read ] = '\0'; // Null-terminate the string

    // Close the pipe
    close(response_read_pipe); // Close the read end of the pipe

    // Assert that "content-type: text/plain" is present in read_buffer
    assert(strstr(read_buffer, "content-type: text/plain") != NULL);

    // Assert that "Hello, world!" is present in read_buffer
    assert(strstr(read_buffer, "Hello, world!") != NULL);

    // Test case 2: Generate a cgi python response with query parameters
    //******************************************************************
    // Set the test script name
    script_name = "query_params_python.py";

    // Set the request variables
    mock_request.setMethod("GET");
    mock_request.setUri("/mock-cgi-bin/" + script_name + "?name=John&age=30");

    // Generate cgi response
    cgi_info = cgi_response_generator.generateResponse(
        mock_route, mock_request, mock_response, mock_configuration_block,
        script_name);

    // Get the read pipe file descriptor
    response_read_pipe = cgi_info.second.first;

    // Read the response
    while ((bytes_read =
                read(response_read_pipe, read_buffer, sizeof(read_buffer))) <= 0)
        ;                           // wait for the response
    read_buffer[ bytes_read ] = '\0'; // Null-terminate the string

    // Close the pipe
    close(response_read_pipe); // Close the read end of the pipe

    // Assert that age param is present in read_buffer
    assert(strstr(read_buffer, "age = 30") != NULL);

    // Assert that name param is present in read_buffer
    assert(strstr(read_buffer, "name = John") != NULL);

// Test case 3: Generate a cgi python response with POST method
//*************************************************************
// Set the test script name
#include <sstream>

    script_name = "post_python.py";

    // Set the request variables
    mock_request.setMethod("POST");
    mock_request.setUri("/mock-cgi-bin/" + script_name + "?name=John&age=30");
    mock_request.setBody("Hello, world! (from the body)");
    std::ostringstream body_size_stream;
    body_size_stream << mock_request.getBody().size();
    mock_request.addHeader("content-length", body_size_stream.str());

    // Generate cgi response
    cgi_info = cgi_response_generator.generateResponse(
        mock_route, mock_request, mock_response, mock_configuration_block,
        script_name);

    // Get the pipe file descriptors
    response_read_pipe = cgi_info.second.first;
    request_write_pipe = cgi_info.second.second;

    // Write the request body to the pipe
    write(request_write_pipe, mock_request.getBody().data(),
          mock_request.getBody().size());

    // Read the response
    while ((bytes_read =
                read(response_read_pipe, read_buffer, sizeof(read_buffer))) <= 0)
        ;                           // wait for the response
    read_buffer[ bytes_read ] = '\0'; // Null-terminate the string

    // Close the pipe
    close(response_read_pipe); // Close the read end of the pipe

    // Assert that the line "POST Data:\nHello, world! (from the body)" is
    // present in read_buffer
    assert(strstr(read_buffer, "POST Data:\nHello, world! (from the body)") !=
           NULL);

    return 0;
}

// Path:
// tests/unit_tests/CgiResponseGenerator/CgiResponseGenerator_test_main.cpp
