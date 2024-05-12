/*Router: Selects the right 'ResponseGenerator' based on URI (etc.)
in the 'IRequest' (each locationblock in the conf file corresponds
to a 'ResponseGenerator' mapping, ie the Router selects the correct
locationblock)*/

// Router class using std::map for route storage

#ifndef ROUTER_HPP
# define ROUTER_HPP

# include <string>
# include <vector>
# include "../request/IRequest.hpp"
# include "../response/IResponse.hpp"
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
    void		(*handler)(IRequest *, IResponse *);
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
    ~Router();
    void addRoute(const IRequest &request, void (*newHandler)(IRequest *, IResponse *));
    void execRoute(IRequest *req, IResponse *res);

private:
    //routeMap_t _routes;
    void _createServerRoutes(const IConfiguration *serverBlock);
    //authorities_t _createAuthorities(const IConfiguration *serverBlock);
    void _createRoutes(const IConfiguration *serverBlock);

    const IConfiguration &_configuration;
    ILogger &_logger;
    std::vector<Route>    _routes;
};

#endif