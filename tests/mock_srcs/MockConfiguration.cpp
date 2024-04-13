#include "../mock_includes/MockConfiguration.hpp"

/*
 * MockConfiguration class is a mock implementation of the IConfiguration interface
 * This mock implementation is used for testing purposes to simulate the behavior of the IConfiguration interface
 * during unit testing without relying on the actual implementation.
*/

// Constructor
MockConfiguration::MockConfiguration() : IConfiguration(){};

// Destructor
MockConfiguration::~MockConfiguration(){};

// Getters
size_t MockConfiguration::getClientHeaderBufferSize() const
{
    return 32768;
}

size_t MockConfiguration::getClientBodyBufferSize() const
{
    return 32768;
}

size_t MockConfiguration::getClientMaxBodySize() const
{
    return 10485760;
}

size_t MockConfiguration::getClientMaxUriSize() const
{
    return 4096;
}

size_t MockConfiguration::getLogBufferSize() const
{
    return 4096;
}

size_t MockConfiguration::getMaxConnections() const
{
    return 1000;
}

int MockConfiguration::getPort() const
{
    return 8080;
}

LogLevel MockConfiguration::getLogLevel() const
{
    return INFO;
}

bool MockConfiguration::getErrorLogEnabled() const
{
    return false;
}

bool MockConfiguration::getAccessLogEnabled() const
{
    return false;
}

std::string MockConfiguration::getErrorLogFile() const
{
    return "";
}

std::string MockConfiguration::getAccessLogFile() const
{
    return "";
}

std::set<std::string> MockConfiguration::getSupportedMethods() const
{
    return std::set<std::string>();
}

// Path: tests/mock_headers/MockLogger.hpp
