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

void MockRoute::setRoot(std::string &root) { m_root = root; }

void MockRoute::setPath(std::string &prefix) { m_path = prefix; }

IResponseGenerator *MockRoute::getResponseGenerator() const { return NULL; }

// Path: tests/mock_srcs/MockRoute.cpp
