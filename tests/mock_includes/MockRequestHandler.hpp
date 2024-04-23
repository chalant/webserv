#ifndef MOCKREQUESTHANDLER_HPP
#define MOCKREQUESTHANDLER_HPP

/*
 * MockRequestHandler class is a mock implementation of the IRequestHandler interface
 * This mock implementation is used for testing purposes to simulate the behavior of the IRequestHandler interface
 * during unit testing without relying on the actual implementation.
 */

#include "session/IRequestHandler.hpp" // Include the base interface header

class MockRequestHandler : public IRequestHandler
{
public:
    MockRequestHandler();

    virtual ~MockRequestHandler();
    virtual int handleRequest(int);
    virtual int handlePipeException(int);
    virtual int handlePipeRead(int);
    virtual int handleErrorResponse(int, int);
    virtual int handleErrorResponse(int, HttpStatusCode);
};

#endif // MOCKREQUESTHANDLER_HPP
// Path: tests/mock_includes/MockRequestHandler.hpp
