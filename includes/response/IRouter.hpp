#ifndef IROUTER_HPP
#define IROUTER_HPP

class IRouter
{
public:
    ~IRouter() {}

    virtual void addRoute(const Request &request, void (*newHandler)(Request *, Response *)) = 0;
    virtual void execRoute(Request *req, Response *res) = 0;
};

#endif // IROUTER_HPP
// Path: includes/IRouter.hpp
