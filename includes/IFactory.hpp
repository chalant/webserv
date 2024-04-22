#ifndef IFACTORY_HPP
#define IFACTORY_HPP

#include "ISession.hpp"
#include "IRequest.hpp"
#include "IResponse.hpp"

class IFactory
{
public:
    virtual ~IFactory() {};

    virtual ISession * createSession(std::pair<int, std::pair<std::string, std::string>>) = 0;
    virtual IRequest * createRequest() = 0;
    virtual IResponse * createResponse() = 0;
};

#endif // IFACTORY_HPP
// Path: includes/IFactory.hpp
