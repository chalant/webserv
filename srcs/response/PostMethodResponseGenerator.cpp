#include "../../includes/response/PostMethodResponseGenerator.hpp"
#include <fstream>


PostMethodResponseGenerator::PostMethodResponseGenerator(ILogger &logger)
	: _logger(logger)
{
	_mime_type_extension[ "text/plain" ] = "txt";
    _mime_type_extension[ "text/html" ] = "html";
    _mime_type_extension[ "image/jpeg" ] = "jpg";
    _mime_type_extension[ "image/png" ] = "png";
    _mime_type_extension[ "application/json" ] = "json";
    _mime_type_extension[ "application/pdf" ] = "pdf";
    _mime_type_extension[ "audio/mpeg" ] = "mp3";
    _mime_type_extension[ "video/mp4" ] = "mp4";
    _mime_type_extension[ "application/zip" ] = "zip";
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
	std::string file_path = route.getRoot() + route.getPath();

	std::ofstream	file(file_path.c_str());

	if (!file.is_open())
	{
		_logger.log(ERROR, "Could not open file: " + file_path);
		//todo: need a proper error reponse for this case.
		response.setErrorResponse(NOT_FOUND);
	}

	
	return std::make_pair(-1, std::make_pair(-1, -1));
}
