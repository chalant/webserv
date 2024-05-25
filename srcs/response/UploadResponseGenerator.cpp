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
	const std::vector<BodyParameter>	&body_params = request.getBodyParameters();
	bool	created = false;
	for (std::vector<BodyParameter>::const_iterator itr = body_params.begin(); itr != body_params.end(); itr++)
	{
		// temp fix to ignore form fields that are not files.
		if (itr->filename.empty())
			continue;

		std::string	file_path = route.getRoot() + itr->filename;
		file_path += configuration.getBlocks("types")[0]->getStringVector(itr->content_type)[0];

		m_logger.log(DEBUG, "Received upload request for: " + file_path);
		// check if file exists.
		if (stat(file_path.c_str(), &buffer) == 0 && !created)
		{
			created = true;
		}
		std::ofstream	file(file_path.c_str());
		if (!file.is_open())
		{
			m_logger.log(ERROR, "Failed to open file for write: " + file_path);
			response.setErrorResponse(INTERNAL_SERVER_ERROR);
			return std::make_pair(-1, std::make_pair(-1, -1));
		}
		file.write(itr->data.c_str(), itr->data.size());
	}
	//if a file was created set the status to created.
	if (created)
	{
		response.setStatusLine(CREATED);
		return std::make_pair(-1, std::make_pair(-1, -1));
	}
	response.setStatusLine(OK);
	return std::make_pair(-1, std::make_pair(-1, -1));
}
