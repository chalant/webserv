#ifndef IConfiguration_HPP
#define IConfiguration_HPP

/*
 * IConfiguration.hpp
 * Abstract base class for Configuration in webserv
 *
 * This file defines the Configuration interface, which serves as the abstract base
 * class for the Configuration in webserv.
 * IConfiguration allows for polymorphic behavior and dependency injection,
 * thereby enabling us to create a MockConfigurationBlock class for isolated unit testing.
 *
 */

#include <set>
#include <string>
#include "../../includes/constants/LogLevelHelper.hpp"

class IConfiguration
{
public:
    virtual ~IConfiguration() {}

    virtual const std::vector<IConfiguration *>&	getBlocks(const std::string &key) const = 0;
    virtual const std::vector<std::string>&	getStringVector(const std::string &) const = 0;
    virtual const std::string&	getString(const std::string &parameter, size_t index = 0) const = 0;
    virtual int		getInt(const std::string &parameter, size_t index = 0) const = 0;
    virtual size_t	getSize_t(const std::string &, size_t index = 0) const = 0;
    virtual bool	getBool(const std::string &parameter, size_t index = 0) const = 0;
	virtual void	print(size_t depth) const = 0;
	virtual const std::string&	getName() const = 0;
	virtual bool	matchURI(const std::string& uri) = 0;
};

#endif // IConfiguration_HPP
// Path: includes/configuration/IConfiguration.hpp
