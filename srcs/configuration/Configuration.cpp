#include "configuration/Configuration.hpp"

/*'Configuration': Verifies, reads, parses and stores the config 
file data. This data is used to initiate the 'Server' and the 'Router'*/

Configuration::Configuration(ILogger &logger) : 
    _logger(logger)
{
}

Configuration::~Configuration()
{
}

const std::vector<IBlock *> Configuration::getBlocks(const std::string &parameter) const
{
    try{
        return _blocks.at(parameter);
    } catch (const std::out_of_range &e){
        _logger.log(DEBUG, "Configuration::getBlocks: " + parameter + " not found");
    }
    return std::vector<IBlock *>();
}

const std::vector<std::string> Configuration::getStringVector(const std::string &parameter) const
{
    try{
        return _stringVectors.at(parameter);
    } catch (const std::out_of_range &e){
        _logger.log(DEBUG, "Configuration::getStringVector: " + parameter + " not found");
    }
    return std::vector<std::string>();
}

const std::string Configuration::getString(const std::string &parameter) const
{
    try{
        return _strings.at(parameter);
    } catch (const std::out_of_range &e){
        _logger.log(DEBUG, "Configuration::getString: " + parameter + " not found");
    }
    return std::string();
}

int Configuration::getInt(const std::string &parameter) const
{
    try{
        return _ints.at(parameter);
    } catch (const std::out_of_range &e){
        _logger.log(DEBUG, "Configuration::getInt: " + parameter + " not found");
    }
    return -1;
}

size_t Configuration::getSize_t(const std::string &parameter) const
{
    try {
        return _size_ts.at(parameter);
    } catch (const std::out_of_range &e){
        _logger.log(DEBUG, "Configuration::getSize_t: " + parameter + " not found");
    }
    return -1;
}

bool Configuration::getBool(const std::string &parameter) const
{
    try {
        return _bools.at(parameter);
    } catch (const std::out_of_range &e){
        _logger.log(DEBUG, "Configuration::getBool: " + parameter + " not found");
    }
    return false;
}