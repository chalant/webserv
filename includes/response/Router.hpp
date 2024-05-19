/*Router: Selects the right 'ResponseGenerator' based on URI (etc.)
in the 'IRequest' (each locationblock in the conf file corresponds
to a 'ResponseGenerator' mapping, ie the Router selects the correct
locationblock)*/

// Router class using std::map for route storage

#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <string>
#include <vector>
#include "../request/IRequest.hpp"
#include "../request/Request.hpp"
#include "../response/IResponse.hpp"
#include "../constants/HttpMethodHelper.hpp"
#include "../configuration/IConfiguration.hpp"
#include "../logger/ILogger.hpp"
#include "IRouter.hpp"

class Response;

// Route structure with function pointer for handler
class Route
{
public:
    Route();
    std::string getUri() const;
    void setUri(std::string newUri);
    void appendUri(const std::string &newString);
    HttpMethod getMethod() const;
    void setMethod(const std::string newMethod, HttpHelper &httpHelper);
    void (*handler)(IRequest *, IResponse *);
    bool operator<(const Route &other) const;
    Route &operator=(const Route &other);

private:
    std::string _uri;
    HttpMethod _method;
};

// Router class using std::map for route storage
class Router : public IRouter
{
public:
    Router(const IConfiguration &Configuration, ILogger &logger, HttpHelper _httpHelper);
    ~Router();
    void addRoute(const IRequest &request, void (*newHandler)(IRequest *, IResponse *));
    void execRoute(IRequest *req, IResponse *res);
    std::vector<Route> getRoutes(void) const;
    size_t getRouteCount(void) const;

private:
    void _createServerRoutes(const IConfiguration *serverBlock);
    void _createRoutes(const IConfiguration *serverBlock);
    const IConfiguration &_configuration;
    std::vector<Route> _routes;
    ILogger &_logger;
    HttpHelper _httpHelper;
};

#endif