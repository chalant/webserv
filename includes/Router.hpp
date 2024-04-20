/*Router: Selects the right 'ResponseGenerator' based on URI (etc.)
in the 'IRequest' (each locationblock in the conf file corresponds
to a 'ResponseGenerator' mapping, ie the Router selects the correct
locationblock)*/

// Router class using std::map for route storage

#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <string>
#include <vector>
#include "Request.hpp"
#include "HttpMethodHelper.hpp"
#include "IConfiguration.hpp"
#include "ILogger.hpp"
#include "IRouter.hpp"

class Response;

typedef std::vector<std::string> authorities;
typedef std::vector<Route> routes;
typedef std::pair<const authorities, routes> routeMapEntry;
typedef std::map<authorities, routes> routeMap;

// Route structure with function pointer for handler
struct Route
{
public:
    std::string getUri() const;
    void setUri(std::string newUri);
    HttpMethod getMethod() const;
    void setMethod(HttpMethod newMethod);
    void (*handler)(Request *, Response *);

private:
    std::string _uri;
    HttpMethod _method;
};

// Router class using std::map for route storage
class Router : public IRouter
{
public:
    Router(const IConfiguration &Configuration, ILogger &errorLogger);
    void addRoute(const Request &request, void (*newHandler)(Request *, Response *));
    void execRoute(Request *req, Response *res);

private:
    routeMap _routes;
    routeMapEntry _createServerRoutes(const IBlock *serverBlock);
    authorities _createAuthorities(const IBlock *serverBlock);
    routes _createRoutes(IBlock *serverBlock);
};

#endif