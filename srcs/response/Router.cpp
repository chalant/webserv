/*Router: Selects the right 'ResponseGenerator' based on URI (etc.)
in the 'IRequest' (each locationblock in the conf file corresponds
to a 'ResponseGenerator' mapping, ie the Router selects the correct
locationblock)*/

#include "../../includes/response/Router.hpp"
#include "../../includes/response/StaticFileResponseGenerator.hpp"
#include "../../includes/response/UploadResponseGenerator.hpp"
#include "../../includes/configuration/ConfigurationLoader.hpp"
#include "../../includes/constants/HttpMethodHelper.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

typedef std::pair<int, std::pair<int, int> > Triplet_t;

Route::Route(IResponseGenerator *response_generator) :
      m_response_generator(response_generator)
{
}

Route::~Route() {}

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
        const std::vector<IConfiguration *> &locations =
        (*serverIt)->getBlocks("location");
        size_t numLocations = locations.size();
    }
}

Router::~Router()
{
    for (std::vector<IRoute*>::iterator it = m_routes.begin(); it != m_routes.end(); ++it) {
        delete *it;
    }
    // Log the destruction of the Router
    this->m_logger.log(VERBOSE, "Router destroyed.");
}

void Router::m_createServerRoutes(IConfiguration *server_block)
{
    this->m_createRoutes(server_block);
}

void Router::m_createRoutes(IConfiguration *server_block)
{
    const std::vector<IConfiguration *> &locations =
        server_block->getBlocks("location");
    std::vector<std::string> hostnames =
        server_block->getStringVector("server_name");
    std::vector<std::string>::iterator hostname_it;

    std::vector<std::string> ports = server_block->getStringVector("listen");
    std::vector<std::string>::iterator port_it;

    std::string root;
    std::string prefix;
    std::vector<std::string> methods;

    size_t i = this->getRouteCount();

    // Every Hostname has a number of routes multiplied by the number of
    // locations multiplied by the number of ports Formula: Number of routes =
    // nHostnames * nLocations * nPorts;

    for (hostname_it = hostnames.begin(); hostname_it != hostnames.end();
         hostname_it++)
    {
        for (std::vector<IConfiguration *>::const_iterator location_it = locations.begin(); location_it != locations.end();
            ++location_it)
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
            root = (*location_it)->getString("root");
            for (port_it = ports.begin(); port_it != ports.end(); port_it++)
            {
                IResponseGenerator *response_generator =
                        new StaticFileResponseGenerator(this->m_logger);
                
                /*if (true)
                {
                    IResponseGenerator *response_generator =
                        new UploadResponseGenerator(this->m_logger);
                }*/
                IRoute *new_route = new Route(response_generator);
                m_routes.push_back(new_route);
                m_routes[ i ]->setRegex((*location_it)->isRegex());
                m_routes[ i ]->setUri(*hostname_it);
                m_routes[ i ]->setHostname(*hostname_it);
                m_routes[ i ]->appendUri(":");
                m_routes[ i ]->appendUri(*port_it);
                m_routes[ i ]->setPort(*port_it);
                m_routes[ i ]->appendUri(prefix);
                m_routes[ i ]->setPath(prefix);
                m_routes[ i ]->setRoot(root);
                m_routes[ i ]->setMethod(methods,
                                       this->m_http_helper);
                i++;
            }
            m_routes[ i - 1]->setIndex((*location_it)->getString("index"));
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

std::vector<std::string> splitString(const std::string& str, const std::string& delimiters) {
    std::vector<std::string> words;
    std::string current;

    for (size_t i = 0; i < str.size(); ++i) {
        if (delimiters.find(str[i]) != std::string::npos) 
        {
            if (!current.empty()) 
            {
                words.push_back(current);
                current.clear();
            }
        } else 
        {
            current += str[i];
        }
    }
    if (!current.empty()) 
    {
        words.push_back(current);
    }
    return words;
}

bool containsRegex(const std::string& regexStr, const std::string& reqStr)
{
    std::string delimiters = "|()^/";
    std::vector<std::string> words = splitString(regexStr, delimiters);
    for (size_t i = 0; i < words.size(); i++)
    {
       if (reqStr.find(words[i]) != std::string::npos)
       {
           return true;
       }
    }
    return false;
}

Triplet_t Router::execRoute(IRequest *req, IResponse *res)
{
    (void)res; // remove this line when implementing the handler
    (void)this
        ->m_configuration; // remove this line when implementing the handler
    std::vector<IRoute*>::iterator route = m_routes.begin();

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
    for (route = m_routes.begin(); route != m_routes.end(); route++)
    {

         if (req->getHostName() == (*route)->getHostname() &&
            req->getHostPort() == (*route)->getPort() &&
            std::find((*route)->getMethod().begin(), (*route)->getMethod().end(),
            req->getMethod()) != (*route)->getMethod().end()
            && !(*route)->getRegex()
            && req->getUri() == (*route)->getRoot() + (*route)->getPath())
        {
            // Exact match
            return (*route)->getResponseGenerator()->generateResponse(**route, *req, *res, this->m_configuration);
        }
    }
    for (route = m_routes.begin(); route != m_routes.end(); route++)
    {
        if (req->getHostName() == (*route)->getHostname() &&
            req->getHostPort() == (*route)->getPort() &&
            std::find((*route)->getMethod().begin(), (*route)->getMethod().end(),
            req->getMethod()) != (*route)->getMethod().end()
            && (*route)->getRegex() 
            && containsRegex((*route)->getPath(), req->getUri()))
        {
            // regex match
            return (*route)->getResponseGenerator()->generateResponse(**route, *req, *res, this->m_configuration);
        }
    }
    for (route = m_routes.begin(); route != m_routes.end(); route++)
    {
        if (req->getHostName() == (*route)->getHostname() &&
            req->getHostPort() == (*route)->getPort() &&
            std::find((*route)->getMethod().begin(), (*route)->getMethod().end(),
            req->getMethod()) != (*route)->getMethod().end()
            && !(*route)->getRegex() 
            && req->getUri().find((*route)->getPath()) != std::string::npos)
        {
            // longest prefix match
           return (*route)->getResponseGenerator()->generateResponse(**route, *req, *res, this->m_configuration);
        }
    }
    return (*route)->getResponseGenerator()->generateResponse(**route, *req, *res, this->m_configuration);
}


size_t Router::getRouteCount(void) const { return (this->m_routes.size()); }

std::vector<IRoute*> Router::getRoutes(void) const { return (this->m_routes); }

std::string Route::getUri() const { return (this->m_uri); }

std::vector<HttpMethod> Route::getMethod() const { return (this->m_methods); }

std::string Route::getHostname() const { return (this->m_host_name); }

std::string Route::getRoot() const { return (this->m_root); }

std::string Route::getPath() const { return (this->m_path); }

std::string Route::getPort() const { return (this->m_port); }

std::string Route::getIndex() const { return (this->m_index); }

void Route::setIndex(std::string newIndex) { this->m_index = newIndex; } 

IResponseGenerator *Route::getResponseGenerator(void) const { return (this->m_response_generator); }

void Route::setUri(std::string newUri) { this->m_uri = newUri; }

void Route::setHostname(std::string newHostname) { this->m_host_name = newHostname; }

void Route::setRoot(std::string newRoot) { this->m_root = newRoot; }

void Route::setPath(std::string newPath) { this->m_path = newPath; }

void Route::setPort(std::string newPort) { this->m_port = newPort; }

void Route::setMethod(const std::vector<std::string> newMethods, HttpHelper &httpHelper)
{
    // Set the method of the request
    for (size_t i = 0; i < newMethods.size(); i++)
    {
        this->m_methods.push_back(httpHelper.stringHttpMethodMap(newMethods[i]));
    }
}

bool Route::getRegex() const { return (this->m_regex_flag); }

void Route::setRegex(bool new_regex) { this->m_regex_flag = new_regex; }

bool Route::operator<(const Route &other) const
{
    return (this->m_uri.length() > other.m_uri.length());
}

void Route::appendUri(const std::string &newString)
{
    this->m_uri.append(newString);
}

Route &Route::operator=(const Route &other)
{
    if (this != &other)
    {
        this->m_host_name = other.m_host_name;
        this->m_methods = other.m_methods;
        this->m_port = other.m_port;
        this->m_path = other.m_path;
        this->m_regex_flag = other.m_regex_flag;
        this->m_root = other.m_root;
    }
    return *this;
}
