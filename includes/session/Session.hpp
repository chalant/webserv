#ifndef SESSION_HPP
#define SESSION_HPP

#include "ISession.hpp"
#include "ILogger.hpp"

class Session : public ISession
{
private:
    int _socketDescriptor;
    std::string _ip;
    int _port;
    std::string _remoteAddress;
    int _readPipeDescriptor;
    ILogger &_logger;
    IRequest *_request;
    IResponse *_response;

public:
    Session(std::pair<int, std::pair<std::string, std::string>> clientInfo, ILogger &logger, IRequest *request, IResponse *response);
    virtual ~Session() = default;

    virtual void setReadPipeDescriptor(int pipe);

    virtual int getSocket() const;
    virtual const std::string &getIp() const;
    virtual int getPort() const;
    virtual const std::string &getRemoteAddress() const;
    virtual int getReadPipeDescriptor() const;
    virtual IRequest &getRequest();
    virtual IResponse &getResponse();
};

#endif // SESSION_HPP
// Path: includes/Session.hpp
