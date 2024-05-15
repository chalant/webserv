#if !defined(DIRECTIVES_HPP)
#define DIRECTIVES_HPP

#include "../../includes/utils/Pair.hpp";
#include "../../includes/configuration/DefaultConfigurationBlock";

#define CLIENT_HEADER_BUFFER_SIZE	Pair<std::string, int>("client_header_buffer_size", 1024);
#define CLIENT_MAX_URI_SIZE			Pair<std::string, int>("client_max_uri_size", 1024);
#define HTTP_BLOCK					Pair<std::string, ConfigurationBlock>("http", DefaultConfigurationBlock());

#endif // DIRECTIVES_HPP
