#include "../mock_includes/MockRoute.hpp"

/*
 * MockRoute class
 *
 * This class represents a mock route
 *
 */

std::string MockRoute::getUri() const { return ""; }

void MockRoute::setUri(std::string newUri) { static_cast<void>(newUri);}

HttpMethod MockRoute::getMethod() const { return GET; }

void MockRoute::setMethod(HttpMethod newMethod) { static_cast<void>(newMethod); }

std::string MockRoute::getRoot() const {
    return this->_root;
}

std::string MockRoute::getPrefix() const {
    return this->_prefix;
}

void MockRoute::setRoot(std::string &root) {
    this->_root = root;
}

void MockRoute::setPrefix(std::string &prefix) {
    this->_prefix = prefix;
}

// Path: tests/mock_srcs/MockRoute.cpp
