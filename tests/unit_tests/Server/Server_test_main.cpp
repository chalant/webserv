#include <sys/socket.h>
#include <unistd.h>

// include the header file for the class under test
#include "../../../includes/network/Server.hpp"

// include the header files for the mock classes
#include "../../mock_includes/MockConfigurationBlock.hpp"
#include "../../mock_includes/MockConnectionManager.hpp"
#include "../../mock_includes/MockLogger.hpp"
#include "../../mock_includes/MockPollfdManager.hpp"
#include "../../mock_includes/MockSocket.hpp"

int main()
{
    // Mock objects
    MockLogger mock_logger;
    MockSocket mock_socket;
    MockPollfdManager mock_pollfd_manager;
    MockConnectionManager mock_connecption_manager;
    MockConfigurationBlock mock_configuration(mock_logger, "configuration");
    MockConfigurationBlock mock_events(mock_logger, "events");
    MockConfigurationBlock mock_http(mock_logger, "http");
    MockConfigurationBlock mock_server_1(mock_logger, "server_1");

    // Set test Configuration listen directives
    mock_server_1.setString("listen", "80");
    mock_http.setBlock("server", &mock_server_1);
    mock_http.setSize_t("worker_connections", 1024);
    mock_configuration.setBlock("http", &mock_http);
    mock_events.setString("worker_connections", "1024");
    mock_configuration.setBlock("events", &mock_events);

    // Instantiate the Server instance
    Server server(mock_socket, mock_pollfd_manager, mock_connection_manager,
                  mock_configuration, mock_logger);

    // Verify that sockets are up and listening
}

// Path: tests/unit_tests/Server/Server_test_main.cpp
