#include "../mock_includes/MockRoute.hpp"

/*
 * MockRoute class
 *
 * This class represents a mock route
 *
 */

MockRoute::MockRoute() {}

MockRoute::~MockRoute() {}

std::string MockRoute::getRoot() const { return this->_root; }

std::string MockRoute::getPath() const { return this->_path; }

void MockRoute::setRoot(std::string &root) { this->_root = root; }

void MockRoute::setPath(std::string &prefix) { this->_path = prefix; }

IResponseGenerator *MockRoute::getResponseGenerator() const { return NULL; }

// Path: tests/mock_srcs/MockRoute.cpp
