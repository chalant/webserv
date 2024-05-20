#include "../mock_includes/MockClientHandler.hpp"

/*
 * MockClientHandler class is a mock implementation of the IClientHandler
 * interface This mock implementation is used for testing purposes to simulate
 * the behavior of the IClientHandler interface during unit testing without
 * relying on the actual implementation.
 */

// Constructor
MockClientHandler::MockClientHandler() : IClientHandler() {};

// Destructor
MockClientHandler::~MockClientHandler() {};

// Setter
void MockClientHandler::setSocketDescriptor(int socketDescriptor)
{
    static_cast<void>(socketDescriptor);
};

// Method to read and send requests and responses
const std::vector<char> MockClientHandler::readRequest() const
{
    return std::vector<char>();
};

ssize_t MockClientHandler::sendResponse(const std::vector<char> &response) const
{
    static_cast<void>(response);
    return 0;
};

ssize_t MockClientHandler::sendResponse(const std::string &response) const
{
    static_cast<void>(response);
    return 0;
};

// Path: tests/mock_includes/MockClientHandler.hpp
