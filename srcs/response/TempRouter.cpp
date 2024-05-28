#include "../../includes/response/TempRouter.hpp"
#include "../../includes/response/Route.hpp"
#include "../../includes/response/StaticFileResponseGenerator.hpp"
#include "../../includes/response/UploadResponseGenerator.hpp"
#include "../../includes/response/CgiResponseGenerator.hpp"

/*TempRouter: Selects the right 'ResponseGenerator' based on URI (etc.)
in the 'IRequest' (each locationblock in the conf file corresponds
to a 'ResponseGenerator' mapping, ie the TempRouter selects the correct
locationblock)*/

// Constructor
TempRouter::TempRouter(IConfiguration &configuration, ILogger &logger)
    : m_configuration(configuration), m_logger(logger), m_http_helper(HttpHelper(configuration))
{
    // Log the creation of the TempRouter
    m_logger.log(VERBOSE, "Initializing TempRouter...");

    // temp set a single route
    std::string path = configuration.getString("path");
    std::string root = configuration.getString("root");
    std::string index = configuration.getString("index");

    IResponseGenerator *response_generator =
        new StaticFileResponseGenerator(m_logger);
    m_static_route = new Route(path, root, index, *response_generator);

	response_generator =
        new UploadResponseGenerator(m_logger);
    m_upload_route = new Route(path, root, index, *response_generator);

    response_generator =
        new CgiResponseGenerator(m_logger);
    m_cgi_route = new Route(path, root, index, *response_generator);
    
}

// Destructor
TempRouter::~TempRouter()
{
    // Log the destruction of the TempRouter
    m_logger.log(VERBOSE, "TempRouter destroyed.");

    // Delete the ResponseGenerator
    delete m_static_route->getResponseGenerator();
    //delete m_upload_route->getResponseGenerator();
    //delete m_cgi_route->getResponseGenerator();

    // Delete the Route
    delete m_static_route;
    //delete m_upload_route;
    //delete m_cgi_route;
}

// Execute the route
Triplet_t TempRouter::execRoute(IRequest *request, IResponse *response)
{
	IRoute	*route;
    std::string script_name;

    // temp skip routing and select the only route available
	if (request->getMethod() == POST)
	{
		m_logger.log(DEBUG, "POST REQUEST");
		route = m_upload_route;
	} else if (request->getUri() == "/hello.py")
    {
        m_logger.log(DEBUG, "CGI REQUEST");
        route = m_cgi_route;
        script_name = "hello.py";
    } else
	{
		route = m_static_route;
	}
	Triplet_t return_value = route->getResponseGenerator()->generateResponse(*route, *request, *response, m_configuration, script_name);

    // print return value
    m_logger.log(DEBUG, "Return value: " + std::to_string(return_value.first) + " " + std::to_string(return_value.second.first) + " " + std::to_string(return_value.second.second));

    return return_value;
}
