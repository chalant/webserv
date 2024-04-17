#ifndef IREQUESTHANDLER_HPP
#define IREQUESTHANDLER_HPP

class IRequestHandler
{
public:
    virtual ~IRequestHandler() = default;

    virtual int handleRequest(int) = 0;
};

#endif // IREQUESTHANDLER_HPP
// Path: includes/IRequestHandler.hpp
