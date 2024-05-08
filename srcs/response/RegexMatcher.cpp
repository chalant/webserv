#include "response/RegexMatcher.hpp"

RegexMatcher::RegexMatcher(const std::string& pattern): 
	m_pattern(pattern) {
}

RegexMatcher::~RegexMatcher() {
}

//todo: provide implementation
bool	RegexMatcher::match(const std::string& uri) {
	(void)uri;
	return false;
}