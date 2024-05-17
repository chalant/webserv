/*Router: Selects the right 'ResponseGenerator' based on URI (etc.)
in the 'IRequest' (each locationblock in the conf file corresponds
to a 'ResponseGenerator' mapping, ie the Router selects the correct
locationblock)*/

#include "../../includes/response/Router.hpp"
#include "../../includes/constants/HttpMethodHelper.hpp"
#include "../../includes/configuration/ConfigurationLoader.hpp"
#include "../../includes/configuration/LocationBlock.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

Route::Route(const HttpHelper &httpHelper) : _httpHelper(httpHelper)
{
}

Router::Router(const IConfiguration &configuration, ILogger &logger) : _configuration(configuration), _logger(logger)
{
	// Log the creation of the Router
	this->_logger.log(VERBOSE, "Initializing Router...");

	// For every server block in the configuration, insert a routeMapEntry into the _routes map
	//const std::vector<IConfiguration *> servers = _configuration.getBlocks("server"); // Declare and initialize the 'servers' vector
	const std::vector<IConfiguration *>& servers = _configuration.getBlocks("http")[0]->getBlocks("server");
	//std::vector<IConfiguration *>::iterator serverIt;
	for (std::vector<IConfiguration *>::const_iterator serverIt = servers.begin(); serverIt != servers.end(); serverIt++)
	{
		this->_createServerRoutes(*serverIt);
	}
}

Router::~Router()
{
	// Log the destruction of the Router
	this->_logger.log(VERBOSE, "Router destroyed.");
}

void Router::_createServerRoutes(const IConfiguration *serverBlock)
{
	this->_createRoutes(serverBlock);
}

void Router::_createRoutes(const IConfiguration *serverBlock)
{

	std::vector<IConfiguration *> locations = serverBlock->getBlocks("location");

	std::vector<IConfiguration *>::iterator locationIt;
	std::vector<std::string> hostnames = serverBlock->getStringVector("server_name");

	std::vector<std::string> ports = serverBlock->getStringVector("listen");
	std::vector<std::string>::iterator hostnameIt;
	std::vector<std::string>::iterator portIt;
	std::string prefix;
	size_t	i = this->getRouteCount();
	size_t	serverRoutesNumber = 0;
	for (hostnameIt = hostnames.begin(); hostnameIt != hostnames.end(); hostnameIt++)
	{
		HttpHelper HttpHelper;
		Route newRoute(HttpHelper);
		_routes.push_back(newRoute);
		_routes[i].setUri(*hostnameIt);
		_routes[i++].appendUri(":");
		serverRoutesNumber++;
	}
	i = this->getRouteCount() - serverRoutesNumber;
	portIt = ports.begin();
	while (i < serverRoutesNumber)
		_routes[i++].appendUri(*portIt);
	i = 0; // replace this later
	// std::vector<std::string> methods = (*locationIt)->getStringVector("limitExcept");
	for (locationIt = locations.begin(); locationIt != locations.end(); locationIt++)
	{
		prefix = (*locationIt)->getString("prefix");
		//_routes[i].setMethod(methods[i]);
		//_routes[i].setMethod((*locationIt)->getString("method"));
		_routes[i++].appendUri(prefix);
		//std::cout << "uri: " << _routes[i].getUri() << std::endl;
	}
	std::sort(_routes.begin(), _routes.end());
}

void Router::execRoute(IRequest *req, IResponse *res)
{
	(void)res; // remove this line when implementing the handler
	(void)this->_configuration; // remove this line when implementing the handler
    std::vector<Route>::iterator i = _routes.begin();

	// todo: implement isACgiRequest or equivalent matcher logic
	//if (isACgiRequest(req)) // ends in .py, .php, .pl etc.
    //{ 
    //         // handle cgi request
    //}
    while (i != _routes.end())
    {
        // match request path with a route
        if (req->getUri().find(i->getUri()) && req->getMethod() == i->getMethod())
        {  
            // i->handler(req, res); // handle static file // not implemented yet
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

void Route::setMethod(const std::string newMethod)
{
	/*if (this->_httpHelper.isMethod(newMethod) == false)
        throw HttpStatusCodeException(METHOD_NOT_ALLOWED, // Throw '405' status error
                                      "unknown method: \"" + newMethod + "\"");
    else if (this->_httpHelper.isSupportedMethod(newMethod) == false)
        throw HttpStatusCodeException(NOT_IMPLEMENTED, // Throw '501' status error
                                      "unsupported method: \"" + newMethod + "\"");*/

    // Set the method of the request
    this->_method = this->_httpHelper.stringHttpMethodMap(newMethod);
}

bool Route::operator< (const Route &other) const 
{
        return (this->_uri.length() < other._uri.length());
}

void Route::appendUri(const std::string& newString)
{
	this->_uri.append(newString);
}

// placeholder for adding a route
void Router::addRoute(const IRequest &request, void (*newHandler)(IRequest *, IResponse *))
{
	(void)request;
	(void)newHandler;
}

size_t Router::getRouteCount(void) const
{
	return (this->_routes.size());
}

std::vector<Route> Router::getRoutes(void) const {
	return (this->_routes);
}

Route& Route::operator= (const Route &other) 
{
	if (this != &other) {
		this->_uri = other._uri;
		this->_method = other._method;
	}
	return *this;
}
