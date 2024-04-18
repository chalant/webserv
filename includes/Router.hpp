/*Router: Selects the right 'ResponseGenerator' based on URI (etc.) 
in the 'IRequest' (each locationblock in the conf file corresponds 
to a 'ResponseGenerator' mapping, ie the Router selects the correct 
locationblock)*/

// Router class using std::map for route storage

#ifndef ROUTER_HPP
#define ROUTER_HPP

# include <string>
# include <vector>
# include "Request.hpp"
# include "HttpMethodHelper.hpp" 

class Response;

// Route structure with function pointer for handler
struct Route {
public:
    
    std::string getUri()const ;
    void setUri(std::string newUri);
    HttpMethod getMethod() const;
    void setMethod(HttpMethod newMethod);
    void (*handler)(Request*, Response*);

    private:
        std::string _uri;
        HttpMethod _method;
};

// Router class using std::map for route storage
class Router {
public:
    void addRoute(const Request& request, void (*newHandler)(Request*, Response*));
    void execRoute(Request* req, Response* res);

private:
    std::vector<Route> _routes;
};

#endif