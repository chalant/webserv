#if !defined(CONFIGURATIONLOADER_HPP)
#define CONFIGURATIONLOADER_HPP

#include <string>
#include "Grammar.hpp"
#include "Parser.hpp"
#include "configuration/IBlock.hpp"
#include "configuration/Block.hpp"

class ConfigurationLoader
{
	private:
		Grammar		*m_grammar;
		ILogger&	m_logger;
		Block		*m_config;
	public:
		ConfigurationLoader(ILogger& logger);
		~ConfigurationLoader();
		const IBlock&	loadConfiguration(const std::string& path);
};

#endif
