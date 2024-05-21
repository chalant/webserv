#include "../../includes/configuration/ConfigurationBlock.hpp"
#include "../../includes/utils/Converter.hpp"
#include <iomanip>
#include <iostream>

ConfigurationBlock::ConfigurationBlock(ILogger &logger, const std::string name,
                                       Defaults &defaults)
    : _logger(logger), _name(name), m_defaults(defaults), m_is_regex(false)
{
    this->push_back(this);
}

ConfigurationBlock::ConfigurationBlock(const ConfigurationBlock &parent,
                                       const std::string name,
                                       Defaults &defaults)
    : _logger(parent._logger), _name(name), m_defaults(defaults),
      m_is_regex(false)
{
    this->push_back(this);
}

ConfigurationBlock::~ConfigurationBlock()
{
    for (std::map<std::string, std::vector<std::string> *>::const_iterator it =
             _directives.begin();
         it != _directives.end(); ++it)
    {
        delete it->second;
    }
    for (std::map<std::string, BlockList>::iterator it = _blocks.begin();
         it != _blocks.end(); ++it)
    {
        for (size_t i = 0; i < it->second.size(); i++)
        {
            delete it->second[ i ];
        }
    }
}

const BlockList &ConfigurationBlock::getBlocks(const std::string &key)
{
    try
    {
        return _blocks.at(key);
    }
    catch (std::exception &e)
    {
        _logger.log(DEBUG,
                    "ConfigurationBlock::getBlocks: " + key + " not found");
		BlockList	*blk = &_blocks[ key ];
		blk->push_back(new ConfigurationBlock(*this, key, m_defaults));
		return *blk;
    }
}

const std::vector<std::string> &
ConfigurationBlock::getStringVector(const std::string &key) const
{
	try
	{
		return *_directives.at(key);
	}
	catch(const std::exception& e)
	{
        _logger.log(DEBUG, "ConfigurationBlock::getString: " + key +
                               " not found using default");
        return m_defaults.getDirectiveParameters(key);
	}
}

const std::string &ConfigurationBlock::getString(const std::string &key,
                                                 size_t index = 0) const
{
    try
    {
        return _directives.at(key)->at(index);
    }
    catch (std::exception &e)
    {
        const std::string &res = m_defaults.getDirectiveParameters(key)[ index ];
        _logger.log(DEBUG, "ConfigurationBlock::getString: " + key +
                               " not found defaulting to " + res);
        return res;
    }
}

int ConfigurationBlock::getInt(const std::string &key, size_t index = 0) const
{
    try
    {
        return Converter::toInt(_directives.at(key)->at(index));
    }
    catch (const std::out_of_range &e)
    {
        _logger.log(DEBUG, "ConfigurationBlock::getInt: " + key + " not found");
    }
    catch (const std::invalid_argument &e)
    {
        _logger.log(DEBUG, "ConfigurationBlock::getInt: " + key + " " +
                               _directives.at(key)->at(index) +
                               " not an integer");
    }
    return Converter::toInt(m_defaults.getDirectiveParameters( key )[ index ]);
}

size_t ConfigurationBlock::getSize_t(const std::string &key,
                                     size_t index = 0) const
{
    try
    {
        return Converter::toUInt(_directives.at(key)->at(index));
    }
    catch (const std::out_of_range &e)
    {
        _logger.log(DEBUG,
                    "ConfigurationBlock::getSize_t: " + key + " not found");
    }
    catch (const std::invalid_argument &e)
    {
        _logger.log(DEBUG, "ConfigurationBlock::getSize_t: " + key + " " +
                               _directives.at(key)->at(index) +
                               " not an unsigned long");
    }
    return Converter::toUInt(m_defaults.getDirectiveParameters(key)[ index ]);
}

bool ConfigurationBlock::getBool(const std::string &key, size_t index = 0) const
{
    try
    {
        std::string &value = _directives.at(key)->at(index);
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
        _logger.log(DEBUG,
                    "ConfigurationBlock::getBool: " + key + " not found");
    }
    catch (const std::invalid_argument &e)
    {
        _logger.log(DEBUG, "ConfigurationBlock::getBool: " + key + " " +
                               _directives.at(key)->at(index) + " not a bool");
    }
    return false;
}

void ConfigurationBlock::addBlock(const std::string &name,
                                  IConfiguration *block)
{
    BlockList *blc = &_blocks[ name ];
    blc->push_back(block);
    blc->setLogger(&_logger);
    blc->setParent(this);
}

std::vector<std::string> &
ConfigurationBlock::addDirective(const std::string &name)
{
    if (_directives.find(name) == _directives.end())
    {
        _directives[ name ] = new std::vector<std::string>();
    }
    return *_directives[ name ];
}

void ConfigurationBlock::print(size_t depth = 0) const
{
    for (std::map<std::string, std::vector<std::string> *>::const_iterator it =
             _directives.begin();
         it != _directives.end(); ++it)
    {
        std::cout << std::setw(depth + 2) << "• " << it->first << " ";
        for (size_t i = 0; i < it->second->size(); i++)
        {
            std::cout << (*it->second)[ i ] << " ";
        }
        std::cout << std::endl;
    }
    for (std::map<std::string, BlockList>::const_iterator it = _blocks.begin();
         it != _blocks.end(); ++it)
    {
        for (size_t i = 0; i < it->second.size(); i++)
        {
            std::cout << std::setw(depth + 2) << "• " << it->first << " ";
            std::vector<std::string> &params = it->second[ i ]->getParameters();
            for (size_t j = 0; j < params.size(); j++)
            {
                std::cout << params[ j ] << " ";
            }
            std::cout << std::endl;
            it->second[ i ]->print(depth + 4);
        }
        std::cout << std::endl;
    }
}

std::vector<std::string>&
ConfigurationBlock::getParameters(void) 
{
	if (_parameters.size() == 0)
	{
		return m_defaults.getBlockParameters(this->_name);
	}
	return	_parameters;
}

std::vector<std::string>&
ConfigurationBlock::setParameters(void) 
{
    return _parameters;
}

const std::string &ConfigurationBlock::getName(void) const { return _name; }

bool ConfigurationBlock::isRegex(void) const { return m_is_regex; }
void ConfigurationBlock::isRegex(bool value) { m_is_regex = value; };
