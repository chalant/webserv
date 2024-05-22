#ifndef CONFIGURATIONLOADER_HPP
#define CONFIGURATIONLOADER_HPP

#include "../configuration/ConfigurationBlock.hpp"
#include "../configuration/Defaults.hpp"
#include "../configuration/IConfiguration.hpp"
#include "../../includes/parsing/Grammar.hpp"
#include "../../includes/parsing/Tokenizer.hpp"
#include "../../includes/parsing/ParseTree.hpp"
#include <string>

class ConfigurationLoader
{
private:
    ILogger	&m_logger;
    ConfigurationBlock	*m_config;
    Defaults			m_defaults;
	std::vector<std::string>	m_reserved_symbols;
	std::vector<std::string>	m_separators;
	void				m_add_directive(const Grammar &grammar, const std::vector<Token> &tokens, ParseTree &parse_tree, ConfigurationBlock &block);
	void				m_add_block(const Grammar &grammar, const std::vector<Token> &tokens, ParseTree &parse_tree, ConfigurationBlock &block);
	void				m_build_config(const Grammar &grammar, const std::vector<Token> &tokens, ParseTree &parse_tree, ConfigurationBlock &block);

public:
    ConfigurationLoader(ILogger &logger);
    ~ConfigurationLoader();
    IConfiguration &loadConfiguration(const std::string &path);
};

#endif
