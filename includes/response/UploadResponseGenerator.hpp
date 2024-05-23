/*PostMethodResponseGenerator: Produces a 'Response'. Input is a 'IRequest',
this class is derived from AResponseGenerator, and is selected by 'Router'*/

#ifndef UPLOADRESPONSEGENERATOR_HPP
#define UPLOADRESPONSEGENERATOR_HPP

#include "../../includes/response/IResponseGenerator.hpp"
#include "../../includes/logger/ILogger.hpp"
#include "../../includes/configuration/IConfiguration.hpp"
#include "../../includes/configuration/BlockList.hpp"

class UploadResponseGenerator: public IResponseGenerator
{
	private:
		ILogger	&m_logger;
	public:
		UploadResponseGenerator(ILogger &logger);
		~UploadResponseGenerator();
		Triplet_t	generateResponse(const IRoute &route, 
									const IRequest &request,
									IResponse &response,
									IConfiguration &configuration,
									const std::string &script_name = "");
};

#endif