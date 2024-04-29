#include "MockServer.hpp"

/*
 * MockServer class is a mock implementation of the IServer interface
 * This mock implementation is used for testing purposes to simulate the behavior of the IServer interface
 * during unit testing without relying on the actual implementation.
 */

MockServer::MockServer() {}

MockServer::~MockServer() {}

void MockServer::acceptConnection(int serverSocketDescriptor) { static_cast<void>(serverSocketDescriptor); }

void MockServer::terminate(int exitCode) { static_cast<void>(exitCode); }

// Path: tests/mock_srcs/MockServer.cpp
