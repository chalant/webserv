#include "../mock_includes/MockExceptionHandler.hpp"

/*
 * MockExceptionHandler class is a mock implementation of the IExceptionHandler
 * interface This mock implementation is used for testing purposes to simulate
 * the behavior of the IExceptionHandler interface during unit testing without
 * relying on the actual implementation.
 */

// Constructor
MockExceptionHandler::MockExceptionHandler() : IExceptionHandler() {};

// Destructor
MockExceptionHandler::~MockExceptionHandler() {};

// Method to handle exceptions
int MockExceptionHandler::handleException(const std::exception &e,
                                          const std::string &context) const
{
    static_cast<void>(e);
    static_cast<void>(context);
    return 0;
};

// Path: tests/mock_includes/MockLogger.hpp
