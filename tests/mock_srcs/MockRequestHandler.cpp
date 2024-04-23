#include "MockRequestHandler.hpp"

/*
 * MockRequestHandler class is a mock implementation of the IRequestHandler interface
 * This mock implementation is used for testing purposes to simulate the behavior of the IRequestHandler interface
 * during unit testing without relying on the actual implementation.
 */

MockRequestHandler::MockRequestHandler() {}

MockRequestHandler::~MockRequestHandler() {}

int MockRequestHandler::handleRequest(int) { return 0; }

int MockRequestHandler::handlePipeException(int) { return 0; }

int MockRequestHandler::handlePipeRead(int) { return 0; }

int MockRequestHandler::handleErrorResponse(int, int) { return 0; }

int MockRequestHandler::handleErrorResponse(int, HttpStatusCode) { return 0; }

// Path: tests/mock_srcs/MockRequestHandler.cpp
