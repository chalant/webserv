#ifndef MOCKEXCEPTIONHANDLER_HPP
#define MOCKEXCEPTIONHANDLER_HPP

/*
 * MockExceptionHandler class is a mock implementation of the IExceptionHandler interface
 * This mock implementation is used for testing purposes to simulate the behavior of the IExceptionHandler interface
 * during unit testing without relying on the actual implementation.
 */

#include "exception/IExceptionHandler.hpp"

class MockExceptionHandler : public IExceptionHandler
{
public:
    // Constructor
    MockExceptionHandler();

    // Destructor
    ~MockExceptionHandler();

    // Method implementing the IExceptionHandler interface
    // This method is overridden to provide custom behavior for testing.
    // Method to handle exceptions
    int handleException(const std::exception &e, const std::string &context = "") const;
};

#endif // MOCKEXCEPTIONHANDLER_HPP
// Path: tests/mock_includes/MockConfiguration.hpp
