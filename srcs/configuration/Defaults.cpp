#include "../../includes/configuration/Defaults.hpp"

Defaults::Defaults()
{
    m_directive_parameters[ "client_header_buffer_size" ].push_back("1024");
    m_directive_parameters[ "client_max_uri_size" ].push_back("1024");
    m_directive_parameters[ "client_body_buffer_size" ].push_back("1024");
    m_directive_parameters[ "client_max_body_size" ].push_back("1024");
    // //m_directive_parameters[ "prefix" ] =
    //     "/"; // temporary, the parser should detect the type of prefix.
    m_directive_parameters[ "listen" ].push_back("127.0.0.1:8080");
    m_directive_parameters[ "server_name" ].push_back("default");

	m_directive_parameters[ "error_log" ].push_back("/"); //default path ?
    m_directive_parameters[ "error_log" ].push_back("debug");

	m_directive_parameters[ "deny "].push_back("all");

	m_block_parameters[ "location "].push_back("/");
	m_block_parameters[ "limit_except" ].push_back("GET");
}

Defaults::~Defaults() {}

std::vector<std::string>	&Defaults::getBlockParameters(const std::string& key)
{
	return m_block_parameters[ key ];
}

std::vector<std::string>	&Defaults::getDirectiveParameters(const std::string& key)
{
	return m_directive_parameters[ key ];
}

// Path: srcs/configuration/Defaults.cpp
