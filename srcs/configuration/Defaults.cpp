#include "../../includes/configuration/Defaults.hpp"

Defaults::Defaults() {
	m_pairs["client_header_buffer_size"] = "1024";
	m_pairs["client_max_uri_size"] = "1024";
	m_pairs["prefix"] = "/"; //temporary, the parser should detect the type of prefix.
	m_pairs["listen"] = "127.0.0.1:8080";
	m_pairs["server_name"] = "default";
}

Defaults::~Defaults() {	
}

const std::string&	Defaults::operator[](const std::string& key) {
	return m_pairs[key];
}
