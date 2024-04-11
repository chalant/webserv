/*Router: Selects the right 'RequestHandler' based on URI (etc.) 
in the 'Request' (each locationblock in the conf file corresponds 
to a 'RequestHandler' mapping, ie the Router selects the correct 
locationblock)*/

// Router class using std::map for route storage

#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <string>
#include <map>

// Forward declarations
class Request;
class Response;

// Enum for HTTP methods
enum HttpMethod {
    GET = 0,
    POST = 1,
    PUT = 2,
    DELETE = 3,
};

// Route structure with function pointer for handler
struct Route {
public:
    std::string path;
    void (*handler)(const Request&, Response&);
    HttpMethod method;

    // Static path matching function
    static bool matchPath(const std::string& path, const std::string& pattern);
};

// Router class using std::map for route storage
class Router {
public:
    void addRoute(const Route& route);
    Route* findRoute(const Request& request);

private:
    std::map<std::string, Route> routes_;
};

#endif