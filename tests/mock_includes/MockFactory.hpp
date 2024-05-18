#ifndef MOCKFACTORY_HPP
#define MOCKFACTORY_HPP

/*
 * MockFactory.hpp
 *
 * This class is a mock implementation of the IFactory interface.
 * It is used for testing purposes.
 *
 */

#include "../../includes/factory/IFactory.hpp"

class MockFactory : public IFactory
{
public:
    MockFactory();
    ~MockFactory();

    virtual IConnection *createConnection(std::pair<int, std::pair<std::string, std::string>>);
    virtual IRequest *createRequest();
    virtual IResponse *createResponse();
    virtual ISession *createSession(SessionId_t id);
};

#endif // MOCKFACTORY_HPP
// Path: tests/mock_includes/MockFactory.hpp
