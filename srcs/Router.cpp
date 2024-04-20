/*Router: Selects the right 'ResponseGenerator' based on URI (etc.)
in the 'IRequest' (each locationblock in the conf file corresponds
to a 'ResponseGenerator' mapping, ie the Router selects the correct
locationblock)*/

#include "Router.hpp"
#include <iostream>
#include <vector>

Router::Router(const IConfiguration &Configuration, ILogger &errorLogger) {}

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

	while (i != _routes.end())
	{
		// match request path with a route
		std::string uri = i->getUri();

		if (req->getUri() == i->getUri() && req->getMethod() == i->getMethod())
		{
			i->handler(req, res); // handle static file
			break;
		}
		i++;
	}
	if (isACgiRequest(req))
	{ // ends in .py, .php, .pl etc.
		// handle cgi request
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
