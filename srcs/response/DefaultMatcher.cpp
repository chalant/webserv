#include "../../includes/response/DefaultMatcher.hpp"

DefaultMatcher::DefaultMatcher(const std::string &path) : m_path(path)
{
}

DefaultMatcher::~DefaultMatcher()
{
}

// todo: provide implementation
bool DefaultMatcher::match(const std::string &uri)
{
	(void)uri;
	(void)this->m_path; // remove this line when implementing the method.
	return false;
}