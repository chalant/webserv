/*Router: Selects the right 'ResponseGenerator' based on URI (etc.) 
in the 'IRequest' (each locationblock in the conf file corresponds 
to a 'ResponseGenerator' mapping, ie the Router selects the correct 
locationblock)*/

// Router class using std::map for route storage

#ifndef ROUTER_HPP
#define ROUTER_HPP

# include <string>
# include <map>
# include "HttpMethodHelper.hpp" 

class IRequest;
class Response;

// Route structure with function pointer for handler
struct Route {
public:
    std::string path;
    void (*handler)(const IRequest&, Response&);
    HttpMethod method;

    // Static path matching function
    static bool matchPath(const std::string& path, const std::string& pattern);
};

// Router class using std::map for route storage
class Router {
public:
    void addRoute(const Route& route);
    Route* findRoute(const IRequest& request);

private:
    std::map<std::string, Route> routes_;
};

#endif