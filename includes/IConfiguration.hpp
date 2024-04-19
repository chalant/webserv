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

    virtual const std::vector<IBlock> getBlocks(const std::string &) const = 0;
    virtual const std::string getString(const std::string &) const = 0;
    virtual int getInt(const std::string &) const = 0;
    virtual bool getBool(const std::string &) const = 0;
};

#endif