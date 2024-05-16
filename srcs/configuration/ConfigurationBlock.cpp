#include "../../includes/configuration/ConfigurationBlock.hpp"
#include "../../includes/utils/Converter.hpp"
#include <iostream>
#include <iomanip>

ConfigurationBlock::ConfigurationBlock(ILogger& logger, const std::string name, Defaults defaults) : 
    _logger(logger), _name(name), m_defaults(defaults)
{
}

ConfigurationBlock::ConfigurationBlock(const ConfigurationBlock& parent, const std::string name, Defaults defaults): 
    _logger(parent._logger), _name(name), m_defaults(defaults)
{
}

ConfigurationBlock::~ConfigurationBlock() {
	for (std::map<std::string, std::vector<std::string> *>::const_iterator it = _directives.begin(); it != _directives.end(); ++it) {
		delete it->second;
	}
	for (std::map<std::string, std::vector<IConfiguration *> >::iterator it = _blocks.begin(); it != _blocks.end(); ++it) {
		for (size_t i = 0; i < it->second.size(); i++) {
			delete it->second[i];
		}
	}
}

const std::vector<IConfiguration *>&	ConfigurationBlock::getBlocks(const std::string &key) const
{
	try
	{
    	return _blocks.at(key);
	}
	catch (std::exception& e)
	{
		return *this;
	} 
}

const std::vector<std::string>& ConfigurationBlock::getStringVector(const std::string &key) const
{
	return *_directives.at(key);
}

const std::string&	ConfigurationBlock::getString(const std::string &key, size_t index = 0) const
{
	try 
	{
		return _directives.at(key)->at(index);
	}
	catch (std::exception& e)
	{
		const std::string&	res = m_defaults[key];
		_logger.log(DEBUG, "ConfigurationBlock::getString: " + key + " not found defaulting to " + res);
		return res;	
	}
}

int ConfigurationBlock::getInt(const std::string &key, size_t index = 0) const
{
    try {
        return Converter::toInt(_directives.at(key)->at(index));
    } 
	catch (const std::out_of_range &e) 
	{
        _logger.log(DEBUG, "ConfigurationBlock::getInt: " + key + " not found");
    }
	catch (const std::invalid_argument& e) {
		_logger.log(DEBUG, "ConfigurationBlock::getInt: " + key + " " + _directives.at(key)->at(index) + " not an integer");
	}
    return Converter::toInt(m_defaults[key]);
}

size_t ConfigurationBlock::getSize_t(const std::string &key, size_t index = 0) const
{
    try {
		return Converter::toUInt(_directives.at(key)->at(index));
	} 
	catch (const std::out_of_range &e) {
        _logger.log(DEBUG, "ConfigurationBlock::getSize_t: " + key + " not found");
    }
	catch (const std::invalid_argument& e) {
		_logger.log(DEBUG, "ConfigurationBlock::getSize_t: " + key + " " + _directives.at(key)->at(index) + " not an unsigned long");
	}
    return Converter::toUInt(m_defaults[key]);
}

bool ConfigurationBlock::getBool(const std::string &key, size_t index = 0) const
{
    try {
		std::string&	value = _directives.at(key)->at(index);
		if (value == "on")
			return true;
		else if (value == "off")
			return false;
		else 
		{
			throw std::invalid_argument("not a bool");
		}
    }
	catch (const std::out_of_range &e) 
	{
        _logger.log(DEBUG, "ConfigurationBlock::getBool: " + key + " not found");
    }
	catch (const std::invalid_argument& e) 
	{
		_logger.log(DEBUG, "ConfigurationBlock::getBool: " + key + " " + _directives.at(key)->at(index) + " not a bool");
	}
    return false;
}

void	ConfigurationBlock::addBlock(const std::string& name, IConfiguration *block) {
	_blocks[name].push_back(block);
}

std::vector<std::string>&	ConfigurationBlock::addDirective(const std::string& name) {
	if (_directives.find(name) == _directives.end())
	{
		_directives[name] = new std::vector<std::string>();
	}
	return *_directives[name];
}

void	ConfigurationBlock::print(size_t depth = 0) const {
	for (std::map<std::string, std::vector<std::string> *>::const_iterator it = _directives.begin(); it != _directives.end(); ++it) {
		std::cout << std::setw(depth + 2) << "• " << it->first << " ";
		for (size_t i = 0; i < it->second->size(); i++) 
		{
			std::cout << (*it->second)[i] << " ";
		}
		std::cout << std::endl;
	}
	for (std::map<std::string, std::vector<IConfiguration *> >::const_iterator it = _blocks.begin(); it != _blocks.end(); ++it) {
		for (size_t i = 0; i < it->second.size(); i++) 
		{
			std::cout << std::setw(depth + 2) << "• " << it->first << " " << std::endl;
			it->second[i]->print(depth + 4);
		}
		std::cout << std::endl;
	}
}

const std::string&	ConfigurationBlock::getName(void) const {
	return _name;
}

bool	ConfigurationBlock::isRegex(void) const {
	return false;
}

IConfiguration*	ConfigurationBlock::operator[](size_t index) {
	static_cast<void>(index);
	return this;
}
