#include <sys/socket.h>
#include <unistd.h>

// include the header file for the class under test
#include "../../../includes/network/Server.hpp"

// include the header files for the mock classes
#include "../../mock_includes/MockLogger.hpp"
#include "../../mock_includes/MockSocket.hpp"
#include "../../mock_includes/MockPollfdManager.hpp"
#include "../../mock_includes/MockConnectionManager.hpp"
#include "../../mock_includes/MockConfigurationBlock.hpp"

int main()
{
    // Mock objects
    MockLogger mockLogger;
    MockSocket mockSocket;
    MockPollfdManager mockPollfdManager;
    MockConnectionManager mockConnectionManager;
    MockConfigurationBlock mockConfiguration(mockLogger, "configuration");
    MockConfigurationBlock mockEvents(mockLogger, "events");
    MockConfigurationBlock mockHttp(mockLogger, "http");
    MockConfigurationBlock mockServer_1(mockLogger, "server_1");

    // Set test Configuration listen directives
    mockServer_1.setString("listen", "80");
    mockHttp.setBlock("server", &mockServer_1);
    mockHttp.setSize_t("worker_connections", 1024);
    mockConfiguration.setBlock("http", &mockHttp);
    mockEvents.setString("worker_connections", "1024");
    mockConfiguration.setBlock("events", &mockEvents);

    // Instantiate the Server instance
    Server server(mockSocket, mockPollfdManager, mockConnectionManager, mockConfiguration, mockLogger);

    // Verify that sockets are up and listening
}

// Path: tests/unit_tests/Server/Server_test_main.cpp
