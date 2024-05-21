/*PostMethodResponseGenerator: Produces a 'Response'. Input is a 'IRequest',
this class is derived from AResponseGenerator, and is selected by 'Router'*/

#ifndef POSTMETHODRESPONSEGENERATOR_HPP
#define POSTMETHODRESPONSEGENERATOR_HPP

#include "../../includes/response/IResponseGenerator.hpp"

class PostMethodResponseGenerator: public IResponseGenerator
{
	private:
		/* data */
	public:
		PostMethodResponseGenerator(/* args */);
		~PostMethodResponseGenerator();
		Triplet_t	generateResponse(const IRoute &route, 
									const IRequest &request,
									IResponse &response,
									const IConfiguration &configuration,
									const std::string &scriptName = "");
};

#endif