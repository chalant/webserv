#include "../mock_includes/MockRequestHandler.hpp"

/*
 * MockRequestHandler class is a mock implementation of the IRequestHandler
 * interface This mock implementation is used for testing purposes to simulate
 * the behavior of the IRequestHandler interface during unit testing without
 * relying on the actual implementation.
 */

MockRequestHandler::MockRequestHandler() {}

MockRequestHandler::~MockRequestHandler() {}

Triplet_t MockRequestHandler::handleRequest(int) { return Triplet_t(); }

int MockRequestHandler::handlePipeException(int) { return 0; }

int MockRequestHandler::handlePipeRead(int) { return 0; }

void MockRequestHandler::handleErrorResponse(int, int) { return; }

void MockRequestHandler::handleErrorResponse(int, HttpStatusCode) { return; }

void MockRequestHandler::removeConnection(int) { return; }

// Path: tests/mock_srcs/MockRequestHandler.cpp
