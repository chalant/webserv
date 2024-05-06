#include <cassert>
#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>

// include the header file for the class under test
#include "network/Server.hpp"

// include the header files for the mock classes
#include "MockLogger.hpp"
#include "MockSocket.hpp"
#include "MockPollfdManager.hpp"
#include "MockConnectionManager.hpp"
#include "MockConfiguration.hpp"

int main()
{
    // Mock objects
    MockLogger mockLogger;
    MockSocket mockSocket;
    MockPollfdManager mockPollfdManager;
    MockConnectionManager mockConnectionManager;
    MockConfiguration mockConfiguration;

    // Set test Configuration listen directives


    // Instantiate the Server instance
    Server server(mockSocket, mockErrorLogger);

    // Verify that sockets are up and listening
}

// Path: tests/unit_tests/Server/Server_test_main.cpp
