#ifndef CONFIGURATIONLOADER_HPP
# define CONFIGURATIONLOADER_HPP

#include "Configuration.hpp"
#include "ExceptionHandler.hpp"

class ConfigurationLoader
{
	public:
		ConfigurationLoader(const ExceptionHandler &exc_handler);
		~ConfigurationLoader();
		Configuration*	loadConfiguration(const std::string *path);
};

#endif // CONFIGURATION_HPP
