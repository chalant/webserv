#include "../../includes/configuration/ConfigurationBlock.hpp"
#include <iostream>
#include <iomanip>

ConfigurationBlock::ConfigurationBlock(ILogger& logger, const std::string name) : 
    _logger(logger), _name(name)
{
}

ConfigurationBlock::ConfigurationBlock(const ConfigurationBlock& parent, const std::string name): 
    _logger(parent._logger), _name(name)
{
}

ConfigurationBlock::~ConfigurationBlock() {
	for (std::map<std::string, std::vector<std::string> *>::const_iterator it = _directives.begin(); it != _directives.end(); ++it) {
		delete it->second;
	}
}

const std::vector<IConfiguration *>&	ConfigurationBlock::getBlocks(const std::string &key) const
{
    return _blocks.at(key);
}

const std::vector<std::string>& ConfigurationBlock::getStringVector(const std::string &key) const
{
	return *_directives.at(key);
}

const std::string&	ConfigurationBlock::getString(const std::string &key, size_t index = 0) const
{
	return _directives.at(key)->at(index);
}

int ConfigurationBlock::getInt(const std::string &key, size_t index = 0) const
{
    try {
        return std::stoi(_directives.at(key)->at(index));
    } catch (const std::out_of_range &e) {
        _logger.log(DEBUG, "ConfigurationBlock::getInt: " + key + " not found");
    }
	catch (const std::invalid_argument& e) {
		_logger.log(DEBUG, "ConfigurationBlock::getInt: " + key + " " + _directives.at(key)->at(index) + " not an integer");
	}
    return -1;
}

size_t ConfigurationBlock::getSize_t(const std::string &key, size_t index = 0) const
{
    try {
		return std::stoul(_directives.at(key)->at(index));
	} 
	catch (const std::out_of_range &e) {
        _logger.log(DEBUG, "ConfigurationBlock::getSize_t: " + key + " not found");
    }
	catch (const std::invalid_argument& e) {
		_logger.log(DEBUG, "ConfigurationBlock::getSize_t: " + key + " " + _directives.at(key)->at(index) + " not an unsigned long");
	}
    return -1;
}

bool ConfigurationBlock::getBool(const std::string &key, size_t index = 0) const
{
    try {
		std::string&	value = _directives.at(key)->at(index);
		if (value == "on")
			return true;
		else if (value == "off")
			return false;
		else {
			throw std::invalid_argument("not a bool");
		}
    }
	catch (const std::out_of_range &e) {
        _logger.log(DEBUG, "ConfigurationBlock::getBool: " + key + " not found");
    }
	catch (const std::invalid_argument& e) {
		_logger.log(DEBUG, "ConfigurationBlock::getBool: " + key + " " + _directives.at(key)->at(index) + " not a bool");
	}
    return false;
}

void	ConfigurationBlock::addBlock(const std::string& name, IConfiguration *block) {
	_blocks[name].push_back(block);
}

//todo: check if it exists first.
void	ConfigurationBlock::addDirective(const std::string& name, std::vector<std::string> *parameters) {
	_directives[name] = parameters;
}

void	ConfigurationBlock::print(size_t depth = 0) const {
	for (std::map<std::string, std::vector<std::string> *>::const_iterator it = _directives.begin(); it != _directives.end(); ++it) {
		std::cout << std::setw(depth) << "" << it->first << " ";
		for (size_t i = 0; i < it->second->size(); i++) {
			std::cout << (*it->second)[i] << " ";
		}
		std::cout << std::endl;
	}
	for (std::map<std::string, std::vector<IConfiguration *> >::const_iterator it = _blocks.begin(); it != _blocks.end(); ++it) {
		for (size_t i = 0; i < it->second.size(); i++) {
			std::cout << std::setw(depth) << "" << it->first << " " << std::endl;
			it->second[i]->print(depth + 4);
		}
		std::cout << std::endl;
	}
}

const std::string&	ConfigurationBlock::getName(void) const {
	return _name;
}

bool	ConfigurationBlock::matchURI(const std::string& uri) {
	(void)uri;
	return false;
}
