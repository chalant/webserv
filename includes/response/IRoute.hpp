#ifndef IROUTE_HPP
#define IROUTE_HPP

#include <string>
#include "Response.hpp"
#include "../request/IRequest.hpp"
#include "../constants/HttpMethodHelper.hpp"

class IRoute
{
public:
    virtual ~IRoute(){};

    virtual std::string getUri() const = 0;
    virtual void setUri(std::string newUri) = 0;
    virtual HttpMethod getMethod() const = 0;
    virtual void setMethod(HttpMethod newMethod) = 0;
    // virtual void (*handler)(IRequest *, IResponse *) = 0;
    virtual std::string getRoot() const = 0;
    virtual std::string getPrefix() const = 0;
    virtual void setRoot(std::string &root) = 0;
    virtual void setPrefix(std::string &prefix) = 0;
};

#endif // IROUTE_HPP
       // Path: includes/response/IRoute.hpp