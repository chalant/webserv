#ifndef IConfigurationLOADER_HPP
# define IConfigurationLOADER_HPP

#include "IConfiguration.hpp"
#include "IExceptionHandler.hpp"

class IConfigurationLoader
{
	public:
		IConfigurationLoader(const IExceptionHandler& exc_handler);
		~IConfigurationLoader();
		IConfiguration*	loadIConfiguration(const std::string *path);
};

#endif // IConfiguration_HPP
