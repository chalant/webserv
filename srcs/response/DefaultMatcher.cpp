#include "response/DefaultMatcher.hpp"

DefaultMatcher::DefaultMatcher(const std::string& path): m_path(path) {
}

DefaultMatcher::~DefaultMatcher() {
}

//todo: provide implementation
bool	DefaultMatcher::match(const std::string& uri) {
	(void)uri;
	return false;
}