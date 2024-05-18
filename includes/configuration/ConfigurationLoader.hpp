#ifndef CONFIGURATIONLOADER_HPP
#define CONFIGURATIONLOADER_HPP

#include <string>
#include "../configuration/IConfiguration.hpp"
#include "../configuration/ConfigurationBlock.hpp"
#include "../configuration/Defaults.hpp"

class ConfigurationLoader
{
private:
	ILogger &m_logger;
	ConfigurationBlock *m_config;
	Defaults m_defaults;

public:
	ConfigurationLoader(ILogger &logger);
	~ConfigurationLoader();
	IConfiguration &loadConfiguration(const std::string &path);
};

#endif
