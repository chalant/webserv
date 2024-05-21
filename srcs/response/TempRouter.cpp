#include "../../includes/response/TempRouter.hpp"
#include "../../includes/response/Route.hpp"
#include "../../includes/response/StaticFileResponseGenerator.hpp"

/*TempRouter: Selects the right 'ResponseGenerator' based on URI (etc.)
in the 'IRequest' (each locationblock in the conf file corresponds
to a 'ResponseGenerator' mapping, ie the TempRouter selects the correct
locationblock)*/

// Constructor
TempRouter::TempRouter(IConfiguration &configuration, ILogger &logger)
    : _configuration(configuration), _logger(logger), _httpHelper(HttpHelper())
{
    // Log the creation of the TempRouter
    this->_logger.log(VERBOSE, "Initializing TempRouter...");

    // temp set a single route
    std::string path = configuration.getString("path");
    std::string root = configuration.getString("root");
    std::string index = configuration.getString("index");
    IResponseGenerator *responseGenerator =
        new StaticFileResponseGenerator(this->_logger);
    this->_route = new Route(path, root, index, *responseGenerator);
}

// Destructor
TempRouter::~TempRouter()
{
    // Log the destruction of the TempRouter
    this->_logger.log(VERBOSE, "TempRouter destroyed.");

    // Delete the ResponseGenerator
    delete this->_route->getResponseGenerator();

    // Delete the Route
    delete this->_route;
}

// Execute the route
Triplet_t TempRouter::execRoute(IRequest *request, IResponse *response)
{
    // temp skip routing and select the only route available
    return this->_route->getResponseGenerator()->generateResponse(
        *(this->_route), *request, *response, this->_configuration);
}
