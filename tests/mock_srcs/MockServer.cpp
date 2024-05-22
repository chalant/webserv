#include "../mock_includes/MockServer.hpp"

/*
 * MockServer class is a mock implementation of the IServer interface
 * This mock implementation is used for testing purposes to simulate the
 * behavior of the IServer interface during unit testing without relying on the
 * actual implementation.
 */

MockServer::MockServer() {}

MockServer::~MockServer() {}

void MockServer::acceptConnection(int server_socket_descriptor)
{
    static_cast<void>(server_socket_descriptor);
}

void MockServer::terminate(int exit_code) { static_cast<void>(exit_code); }

// Path: tests/mock_srcs/MockServer.cpp
