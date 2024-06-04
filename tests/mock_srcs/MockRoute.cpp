#include "../mock_includes/MockRoute.hpp"

/*
 * MockRoute class
 *
 * This class represents a mock route
 *
 */

MockRoute::MockRoute() {}

MockRoute::~MockRoute() {}

std::string MockRoute::getRoot() const { return m_root; }

std::string MockRoute::getPath() const { return m_path; }

std::string MockRoute::getIndex() const { return ""; }

std::string MockRoute::getCgiScript() const { return m_cgi_script; }

void MockRoute::setRoot(std::string &root) { m_root = root; }

void MockRoute::setPath(std::string &prefix) { m_path = prefix; }

void MockRoute::setCgiScript(std::string &cgi_script) { m_cgi_script = cgi_script; }

bool MockRoute::isAllowedMethod(const HttpMethod method) const { 
    static_cast<void>(method);
    return true; }

bool MockRoute::isRegex() const { return false; }

IResponseGenerator *MockRoute::getResponseGenerator() const { return NULL; }
void	MockRoute::setResponseGenerator(IResponseGenerator *generator) { static_cast<void>(generator); }

// Path: tests/mock_srcs/MockRoute.cpp
