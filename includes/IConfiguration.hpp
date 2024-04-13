#ifndef IConfiguration_HPP
#define IConfiguration_HPP

/*
 * File: IConfiguration.hpp
 * ------------------------
 * This file defines the IIConfiguration interface, which serves as the abstract base
 * class for IConfigurations in webserv. IIConfiguration provides a contract
 * for classes that represent different IConfigurations, allowing for polymorphic behavior
 * and dependency injection.
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