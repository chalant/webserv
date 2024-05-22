/*Router: Selects the right 'ResponseGenerator' based on URI (etc.)
in the 'IRequest' (each locationblock in the conf file corresponds
to a 'ResponseGenerator' mapping, ie the Router selects the correct
locationblock)*/

// Router class using std::map for route storage

#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "../configuration/IConfiguration.hpp"
#include "../constants/HttpMethodHelper.hpp"
#include "../logger/ILogger.hpp"
#include "../request/IRequest.hpp"
#include "../request/Request.hpp"
#include "../response/IResponse.hpp"
#include "IRouter.hpp"
#include <string>
#include <vector>

class Response;

// Route structure with function pointer for handler
class Route
{
public:
    Route();
    std::string	getUri() const;
    void setUri(std::string new_uri);
    void appendUri(const std::string &new_string);
    HttpMethod getMethod() const;
    void setMethod(const std::string new_method, HttpHelper &http_helper);
    void (*handler)(IRequest *, IResponse *);
    bool operator<(const Route &other) const;
    Route &operator=(const Route &other);

private:
    std::string m_uri;
    HttpMethod m_method;
};

// Router class using std::map for route storage
class Router : public IRouter
{
public:
    Router(IConfiguration &Configuration, ILogger &logger,
           HttpHelper m_http_helper);
    ~Router();
    void addRoute(const IRequest &request,
                  void (*new_handler)(IRequest *, IResponse *));
    void execRoute(IRequest *req, IResponse *res);
    std::vector<Route> getRoutes(void) const;
    size_t getRouteCount(void) const;

private:
    void m_createServerRoutes(IConfiguration *server_block);
    void m_createRoutes(IConfiguration *server_block);
    IConfiguration &m_configuration;
    std::vector<Route> m_routes;
    ILogger &m_logger;
    HttpHelper m_http_helper;
};

#endif
