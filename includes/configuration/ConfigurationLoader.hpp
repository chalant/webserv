#if !defined(CONFIGURATIONLOADER_HPP)
#define CONFIGURATIONLOADER_HPP

#include <string>
#include "parsing/Grammar.hpp"
#include "parsing/Parser.hpp"
#include "configuration/IConfiguration.hpp"
#include "configuration/ConfigurationBlock.hpp"

class ConfigurationLoader
{
	private:
		ILogger&			m_logger;
		ConfigurationBlock	*m_config;
	public:
		ConfigurationLoader(ILogger& logger);
		~ConfigurationLoader();
		const IConfiguration&	loadConfiguration(const std::string& path);
};

#endif
