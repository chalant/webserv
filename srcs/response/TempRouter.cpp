#include "../../includes/response/TempRouter.hpp"
#include "../../includes/response/Route.hpp"
#include "../../includes/response/StaticFileResponseGenerator.hpp"
#include "../../includes/response/UploadResponseGenerator.hpp"

/*TempRouter: Selects the right 'ResponseGenerator' based on URI (etc.)
in the 'IRequest' (each locationblock in the conf file corresponds
to a 'ResponseGenerator' mapping, ie the TempRouter selects the correct
locationblock)*/

// Constructor
TempRouter::TempRouter(IConfiguration &configuration, ILogger &logger)
    : m_configuration(configuration), m_logger(logger), m_http_helper(HttpHelper())
{
    // Log the creation of the TempRouter
    this->m_logger.log(VERBOSE, "Initializing TempRouter...");

    // temp set a single route
    std::string path = configuration.getString("path");
    std::string root = configuration.getString("root");
    std::string index = configuration.getString("index");
    IResponseGenerator *response_generator =
        new StaticFileResponseGenerator(this->m_logger);
    this->m_static_route = new Route(path, root, index, *response_generator);
	response_generator =
        new UploadResponseGenerator(this->m_logger);
    this->m_upload_route = new Route(path, root, index, *response_generator);
}

// Destructor
TempRouter::~TempRouter()
{
    // Log the destruction of the TempRouter
    this->m_logger.log(VERBOSE, "TempRouter destroyed.");

    // Delete the ResponseGenerator
    delete this->m_static_route->getResponseGenerator();

    // Delete the Route
    delete this->m_static_route;
}

// Execute the route
Triplet_t TempRouter::execRoute(IRequest *request, IResponse *response)
{
	IRoute	*route;
    // temp skip routing and select the only route available
	if (request->getMethod() == POST)
	{
		 route = m_upload_route;
	}
	else
	{
		route = m_static_route;
	}
	return route->getResponseGenerator()->generateResponse(*route, *request, *response, this->m_configuration);
}
