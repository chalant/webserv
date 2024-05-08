/*Router: Selects the right 'ResponseGenerator' based on URI (etc.)
in the 'IRequest' (each locationblock in the conf file corresponds
to a 'ResponseGenerator' mapping, ie the Router selects the correct
locationblock)*/

#include "../../includes/response/Router.hpp"
#include "../../includes/constants/HttpMethodHelper.hpp"
#include <vector>
#include <algorithm>

Router::Router(const IConfiguration &configuration, ILogger &logger) : _configuration(configuration), _logger(logger)
{
	// For every server block in the configuration, insert a routeMapEntry into the _routes map
	std::vector<IConfiguration *> servers = configuration.getBlocks("http")[0]->getBlocks("server");
	std::vector<IConfiguration *>::iterator serverIt;
	for (serverIt = servers.begin(); serverIt != servers.end(); serverIt++)
	{
		this->_createServerRoutes(*serverIt);
	}
}

void Router::_createServerRoutes(IConfiguration *serverBlock)
{
	this->_createRoutes(serverBlock);
}

void Router::_createRoutes(IConfiguration *serverBlock)
{
	std::vector<IConfiguration *> locations = serverBlock->getBlocks("location");
	std::vector<IConfiguration *>::iterator locationIt;
	std::vector<std::string> hostnames = serverBlock->getStringVector("serverName");
	std::vector<std::string> ports = serverBlock->getStringVector("port");
	std::vector<std::string> methods = (*locationIt)->getStringVector("limitExcept");
	std::vector<std::string>::iterator hostnameIt;
	std::vector<std::string>::iterator portIt;
	size_t	i = 0;

	for (hostnameIt = hostnames.begin(); hostnameIt != hostnames.end(); hostnameIt++)
	{
		_routes[i].appendUri(*hostnameIt);
		_routes[i++].appendUri(":");
	}

	i = 0;
	for (portIt = ports.begin(); portIt != ports.end(); portIt++)
	{
		_routes[i++].appendUri(*portIt);
	}
	i = 0; 
	HttpMethodHelper helper(this->_configuration);
	for (locationIt = locations.begin(); locationIt != locations.end(); locationIt++)
	{
		std::string prefix = (*locationIt)->getString("prefix");
		_routes[i].setMethod(helper.stringHttpMethodMap(methods[i]));
		_routes[i++].appendUri(prefix);
	}
	std::sort(_routes.begin(), _routes.end());
}

void Router::execRoute(IRequest *req, IResponse *res)
{
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

bool Route::operator< (const Route &other) const 
{
        return (this->_uri.length() < other._uri.length());
}

void Route::appendUri(const std::string& newString)
{
	this->_uri.append(newString);
}

