#include "../../includes/configuration/LocationBlock.hpp"

LocationBlock::LocationBlock(ILogger &logger, const std::string name, bool is_regex):
	ConfigurationBlock(logger, name), m_is_regex(is_regex) {
}

LocationBlock::LocationBlock(const ConfigurationBlock &parent, const std::string name, bool is_regex):
	ConfigurationBlock(parent, name), m_is_regex(is_regex) {		
}

LocationBlock::~LocationBlock() {
}

bool	LocationBlock::isRegex() const {
	return m_is_regex;
}
