#ifndef ISESSION_HPP
#define ISESSION_HPP

#include <string>
#include "request/IRequest.hpp"
#include "response/IResponse.hpp"

class ISession
{
public:
    virtual ~ISession() = default;

    virtual void setReadPipeDescriptor(int pipe) = 0;

    virtual int getSocket() const = 0;
    virtual const std::string &getIp() const = 0;
    virtual int getPort() const = 0;
    virtual const std::string &getRemoteAddress() const = 0;
    virtual int getReadPipeDescriptor() const = 0;
    virtual IRequest &getRequest() const = 0;
    virtual IResponse &getResponse() const = 0;
};

#endif // ISESSION_HPP
// Path: includes/ISession.hpp
