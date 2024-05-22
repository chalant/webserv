#include "../../includes/response/PostMethodResponseGenerator.hpp"
#include <fstream>


PostMethodResponseGenerator::PostMethodResponseGenerator(ILogger &logger)
	: m_logger(logger)
{
	m_mime_type_extension[ "text/plain" ] = "txt";
    m_mime_type_extension[ "text/html" ] = "html";
    m_mime_type_extension[ "image/jpeg" ] = "jpg";
    m_mime_type_extension[ "image/png" ] = "png";
    m_mime_type_extension[ "application/json" ] = "json";
    m_mime_type_extension[ "application/pdf" ] = "pdf";
    m_mime_type_extension[ "audio/mpeg" ] = "mp3";
    m_mime_type_extension[ "video/mp4" ] = "mp4";
    m_mime_type_extension[ "application/zip" ] = "zip";
}

PostMethodResponseGenerator::~PostMethodResponseGenerator()
{
}

Triplet_t	PostMethodResponseGenerator::generateResponse(const IRoute &route, 
									const IRequest &request,
                                    IResponse &response,
                                    const IConfiguration &configuration,
                                    const std::string &script_name)
{
	(void)route;
	(void)request;
	(void)response;
	(void)configuration;
	(void)script_name;
	std::string file_path = route.getRoot() + route.getPath();

	std::ofstream	file(file_path.c_str());

	if (!file.is_open())
	{
		m_logger.log(ERROR, "Could not open file: " + file_path);
		//todo: need a proper error reponse for this case.
		response.setErrorResponse(NOT_FOUND);
	}

	
	return std::make_pair(-1, std::make_pair(-1, -1));
}
