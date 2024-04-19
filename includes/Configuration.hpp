/*'Configuration': Verifies, reads, parses and stores the config
file data. This data is used to initiate the 'Server' and the 'Router'*/

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "IConfiguration.hpp"
#include "IBlock.hpp"
#include <string>
#include <map>
#include <vector>

class Configuration : public IConfiguration
{
private:
	std::map<std::string, std::vector<IBlock>> _blocks;
	std::map<std::string, std::string> _strings;
	std::map<std::string, int> _ints;
	std::map<std::string, bool> _bools;

public:
	virtual const std::vector<IBlock> getBlocks(const std::string &parameter) const;
	virtual const std::string getString(const std::string &parameter) const;
	virtual int getInt(const std::string &parameter) const;
	virtual bool getBool(const std::string &parameter) const;
};

#endif
