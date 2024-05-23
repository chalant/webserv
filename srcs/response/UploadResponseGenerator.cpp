#include "../../includes/response/UploadResponseGenerator.hpp"
#include <fstream>
#include <sys/stat.h>


UploadResponseGenerator::UploadResponseGenerator(ILogger &logger)
	: m_logger(logger)
{
}

UploadResponseGenerator::~UploadResponseGenerator()
{
}

Triplet_t	UploadResponseGenerator::generateResponse(const IRoute &route, 
									const IRequest &request,
                                    IResponse &response,
                                    IConfiguration &configuration,
                                    const std::string &script_name)
{
	static_cast<void>(script_name);
	struct stat	buffer;
	std::string	file_path = route.getRoot() + route.getPath();
	bool		created = false;
	const std::vector<std::string> &extensions = configuration.getBlocks("types")[0]->getStringVector(request.getContentType());
	file_path += extensions[0];

	m_logger.log(DEBUG, "Received upload request for: " + file_path);
	// check if file exists.
	if (stat(file_path.c_str(), &buffer) == 0)
	{
		created = true;
	}
	std::ofstream	file(file_path.c_str());
	if (!file.is_open())
	{
		m_logger.log(ERROR, "Fail to open file for write: " + file_path);
		//todo: need a proper error reponse for this case.
		response.setErrorResponse(INTERNAL_SERVER_ERROR);
		return std::make_pair(-1, std::make_pair(-1, -1));
	}
	std::vector<char> body = request.getBody();
	file.write(&body[0], body.size());
	if (created)
	{
		response.setStatusLine(CREATED);
		return std::make_pair(-1, std::make_pair(-1, -1));
	}
	response.setStatusLine(OK);
	return std::make_pair(-1, std::make_pair(-1, -1));
}
