#include "../../includes/configuration/Defaults.hpp"

Defaults::Defaults()
{
    m_directive_parameters[ "client_header_buffer_size" ].push_back("1024");
    m_directive_parameters[ "client_max_uri_size" ].push_back("1024");
    m_directive_parameters[ "client_body_buffer_size" ].push_back("1024");
    m_directive_parameters[ "client_max_body_size" ].push_back("1024");
    m_directive_parameters[ "listen" ].push_back("8080");
    m_directive_parameters[ "server_name" ].push_back("default");
    m_directive_parameters[ "error_log" ].push_back("/logs/error.log");
    m_directive_parameters[ "error_log" ].push_back("verbose");
    m_directive_parameters[ "access_log" ].push_back("/logs/access.log");
    m_directive_parameters[ "deny" ].push_back("all");
    m_block_parameters[ "location" ].push_back("/");
    m_block_parameters[ "limit_except" ].push_back("GET");
	m_block_parameters[ "limit_except" ].push_back("POST");
    m_directive_parameters[ "root" ].push_back("sample_site");
    m_directive_parameters[ "index" ].push_back("index.html");
    m_directive_parameters[ "path" ].push_back("/"); // temp for testing
}

Defaults::~Defaults() {}

std::vector<std::string> &Defaults::getBlockParameters(const std::string &key)
{
    return m_block_parameters[ key ];
}

std::vector<std::string> &
Defaults::getDirectiveParameters(const std::string &key)
{
    return m_directive_parameters[ key ];
}

std::string Defaults::getDirectiveParameter(const std::string &key, int index)
{
    std::vector<std::string> &vec = m_directive_parameters[ key ];
    return vec[ index ];
}

// Path: srcs/configuration/Defaults.cpp
