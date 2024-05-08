#include "../../includes/configuration/LocationBlock.hpp"

LocationBlock::LocationBlock(ILogger &logger, const std::string name, IURIMatcher* uri_matcher):
	ConfigurationBlock(logger, name), m_uri_matcher(uri_matcher) {
}

LocationBlock::LocationBlock(const ConfigurationBlock &parent, const std::string name, IURIMatcher* uri_matcher):
	ConfigurationBlock(parent, name), m_uri_matcher(uri_matcher) {		
}

LocationBlock::~LocationBlock() {
}

bool	LocationBlock::matchURI(const std::string& uri) {
	return m_uri_matcher->match(uri);
}
