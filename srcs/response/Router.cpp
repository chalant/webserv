/*Router: Selects the right 'ResponseGenerator' based on URI (etc.)
in the 'IRequest' (each locationblock in the conf file corresponds
to a 'ResponseGenerator' mapping, ie the Router selects the correct
locationblock)*/

#include "Router.hpp"
#include <iostream>
#include <vector>

Router::Router(const IConfiguration &configuration, ILogger &logger)
{
	// For every server block in the configuration, insert a routeMapEntry into the _routes map
	std::vector<IBlock *> servers = configuration.getHttp().getBlocks("server");
	std::vector<IBlock *>::iterator serverIt;
	for (serverIt = servers.begin(); serverIt != servers.end(); serverIt++)
	{
		routeMapEntry newEntry = this->_createServerRoutes(*serverIt);
		this->_routes.insert(newEntry);
	}
}

routeMapEntry Router::_createServerRoutes(const IBlock *serverBlock)
{
	// Make Authority vector
	authorities authorities = this->_createAuthorities(serverBlock);

	// Make Route vector
	routes routes = this->_createRoutes(serverBlock);

	// Return the pair of authorities and routes
	return std::make_pair(authorities, routes);
}

authorities Router::_createAuthorities(const IBlock *serverBlock)
{
	// Permutations of hostnames and ports
	std::vector<std::string> authorities;

	std::vector<std::string> hostnames = serverBlock->getStringVector("serverName");
	std::vector<std::string> ports = serverBlock->getStringVector("port");

	std::vector<std::string>::iterator hostnameIt;
	std::vector<std::string>::iterator portIt;

	for (hostnameIt = hostnames.begin(); hostnameIt != hostnames.end(); hostnameIt++)
	{
		for (portIt = ports.begin(); portIt != ports.end(); portIt++)
		{
			authorities.push_back(*hostnameIt + ":" + *portIt);
		}
	}
	return authorities;
}

routes Router::_createRoutes(IBlock *serverBlock)
{
	routes routes;

	std::vector<IBlock *> locations = serverBlock->getBlocks("location");

	std::vector<IBlock *>::iterator locationIt;

	for (locationIt = locations.begin(); locationIt != locations.end(); locationIt++)
	{
		std::vector<HttpMethod> methods = (*locationIt)->getStringVector("limitExcept");
		std::string prefix = (*locationIt)->getString("prefix");
		Route route;
		route.setMethods(methods);
		route.setPrefix(prefix);
		routes.push_back(route);
	}
	return routes;
}

void Router::addRoute(const Request &request, void (*newHandler)(Request *, Response *))
{
	// Simply adds a route to the vector of the Router
	Route route;
	route.setUri(request.getUri());
	route.setMethod(request.getMethod());
	route.handler = newHandler;
	_routes.push_back(route);
}

void Router::execRoute(Request *req, Response *res)
{
    std::vector<Route>::iterator i = _routes.begin();

	if (isACgiRequest(req)) // ends in .py, .php, .pl etc.
    { 
             // handle cgi request
    }
    while (i != _routes.end())
    {
        // match request path with a route
        if (req->getUri().find(i->getUri()) && req->getMethod() == i->getMethod())
        {  
            i->handler(req, res); // handle static file
            break;
        }
        i++;
    }
}

std::string Route::getUri() const
{
	return (this->_uri);
}

void Route::setUri(std::string newUri)
{
	this->_uri = newUri;
}

HttpMethod Route::getMethod() const
{
	return (this->_method);
}

void Route::setMethod(HttpMethod newMethod)
{
	this->_method = newMethod;
}
