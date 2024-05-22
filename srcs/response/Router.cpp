/*Router: Selects the right 'ResponseGenerator' based on URI (etc.)
in the 'IRequest' (each locationblock in the conf file corresponds
to a 'ResponseGenerator' mapping, ie the Router selects the correct
locationblock)*/

#include "../../includes/response/Router.hpp"
#include "../../includes/configuration/ConfigurationLoader.hpp"
#include "../../includes/constants/HttpMethodHelper.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

Route::Route() {}

Router::Router(IConfiguration &configuration, ILogger &logger,
               HttpHelper m_http_helper)
    : m_configuration(configuration), m_logger(logger), m_http_helper(m_http_helper)
{
    // Log the creation of the Router
    this->m_logger.log(VERBOSE, "Initializing Router...");

    // For every server block in the configuration, insert a routeMapEntry into
    // the m_routes map const std::vector<IConfiguration *> servers =
    // m_configuration.getBlocks("server"); // Declare and initialize the
    // 'servers' vector
    const std::vector<IConfiguration *> &servers =
        m_configuration.getBlocks("http")[ 0 ]->getBlocks("server");
    for (std::vector<IConfiguration *>::const_iterator serverIt =
             servers.begin();
         serverIt != servers.end(); serverIt++)
    {
        this->m_createServerRoutes(*serverIt);
    }
}

Router::~Router()
{
    // Log the destruction of the Router
    this->m_logger.log(VERBOSE, "Router destroyed.");
}

void Router::m_createServerRoutes(IConfiguration *server_block)
{
    this->m_createRoutes(server_block);
}

void Router::m_createRoutes(IConfiguration *server_block)
{
    std::vector<IConfiguration *> locations =
        server_block->getBlocks("location");
    std::vector<IConfiguration *>::iterator location_it;

    std::vector<std::string> hostnames =
        server_block->getStringVector("server_name");
    std::vector<std::string>::iterator hostname_it;

    std::vector<std::string> ports = server_block->getStringVector("listen");
    std::vector<std::string>::iterator port_it;

    std::string prefix;
    std::vector<std::string> methods;

    size_t i = this->getRouteCount();

    // Every Hostname has a number of routes multiplied by the number of
    // locations multiplied by the number of ports Formula: Number of routes =
    // nHostnames * nLocations * nPorts;

    for (hostname_it = hostnames.begin(); hostname_it != hostnames.end();
         hostname_it++)
    {
        for (location_it = locations.begin(); location_it != locations.end();
             location_it++)
        {
            // methods = (*location_it)->getStringVector("limit_except");
            methods =
                (*location_it)->getBlocks("limit_except")[ 0 ]->getParameters();

            // if limit_except is not defined, default to GET
            // temp fix as maybe we should have defaults for this
            if (methods ==
                (*location_it)->getParameters()) // getBlocks returned *this
            {
                methods.clear();
                methods.push_back("GET");
            }

            // prefix = (*location_it)->getString("prefix");
            prefix = (*location_it)->getParameters()[ 0 ];
            for (port_it = ports.begin(); port_it != ports.end(); port_it++)
            {
                Route new_route;
                m_routes.push_back(new_route);
                m_routes[ i ].setUri(*hostname_it);
                m_routes[ i ].appendUri(":");
                m_routes[ i ].appendUri(*port_it);
                m_routes[ i ].appendUri(prefix);
                m_routes[ i ].setMethod(methods[ methods.size() - 1 ],
                                       this->m_http_helper);
                i++;
            }
        }
    }
    std::sort(m_routes.begin(), m_routes.end());
}

std::string rfindSubstr(const std::string &str, char occurrence)
{
    size_t pos = str.rfind(occurrence);
    if (pos != std::string::npos)
    {
        return str.substr(pos);
    }
    else
    {
        return "";
    }
}

bool isCgiRequest(IRequest *req)
{
    if (rfindSubstr(req->getUri(), '.') == ".py" ||
        rfindSubstr(req->getUri(), '.') == ".php" ||
        rfindSubstr(req->getUri(), '.') == ".pl")
        return (true);
    return (false);
}

void Router::execRoute(IRequest *req, IResponse *res)
{
    (void)res; // remove this line when implementing the handler
    (void)this
        ->m_configuration; // remove this line when implementing the handler
    std::vector<Route>::iterator i = m_routes.begin();

    // todo: implement isACgiRequest or equivalent matcher logic
    // if (isACgiRequest(req)) // ends in .py, .php, .pl etc.
    //{
    //         // handle cgi request
    //}
    if (isCgiRequest(req))
    {
        // need implementation ;
        ;
    }
    while (i != m_routes.end())
    {
        // match request path with a route
        if (req->getUri().find(i->getUri()) &&
            req->getMethod() == i->getMethod())
        {
            // i->handler(req, res); // handle static file // not implemented
            // yet
            break;
        }
        i++;
    }
}

std::string Route::getUri() const { return (this->m_uri); }

void Route::setUri(std::string newUri) { this->m_uri = newUri; }

HttpMethod Route::getMethod() const { return (this->m_method); }

void Route::setMethod(const std::string newMethod, HttpHelper &httpHelper)
{
    /*if (this->m_http_helper.isMethod(newMethod) == false)
            throw HttpStatusCodeException(METHOD_NOT_ALLOWED, // Throw '405'
    status error "unknown method: \"" + newMethod + "\""); else if
    (this->m_http_helper.isSupportedMethod(newMethod) == false) throw
    HttpStatusCodeException(NOT_IMPLEMENTED, // Throw '501' status error
                                                                      "unsupported
    method: \"" + newMethod + "\"");*/

    // Set the method of the request
    this->m_method = httpHelper.stringHttpMethodMap(newMethod);
}

bool Route::operator<(const Route &other) const
{
    return (this->m_uri.length() > other.m_uri.length());
}

void Route::appendUri(const std::string &newString)
{
    this->m_uri.append(newString);
}

// placeholder for adding a route
void Router::addRoute(const IRequest &request,
                      void (*newHandler)(IRequest *, IResponse *))
{
    (void)request;
    (void)newHandler;
}

size_t Router::getRouteCount(void) const { return (this->m_routes.size()); }

std::vector<Route> Router::getRoutes(void) const { return (this->m_routes); }

Route &Route::operator=(const Route &other)
{
    if (this != &other)
    {
        this->m_uri = other.m_uri;
        this->m_method = other.m_method;
    }
    return *this;
}
