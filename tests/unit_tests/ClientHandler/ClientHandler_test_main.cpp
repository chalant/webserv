#include <cassert>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <sstream>

// include the header file for the class under test
#include "../../../includes/connection/ClientHandler.hpp"

// include the header files for the mock classes
#include "../../mock_includes/MockLogger.hpp"
#include "../../mock_includes/MockSocket.hpp"

int main()
{
    // Mock objects
    MockLogger mockErrorLogger;
    MockSocket mockSocket;

    // Instantiate the ClientHandler class
    ClientHandler clientHandler(mockSocket, mockErrorLogger);

    // Instantiate test variables
    int MOCK_CLIENT_SOCKET_DESCRIPTOR = 44998;
    std::vector<char> mockMessageVector;
    std::vector<char> readVector;
    std::string mockMessageString;
    std::string readString;
    size_t BODY_SIZE;
    std::string body;

    // Test case 1: Read A simple request
    //***********************************
    // Set a Mock client socket descriptor
    MOCK_CLIENT_SOCKET_DESCRIPTOR = 44998;

    // Assign the socket to the ClientHandler
    clientHandler.setSocketDescriptor(MOCK_CLIENT_SOCKET_DESCRIPTOR);

    // Make a Mock Request
    BODY_SIZE = 0;
    body.assign(BODY_SIZE, 'x');              // Creating a string of 4096 'x' characters
    mockMessageString = "GET / HTTP/1.1\r\n"; // Request line
    mockMessageString += "content-type: text/plain\r\n";
    std::ostringstream bodySizeStream;
    bodySizeStream << body.size();                                                                                                                                              // content-type header
    mockMessageString += "content-length: " + bodySizeStream.str() + "\r\n";                                                                                                    // content-length header
    mockMessageString += "Host: 42.fr\r\n";                                                                                                                                     // Mandatory Host header
    mockMessageString += "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.99 Safari/537.36\r\n";                  // Mandatory User-Agent header
    mockMessageString += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"; // Mandatory Accept header
    mockMessageString += "Accept-Language: en-US,en;q=0.9\r\n";                                                                                                                 // Mandatory Accept-Language header
    mockMessageString += "Accept-Encoding: gzip, deflate, br\r\n";                                                                                                              // Mandatory Accept-Encoding header
    mockMessageString += "\r\n";                                                                                                                                                // Empty line before the body
    mockMessageString += body;
    // Request body
    // Convert the string to a vector
    mockMessageVector.assign(mockMessageString.begin(), mockMessageString.end());

    // Send to the socket
    mockSocket.send(MOCK_CLIENT_SOCKET_DESCRIPTOR, mockMessageVector);

    // Read from the socket
    readVector = clientHandler.readRequest();

    // Convert the vector to a string
    readString.assign(readVector.begin(), readVector.end());

    // Verify the client request
    assert(readString == mockMessageString);

    // Test case 2: Send and Read a large request requiring multiple reads and buffer resizing
    //****************************************************************************************
    // Set a Mock client socket descriptor
    MOCK_CLIENT_SOCKET_DESCRIPTOR = 44998;

    // Assign the socket to the ClientHandler
    clientHandler.setSocketDescriptor(MOCK_CLIENT_SOCKET_DESCRIPTOR);

    // Make a Mock Request
    BODY_SIZE = 5000;
    body.assign(BODY_SIZE, 'x');              // Creating a string of 4096 'x' characters
    mockMessageString = "GET / HTTP/1.1\r\n"; // Request line
    mockMessageString += "content-type: text/plain\r\n";
    bodySizeStream.clear();
    bodySizeStream << body.size();                                                                                                                                              // content-type header
    mockMessageString += "content-length: " + bodySizeStream.str() + "\r\n";                                                                                                    // content-length header
    mockMessageString += "Host: 42.fr\r\n";                                                                                                                                     // Mandatory Host header
    mockMessageString += "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.99 Safari/537.36\r\n";                  // Mandatory User-Agent header
    mockMessageString += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"; // Mandatory Accept header
    mockMessageString += "Accept-Language: en-US,en;q=0.9\r\n";                                                                                                                 // Mandatory Accept-Language header
    mockMessageString += "Accept-Encoding: gzip, deflate, br\r\n";                                                                                                              // Mandatory Accept-Encoding header
    mockMessageString += "\r\n";                                                                                                                                                // Empty line before the body
    mockMessageString += body;                                                                                                                                                  // Request body

    // Convert the string to a vector
    mockMessageVector.assign(mockMessageString.begin(), mockMessageString.end());

    // Send to the socket
    clientHandler.sendResponse(mockMessageVector);

    // Read from the socket
    readVector = clientHandler.readRequest();

    // Convert the vector to a string
    readString.assign(readVector.begin(), readVector.end());

    // Verify the client request
    assert(readString == mockMessageString);
}

// Path: tests/unit_tests/ClientHandler/ClientHandler_test_main.cpp