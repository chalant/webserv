#ifndef ISERVER_HPP
#define ISERVER_HPP

class IServer
{
public:
    virtual ~IServer() = default;

    virtual void acceptConnection() = 0;    
    virtual void terminate(int) = 0;
    virtual int getServerSocketDescriptor() const = 0;
};

#endif // ISERVER_HPP
// Path: includes/ISocket.hpp