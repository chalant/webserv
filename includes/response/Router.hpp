/*Router: Selects the right 'ResponseGenerator' based on URI (etc.)
in the 'IRequest' (each locationblock in the conf file corresponds
to a 'ResponseGenerator' mapping, ie the Router selects the correct
locationblock)*/

// Router class using std::map for route storage

#ifndef ROUTER_HPP
# define ROUTER_HPP

# include <string>
# include <vector>
# include "../request/Request.hpp"
# include "../constants/HttpMethodHelper.hpp"
# include "../configuration/IConfiguration.hpp"
# include "../logger/ILogger.hpp"
# include "IRouter.hpp"

class Response;

/*typedef std::vector<std::string> authorities_t;
typedef std::vector<Route> routes_t;
typedef std::pair<const authorities_t, routes_t> routeMapEntry_t;
typedef std::map<authorities_t, routes_t> routeMap_t;*/

// Route structure with function pointer for handler
struct Route
{
public:
    std::string getUri() const;
    void 		setUri(std::string newUri);
    void        appendUri(const std::string &newString);
    HttpMethod  getMethod() const;
    void		setMethod(HttpMethod newMethod);
    void		(*handler)(Request *, Response *);
    bool		operator< (const Route &other) const;

private:
    std::string _uri;
    HttpMethod _method;
};

// Router class using std::map for route storage
class Router : public IRouter
{
public:
    Router(const IConfiguration &Configuration, ILogger &logger);
    void addRoute(const Request &request, void (*newHandler)(Request *, Response *));
    void execRoute(Request *req, Response *res);

private:
    /*routeMap_t _routes;
    routeMapEntry_t _createServerRoutes(const IBlock *serverBlock);
    authorities_t _createAuthorities(const IBlock *serverBlock);
    routes_t _createRoutes(IBlock *serverBlock);*/

    std::vector<Route>    _routes;
    void _createServerRoutes(IConfiguration *serverBlock);
    void _createRoutes(IConfiguration *serverBlock);

};

#endif