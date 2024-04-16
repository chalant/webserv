#ifndef IConfiguration_HPP
#define IConfiguration_HPP

/*
 * IConfiguration.hpp
 * Abstract base class for Configuration in webserv
 *
 * This file defines the Configuration interface, which serves as the abstract base
 * class for the Configuration in webserv. 
 * IConfiguration allows for polymorphic behavior and dependency injection,
 * thereby enabling us to create a MockConfiguration class for isolated unit testing.
 * 
 */

#include <set>
#include <string>
#include "constants/LogLevelHelper.hpp"

class IConfiguration
{
public:
    virtual ~IConfiguration() {}

    // Getters
    virtual size_t getClientHeaderBufferSize() const = 0;
    virtual size_t getClientBodyBufferSize() const = 0;
    virtual size_t getClientMaxBodySize() const = 0;
    virtual size_t getClientMaxUriSize() const = 0;
    virtual size_t getLogBufferSize() const = 0;
    virtual size_t getMaxConnections() const = 0;
    virtual int getPort() const = 0;
    virtual LogLevel getLogLevel() const = 0;
    virtual bool getErrorLogEnabled() const = 0;
    virtual bool getAccessLogEnabled() const = 0;
    virtual std::string getErrorLogFile() const = 0;
    virtual std::string getAccessLogFile() const = 0;
    virtual std::set<std::string> getSupportedMethods() const = 0;
};

#endif