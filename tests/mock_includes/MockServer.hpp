#ifndef MOCKSERVER_HPP
#define MOCKSERVER_HPP

/*
 * MockServer class is a mock implementation of the IServer interface
 * This mock implementation is used for testing purposes to simulate the behavior of the IServer interface
 * during unit testing without relying on the actual implementation.
 */

#include "../../includes/network/IServer.hpp" // Include the base interface header

class MockServer : public IServer
{
public:
    MockServer();

    virtual ~MockServer();
    virtual void acceptConnection(int serverSocketDescriptor);
    virtual void terminate(int exitCode);
};

#endif // MOCKSERVER_HPP
// Path: tests/mock_includes/MockServer.hpp
