#include "../../includes/response/TempRouter.hpp"
#include "../../includes/response/RFCCgiResponseGenerator.hpp"
#include "../../includes/response/Route.hpp"
#include "../../includes/response/StaticFileResponseGenerator.hpp"
#include "../../includes/response/UploadResponseGenerator.hpp"
#include "../../includes/utils/Converter.hpp"
#include <algorithm>
#include <random>
#include "../../includes/exception/WebservExceptions.hpp"

/*TempRouter: Selects the right 'Route' and 'ResponseGenerator' based on URI (etc.)
in the 'IRequest' (each locationblock in the conf file corresponds
to a 'Route', ie the TempRouter selects the correct
locationblock)*/

// Constructor
TempRouter::TempRouter(IConfiguration &configuration, ILogger &logger)
    : m_configuration(configuration), m_logger(logger),
      m_http_helper(HttpHelper(configuration))
{
    // Log the creation of the TempRouter
    m_logger.log(VERBOSE, "Initializing TempRouter...");

    // Create the response generators
    m_response_generators["GET"] = new StaticFileResponseGenerator(logger);
    m_response_generators["POST"] = new UploadResponseGenerator(logger);
    m_response_generators["PUT"] = new UploadResponseGenerator(logger);
    //m_response_generators["CGI"] = NULL;

    // Create a route for each location block
    const BlockList &locations_list = configuration.getBlocks("http")[0]->getBlocks("server")[0]->getBlocks("location");
    for (size_t i = 0; i < locations_list.size(); i++)
    {
        std::string path;
        bool is_regex = false;
        std::vector<HttpMethod> methods;
        std::string root;
        std::string index;
        std::string cgi_script;

        // Get the path
        std::vector<std::string> &location_params = locations_list[i]->getParameters();
          path = location_params[0];

          // set is_regex
          is_regex = locations_list[i]->isRegex();

        // remove trailing slash
        if (path.length() > 1 && path[path.length() - 1] == '/')
            path = path.substr(0, path.length() - 1);

        // Get the Methods
        std::vector<std::string> &method_vector = locations_list[i]->getBlocks("limit_except")[0]->getParameters();
        std::string methods_string;
        for (size_t j = 0; j < method_vector.size(); j++)
            methods_string += method_vector[j] + " ";

        for (size_t j = 0; j < method_vector.size(); j++)
            methods.push_back(m_http_helper.stringHttpMethodMap(method_vector[j]));

        // Get the root
        const std::vector<std::string> &root_vector = locations_list[i]->getStringVector("root");
        if (root_vector.size() == 0)
            root = locations_list[i]->getString("root");
        else
            root = root_vector[0];

        // Get the index
        const std::vector<std::string> &index_vector = locations_list[i]->getStringVector("index");
        if (index_vector.size() == 0)
            index = locations_list[i]->getString("index");
        else
            index = index_vector[0];

		// add cgi's
		const BlockList	&cgis =  locations_list[i]->getBlocks("cgi");
		// if there is any CGI in the file, check if it is active and create it if it does not already exist.
		// note: a cgi response generator is mapped to its path.
		// add cgi routes
		Route *route;
		IResponseGenerator	*cgi_rg;
		IURIMatcher	*matcher;
		// marks the location as cgi
		bool	cgi_route = false;
		for (size_t j = 0; j < cgis.size(); j++)
		{
			const std::string& cgi_path = cgis[ j ]->getString("bin_path");
			const std::vector<std::string>& cgi_target = cgis[ j ]->getParameters();
			const std::string& cgi_type = cgis[ j ]->getString("cgi_type");
			if (cgi_path == "none" || cgi_target[0] == "none" || cgi_type == "none")
			{ continue; }
			cgi_route = true;
			std::map<std::string, IResponseGenerator *>::iterator	itr = m_response_generators.find(cgi_path);
			// create or retrieve a CGI response generator
			if (itr == m_response_generators.end())
			{
				cgi_rg = m_createCGIResponseGenerator(cgis[j]->getString("cgi_type"), cgi_path, m_logger);
				m_response_generators[cgi_path] = cgi_rg;
			}
			else
			{
				cgi_rg = itr->second;
			}
			//create and cache if no matcher exists
			if (m_uri_matchers.find(cgi_path) == m_uri_matchers.end())
			{
				matcher =  new ExtensionMatcher(cgi_target);
				m_uri_matchers[cgi_path] = matcher;
			}
			else { matcher = m_uri_matchers[ cgi_path ]; }
			m_uri_matchers[cgi_path] = matcher;
			route = new Route(path, is_regex, methods, root, index, cgi_path, matcher);
			m_logger.log(VERBOSE, "[TEMPROUTER] New location: '" + path + "',  methods: '" + methods_string + "', root: '" + root + "', index: '" + index + "', cgi script: '" + cgi_script + "'.");
			route->setResponseGenerator(cgi_rg);
			m_routes.push_back(route);
			m_response_generators[ cgi_path ] = cgi_rg;
		}
		if (!cgi_route)
		{
			m_logger.log(VERBOSE, "[TEMPROUTER] New location: '" + path + "',  methods: '" + methods_string + "', root: '" + root + "', index: '" + index + "', cgi script: '" + cgi_script + "'.");
			route = new Route(path, is_regex, methods, root, index);
			//route->setResponseGenerator(m_response_generators["GET"]);
			m_routes.push_back(route);
		}
        // // Get the cgi_script
        // // e.g. cgi_script /usr/bin/python3;
        // const std::vector<std::string> &cgi_script_vector = locations_list[i]->getStringVector("cgi_script");
        // if (cgi_script_vector.size() == 0) { cgi_script = ""; }
        // else { cgi_script = cgi_script_vector[0]; }

        // // Add the route
		// Route *route = new Route(path, is_regex, methods, root, index, cgi_script);
		// route->setResponseGenerator(m_response_generators["GET"]);
        // m_routes.push_back(route);

        // Log the creation of the Route
    }

    // Sort the routes
    std::sort(m_routes.begin(), m_routes.end(), m_sortRoutes);

    // print all the route paths
    for (size_t i = 0; i < m_routes.size(); i++)
    {
        m_logger.log(VERBOSE, "[TEMPROUTER] Route path: '" + m_routes[i]->getPath() + "'.");
    }
}

// Destructor
TempRouter::~TempRouter()
{
    // Log the destruction of the TempRouter
    m_logger.log(VERBOSE, "TempRouter destroyed.");

    // Delete the ResponseGenerators
    std::map<std::string, IResponseGenerator *>::iterator it;
    for (it = m_response_generators.begin(); it != m_response_generators.end(); it++)
    {
        delete it->second;
    }

    // Delete the Routes
    for (size_t i = 0; i < m_routes.size(); i++)
    {
        delete m_routes[i];
    }
}

#include <iostream>
IRoute	*TempRouter::getRoute(IRequest *request, IResponse *response)
{
	IRoute *route = m_routes[m_routes.size() - 1]; // Default route
    size_t routes_stop = m_routes.size();
    std::string uri = request->getUri();
    std::string method_str = m_http_helper.httpMethodStringMap(request->getMethod());
    IResponseGenerator *response_generator = m_response_generators[ method_str ];
    
    // Match the request to a route
    (void)response;
    // Check if cgi request
    // size_t last_dot = uri.find_last_of('.');
    // std::string extension = uri.substr(last_dot + 1);
    // if (extension == "php" || extension == "py")
    // {
    //     response_generator = m_response_generators["CGI"];
    //     uri = "." + extension; // temp for matching
    //     // set routes_stop to the first route that is not a regex
    //     for (size_t i = 0; i < routes_stop; i++)
    //     {
    //         if (m_routes[i]->isRegex() == false)
    //         {
    //             routes_stop = i;
    //             break;
    //         }
    //     }
    // }

    // Match the request to a route
    HttpMethod method = request->getMethod();
    // size_t i;
    // for (i = 0; i < routes_stop; i++)
    // {
    //     // if the route path is contained in the uri
    //     if (uri.find(m_routes[i]->getPath()) != std::string::npos)
    //     {
    //         // Check if the method is allowed
    //         if (m_routes[i]->isAllowedMethod(method) == false)
    //         {
    //             continue;
    //         }
    //         route = m_routes[i]; // select the route
    //         route->setResponseGenerator(response_generator);
    //         return route;
    //     }
    // }

	// search for a route that matches the request.
    for (size_t i = 0; i < routes_stop; i++)
    {
        // if the route path is contained in the uri
        if (m_routes[i]->match(uri) && m_routes[i]->isAllowedMethod(method)) 
		{
			//return cgi directly since it already has a response generator
			if (m_routes[i]->isCGI()) { return m_routes[ i ]; }
			m_routes[ i ]->setResponseGenerator(response_generator); 
			return m_routes [ i ];
		}
    }	

	//todo: need a better default route this could be anything...
    route = m_routes[m_routes.size() - 1]; // Default route
    if (route->isAllowedMethod(method) == false)
    {
        throw HttpStatusCodeException(METHOD_NOT_ALLOWED);
    }
	route->setResponseGenerator(response_generator);
	return route;
}

// Execute the route
Triplet_t TempRouter::execRoute(IRequest *request, IResponse *response)
{
    IRoute *route = m_routes[m_routes.size() - 1]; // Default route
    size_t routes_stop = m_routes.size();
    std::string uri = request->getUri();
    std::string method_str = m_http_helper.httpMethodStringMap(request->getMethod());
    IResponseGenerator *response_generator = m_response_generators[method_str];
    
    // Match the request to a route

    // Check if cgi request
    size_t last_dot = uri.find_last_of('.');
    std::string extension = uri.substr(last_dot + 1);
    if (extension == "php" || extension == "py")
    {
        response_generator = m_response_generators["CGI"];
        uri = "." + extension; // temp for matching
        // set routes_stop to the first route that is not a regex
        for (size_t i = 0; i < routes_stop; i++)
        {
            if (m_routes[i]->isRegex() == false)
            {
                // print the route path
                routes_stop = i;
                break;
            }
        }
    }

    // Match the request to a route
    HttpMethod method = request->getMethod();
    for (size_t i = 0; i < routes_stop; i++)
    {
        // if the route path is contained in the uri
        if (uri.find(m_routes[i]->getPath()) != std::string::npos)
        {
            // Check if the method is allowed
            if (m_routes[i]->isAllowedMethod(method) == false)
            {
                response->setErrorResponse(METHOD_NOT_ALLOWED);
                return Triplet_t(-1, std::pair<int, int>(-1, -1));
            }
            route = m_routes[i]; // select the route
            break;
        }
    }

    // Generate the response
    Triplet_t return_value = response_generator->generateResponse(*route, *request, *response, m_configuration);

    // print return value
    m_logger.log(DEBUG,
                 "Return value: " + Converter::toString(return_value.first) +
                     " " + Converter::toString(return_value.second.first) +
                     " " + Converter::toString(return_value.second.second));

    // return the return value
    return return_value;
}

Triplet_t TempRouter::execRoute(IRoute *route, IRequest *request, IResponse *response)
{
	if (!route)
		return Triplet_t(-1, std::pair<int, int>(-1, -1));
    IResponseGenerator *response_generator = route->getResponseGenerator();
    // Generate the response
    Triplet_t return_value = response_generator->generateResponse(*route, *request, *response, m_configuration);

    // print return value
    m_logger.log(DEBUG,
                 "Return value: " + Converter::toString(return_value.first) +
                     " " + Converter::toString(return_value.second.first) +
                     " " + Converter::toString(return_value.second.second));

    // return the return value
    return return_value;
}

// Sort Routes; regex first, then by path length in descending order
bool TempRouter::m_sortRoutes(const IRoute *a, const IRoute *b)
{
    if (a->isRegex() && !b->isRegex())
        return true;
    if (!a->isRegex() && b->isRegex())
        return false;
    return a->getPath().length() > b->getPath().length();
}

IResponseGenerator	*TempRouter::m_createCGIResponseGenerator(const std::string& type, const std::string& cgi_path, ILogger &logger)
{
	if (type == "file")
	{
		return new RFCCgiResponseGenerator(logger, cgi_path, true);
	}
	// default
	return new RFCCgiResponseGenerator(logger, cgi_path);
}
