#include <cassert>
#include <cstring>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

// include the header file for the class under test
#include "../../../includes/connection/ClientHandler.hpp"

// include the header files for the mock classes
#include "../../mock_includes/MockLogger.hpp"
#include "../../mock_includes/MockSocket.hpp"

int main()
{
    // Mock objects
    MockLogger mock_logger;
    MockSocket mock_socket;

    // Instantiate the ClientHandler class
    ClientHandler client_handler(mock_socket, mock_logger);

    // Instantiate test variables
    int mock_client_socket_descriptor = 44998;
    std::vector<char> mock_message_vector;
    std::vector<char> read_vector;
    std::string mock_message_string;
    std::string read_string;
    size_t body_size;
    std::string body;

    // Test case 1: Read A simple request
    //***********************************
    // Set a Mock client socket descriptor
    mock_client_socket_descriptor = 44998;

    // Assign the socket to the ClientHandler
    client_handler.setSocketDescriptor(mock_client_socket_descriptor);

    // Make a Mock Request
    body_size = 0;
    body.assign(body_size, 'x'); // Creating a string of 4096 'x' characters
    mock_message_string = "GET / HTTP/1.1\r\n"; // Request line
    mock_message_string += "content-type: text/plain\r\n";
    std::ostringstream body_size_stream;
    body_size_stream << body.size(); // content-type header
    mock_message_string += "content-length: " + body_size_stream.str() +
                         "\r\n";            // content-length header
    mock_message_string += "Host: 42.fr\r\n"; // Mandatory Host header
    mock_message_string +=
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
        "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.99 "
        "Safari/537.36\r\n"; // Mandatory User-Agent header
    mock_message_string +=
        "Accept: "
        "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
        "avif,image/webp,image/apng,*/*;q=0.8,application/"
        "signed-exchange;v=b3;q=0.9\r\n"; // Mandatory Accept header
    mock_message_string +=
        "Accept-Language: en-US,en;q=0.9\r\n"; // Mandatory Accept-Language
                                               // header
    mock_message_string +=
        "Accept-Encoding: gzip, deflate, br\r\n"; // Mandatory
                                                  // Accept-Encoding header
    mock_message_string += "\r\n";                  // Empty line before the body
    mock_message_string += body;
    // Request body
    // Convert the string to a vector
    mock_message_vector.assign(mock_message_string.begin(),
                             mock_message_string.end());

    // Send to the socket
    mock_socket.send(mock_client_socket_descriptor, mock_message_vector);

    // Read from the socket
    read_vector = client_handler.readRequest();

    // Convert the vector to a string
    read_string.assign(read_vector.begin(), read_vector.end());

    // Verify the client request
    assert(read_string == mock_message_string);

    // Test case 2: Send and Read a large request requiring multiple reads and
    // buffer resizing
    //****************************************************************************************
    // Set a Mock client socket descriptor
    mock_client_socket_descriptor = 44998;

    // Assign the socket to the ClientHandler
    client_handler.setSocketDescriptor(mock_client_socket_descriptor);

    // Make a Mock Request
    body_size = 5000;
    body.assign(body_size, 'x'); // Creating a string of 4096 'x' characters
    mock_message_string = "GET / HTTP/1.1\r\n"; // Request line
    mock_message_string += "content-type: text/plain\r\n";
    body_size_stream.clear();
    body_size_stream << body.size(); // content-type header
    mock_message_string += "content-length: " + body_size_stream.str() +
                         "\r\n";            // content-length header
    mock_message_string += "Host: 42.fr\r\n"; // Mandatory Host header
    mock_message_string +=
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
        "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.99 "
        "Safari/537.36\r\n"; // Mandatory User-Agent header
    mock_message_string +=
        "Accept: "
        "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
        "avif,image/webp,image/apng,*/*;q=0.8,application/"
        "signed-exchange;v=b3;q=0.9\r\n"; // Mandatory Accept header
    mock_message_string +=
        "Accept-Language: en-US,en;q=0.9\r\n"; // Mandatory Accept-Language
                                               // header
    mock_message_string +=
        "Accept-Encoding: gzip, deflate, br\r\n"; // Mandatory
                                                  // Accept-Encoding header
    mock_message_string += "\r\n";                  // Empty line before the body
    mock_message_string += body;                    // Request body

    // Convert the string to a vector
    mock_message_vector.assign(mock_message_string.begin(),
                             mock_message_string.end());

    // Send to the socket
    client_handler.sendResponse(mock_message_vector);

    // Read from the socket
    read_vector = client_handler.readRequest();

    // Convert the vector to a string
    read_string.assign(read_vector.begin(), read_vector.end());

    // Verify the client request
    assert(read_string == mock_message_string);
}

// Path: tests/unit_tests/ClientHandler/ClientHandler_test_main.cpp
