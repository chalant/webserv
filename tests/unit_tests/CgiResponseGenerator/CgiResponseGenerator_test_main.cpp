#include <cassert>
#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>

// include the header file for the class under test
#include "response/CgiResponseGenerator.hpp"

// include the header files for the mock classes
#include "MockRoute.hpp"
#include "MockRequest.hpp"
#include "MockConfiguration.hpp"
#include "MockLogger.hpp"

int main()
{
    // Mock objects
    MockRoute mockRoute;
    MockRequest mockRequest;
    MockConfiguration mockConfiguration;
    MockLogger mockLogger;

    // Set Mock Route Variables
    std::string root = ".";
    std::string prefix = "/mock-cgi-bin";
    mockRoute.setRoot(root);
    mockRoute.setPrefix(prefix);

    // Set Mock Configuration Variables
    mockConfiguration.setString("PhpCgiPath", "/usr/bin/php-cgi");
    mockConfiguration.setString("PythonCgiPath", "/usr/bin/python3");

    // Declare cgi info
    Triplet_t cgiInfo; // <cgiPid, <responseReadPipe, requestWritePipe>>
    int responseReadPipe;
    int requestWritePipe;

    // Declare a read buffer
    char readBuffer[1024];

    // Instantiate the CgiResponseGenerator object
    CgiResponseGenerator cgiResponseGenerator(mockLogger);

    // Test case 1: Generate a simple cgi python response
    //***************************************************
    // Set the test script name
    std::string scriptName = "hello_world.py";

    // Set the request variables
    mockRequest.setMethod("GET");
    mockRequest.setUri("/mock-cgi-bin/" + scriptName);

    // Generate cgi response
    cgiInfo = cgiResponseGenerator.generateResponse(mockRoute, mockRequest, mockConfiguration, scriptName);
    
    // Get the read pipe file descriptor
    responseReadPipe = cgiInfo.second.first;

    // Read the response
    ssize_t bytesRead;
    while ((bytesRead = read(responseReadPipe, readBuffer, sizeof(readBuffer))) <= 0); // wait for the response
    readBuffer[bytesRead] = '\0'; // Null-terminate the string

    // Close the pipe
    close(responseReadPipe); // Close the read end of the pipe

    // Assert that "Content-Type: text/plain" is present in readBuffer
    assert(strstr(readBuffer, "Content-Type: text/plain") != NULL);

    // Assert that "Hello, world!" is present in readBuffer
    assert(strstr(readBuffer, "Hello, world!") != NULL);

    // Test case 2: Generate a cgi python response with query parameters
    //******************************************************************
    // Set the test script name
    scriptName = "query_params_python.py";

    // Set the request variables
    mockRequest.setMethod("GET");
    mockRequest.setUri("/mock-cgi-bin/" + scriptName + "?name=John&age=30");

    // Generate cgi response
    cgiInfo = cgiResponseGenerator.generateResponse(mockRoute, mockRequest, mockConfiguration, scriptName);
    
    // Get the read pipe file descriptor
    responseReadPipe = cgiInfo.second.first;

    // Read the response
    while ((bytesRead = read(responseReadPipe, readBuffer, sizeof(readBuffer))) <= 0); // wait for the response
    readBuffer[bytesRead] = '\0'; // Null-terminate the string

    // Close the pipe
    close(responseReadPipe); // Close the read end of the pipe

    // Assert that age param is present in readBuffer
    assert(strstr(readBuffer, "age = 30") != NULL);

    // Assert that name param is present in readBuffer
    assert(strstr(readBuffer, "name = John") != NULL);

    // Test case 3: Generate a cgi python response with POST method
    //*************************************************************
    // Set the test script name
    scriptName = "post_python.py";

    // Set the request variables
    mockRequest.setMethod("POST");
    mockRequest.setUri("/mock-cgi-bin/" + scriptName + "?name=John&age=30");
    mockRequest.setBody("Hello, world! (from the body)");
    mockRequest.addHeader("Content-Length", std::to_string(mockRequest.getBody().size()));

    // Generate cgi response
    cgiInfo = cgiResponseGenerator.generateResponse(mockRoute, mockRequest, mockConfiguration, scriptName);
    
    // Get the pipe file descriptors
    responseReadPipe = cgiInfo.second.first;
    requestWritePipe = cgiInfo.second.second;

    // Write the request body to the pipe
    write(requestWritePipe, mockRequest.getBody().data(), mockRequest.getBody().size());

    // Read the response
    while ((bytesRead = read(responseReadPipe, readBuffer, sizeof(readBuffer))) <= 0); // wait for the response
    readBuffer[bytesRead] = '\0'; // Null-terminate the string

    // Close the pipe
    close(responseReadPipe); // Close the read end of the pipe

    // Assert that the line "POST Data:\nHello, world! (from the body)" is present in readBuffer
    assert(strstr(readBuffer, "POST Data:\nHello, world! (from the body)") != NULL);

    return 0;
}

// Path: tests/unit_tests/CgiResponseGenerator/CgiResponseGenerator_test_main.cpp