#ifndef IROUTER_HPP
#define IROUTER_HPP

/*
 * IRouter.hpp
 * Abstract class for the Router
 */

#include "../request/IRequest.hpp"
#include "../response/IResponse.hpp"

class IRouter
{
public:
    virtual ~IRouter(){};

    virtual void addRoute(const IRequest &request, void (*newHandler)(IRequest *, IResponse *)) = 0;
    virtual void execRoute(IRequest *req, IResponse *res) = 0;
};

#endif // IROUTER_HPP
// Path: includes/IRouter.hpp
