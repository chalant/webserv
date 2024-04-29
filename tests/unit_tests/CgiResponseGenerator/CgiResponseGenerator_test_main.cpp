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

    // Declare Read-end of the response pipe
    int readPipe;

    // Declare a read buffer
    char readBuffer[1024];

    // Instantiate the CgiResponseGenerator object
    CgiResponseGenerator cgiResponseGenerator(mockLogger);

    // Test case 1: Generate a simple php response
    //********************************************
    // Set the test script name
    std::string scriptName = "hello_world.py";

    // Set the request variables
    mockRequest.setMethod("GET");
    mockRequest.setUri("/mock-cgi-bin/" + scriptName);

    // Generate cgi response
    readPipe = cgiResponseGenerator.generateResponse(mockRoute, mockRequest, mockConfiguration, scriptName);

    // Read the response
    ssize_t bytesRead = read(readPipe, readBuffer, sizeof(readBuffer));
    readBuffer[bytesRead] = '\0'; // Null-terminate the string

    // Close the pipe
    close(readPipe); // Close the read end of the pipe

    // Assert that "Content-Type: text/plain" is present in readBuffer
    assert(strstr(readBuffer, "Content-Type: text/plain") != NULL);

    // Assert that "Hello, world!" is present in readBuffer
    assert(strstr(readBuffer, "Hello, world!") != NULL);

    return 0;
}

// Path: tests/unit_tests/CgiResponseGenerator/CgiResponseGenerator_test_main.cpp