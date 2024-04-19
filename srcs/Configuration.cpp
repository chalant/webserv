#include "../includes/Configuration.hpp"

/*'Configuration': Verifies, reads, parses and stores the config 
file data. This data is used to initiate the 'Server' and the 'Router'*/

Configuration::Configuration()
{
}

Configuration::~Configuration()
{
}

const std::vector<Block> Configuration::getBlocks(const std::string &parameter) const
{
    return _blocks.at(parameter);
}

const std::string Configuration::getString(const std::string &parameter) const
{
    return _strings.at(parameter);
}

int Configuration::getInt(const std::string &parameter) const
{
    return _ints.at(parameter);
}

bool Configuration::getBool(const std::string &parameter) const
{
    return _bools.at(parameter);
}