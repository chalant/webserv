#ifndef MOCKCLIENTHANDLER_HPP
#define MOCKCLIENTHANDLER_HPP

/*
 * MockClientHandler class is a mock implementation of the IClientHandler interface
 * This mock implementation is used for testing purposes to simulate the behavior of the IClientHandler interface
 * during unit testing without relying on the actual implementation.
 */

#include "connection/IClientHandler.hpp" // Include the base interface header

class MockClientHandler : public IClientHandler
{
public:
    // Constructor
    MockClientHandler();

    // Destructor
    virtual ~MockClientHandler();

    // Methods implementing the IClientHandler interface
    // These methods are overridden to provide custom behavior for testing.

    // Setter
    virtual void setSocketDescriptor(int socketDescriptor);

    // Methods to read and send requests and responses
    virtual const std::vector<char> readRequest() const;
    virtual ssize_t sendResponse(const std::vector<char> &response) const;
    virtual ssize_t sendResponse(const std::string &response) const;
};

#endif // MOCKCLIENTHANDLER_HPP
// Path: tests/mock_includes/MockClientHandler.hpp
