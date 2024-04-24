#include "MockFactory.hpp"

/*
 * MockFactory.cpp
 *
 * This class is a mock implementation of the IFactory interface.
 * It is used for testing purposes.
 *
 */

MockFactory::MockFactory() {}

MockFactory::~MockFactory() {}

IConnection *MockFactory::createConnection(std::pair<int, std::pair<std::string, std::string>>)
{
    return nullptr;
}

IRequest *MockFactory::createRequest()
{
    return nullptr;
}

IResponse *MockFactory::createResponse()
{
    return nullptr;
}

ISession *MockFactory::createSession(SessionId_t id)
{
    static_cast<void>(id);
    return nullptr;
}

// Path: tests/mock_srcs/MockFactory.cpp
