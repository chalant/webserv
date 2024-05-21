/*PostMethodResponseGenerator: Produces a 'Response'. Input is a 'IRequest',
this class is derived from AResponseGenerator, and is selected by 'Router'*/

#include "../../includes/response/PostMethodResponseGenerator.hpp"

PostMethodResponseGenerator::PostMethodResponseGenerator()
{

}

PostMethodResponseGenerator::~PostMethodResponseGenerator()
{

}

Triplet_t	PostMethodResponseGenerator::generateResponse(const IRoute &route, 
									const IRequest &request,
                                    IResponse &response,
                                    const IConfiguration &configuration,
                                    const std::string &scriptName)
{
	(void)route;
	(void)request;
	(void)response;
	(void)configuration;
	(void)scriptName;
	
	return std::make_pair(-1, std::make_pair(-1, -1));
}
