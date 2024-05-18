#include "../mock_includes/MockFactory.hpp"

/*
 * MockFactory.cpp
 *
 * This class is a mock implementation of the IFactory interface.
 * It is used for testing purposes.
 *
 */

MockFactory::MockFactory() {}

MockFactory::~MockFactory() {}

IConnection *MockFactory::createConnection(std::pair<int, std::pair<std::string, std::string> >)
{
    return NULL;
}

IRequest *MockFactory::createRequest()
{
    return NULL;
}

IResponse *MockFactory::createResponse()
{
    return NULL;
}

ISession *MockFactory::createSession(SessionId_t id)
{
    static_cast<void>(id);
    return NULL;
}

// Path: tests/mock_srcs/MockFactory.cpp
