#include "../mock_includes/MockConfigurationBlock.hpp"
#include <stdexcept>

/*
 * MockConfigurationBlock class is a mock implementation of the IConfiguration interface
 * This mock implementation is used for testing purposes to simulate the behavior of the IConfiguration interface
 * during unit testing without relying on the actual implementation.
 */

// Constructor
MockConfigurationBlock::MockConfigurationBlock(ILogger &logger, const std::string name)
    : _blocks(),
      _directives(),
      _logger(logger),
      _name(name)
{
}

// Destructor
MockConfigurationBlock::~MockConfigurationBlock()
{
    for (std::map<std::string, std::vector<std::string> *>::const_iterator it = _directives.begin(); it != _directives.end(); ++it)
    {
        delete it->second;
    }
};

// TEST methods
void MockConfigurationBlock::setBlock(const std::string &parameter, IConfiguration *block)
{
    this->_logger.log(DEBUG, "Setting Block '" + parameter + "' to '" + block->getName() + "'");

    // Add the block to the BlockList
    this->_blocks[parameter].push_back(block);
}

void MockConfigurationBlock::setString(const std::string &parameter, const std::string &value)
{
    this->_logger.log(DEBUG, "Setting Directive '" + parameter + "' to " + value + "'");

    // Check if the parameter already exists
    if (_directives.find(parameter) != _directives.end())
    {
        // If it exists, add the value to the existing vector
        _directives[parameter]->push_back(value);
    }
    else
    {
        // If it doesn't exist, create a new vector with one element
        std::vector<std::string> *vec = new std::vector<std::string>;
        vec->push_back(value);
        _directives[parameter] = vec;
    }
}

void MockConfigurationBlock::setSize_t(const std::string &parameter, size_t value)
{
    this->setString(parameter, std::to_string(value));
}

void MockConfigurationBlock::setStringVector(const std::string &parameter, const std::vector<std::string> &value)
{
    for (size_t i = 0; i < value.size(); i++)
    {
        this->setString(parameter, value[i]);
    }
}

// Setters
void MockConfigurationBlock::setInt(const std::string &parameter, int value)
{
    this->setString(parameter, std::to_string(value));
}

// Getters
const BlockList &MockConfigurationBlock::getBlocks(const std::string &parameter) const
{
    this->_logger.log(DEBUG, "Getting Block '" + parameter + "'");
    try
    {
        return (_blocks.at(parameter));
    }
    catch (const std::out_of_range &e)
    {
        throw std::out_of_range("Block Key not found " + parameter);
    }
}

const std::vector<std::string> &MockConfigurationBlock::getStringVector(const std::string &parameter) const
{
    try
    {
        return (*_directives.at(parameter));
    }
    catch (const std::out_of_range &e)
    {
        throw std::out_of_range("String Vector Key not found " + parameter);
    }
}

const std::string &MockConfigurationBlock::getString(const std::string &key, size_t index) const
{
    this->_logger.log(DEBUG, "Getting Directive '" + key + "' at index " + std::to_string(index) + "'");
    return _directives.at(key)->at(index);
}

int MockConfigurationBlock::getInt(const std::string &parameter, size_t index) const
{
    try
    {
        return (std::stoi(_directives.at(parameter)->at(index)));
    }
    catch (const std::out_of_range &e)
    {
        throw std::out_of_range("Int Key not found " + parameter);
    }
    catch (const std::invalid_argument &e)
    {
        throw std::invalid_argument("Int Value not found " + parameter);
    }
    return -1;
}

size_t MockConfigurationBlock::getSize_t(const std::string &parameter, size_t index) const
{
    try
    {
        return (std::stoul(_directives.at(parameter)->at(index)));
    }
    catch (const std::out_of_range &e)
    {
        throw std::out_of_range("Size_t Key not found " + parameter);
    }
    return 0;
}

bool MockConfigurationBlock::getBool(const std::string &parameter, size_t index) const
{
    (void)parameter;
    (void)index;
    return false;
}

void addBlock(const std::string &name, IConfiguration *block)
{
    (void)name;
    (void)block;
}

void addDirective(const std::string &name, std::vector<std::string> *parameters)
{
    (void)name;
    (void)parameters;
}

void MockConfigurationBlock::print(size_t depth) const
{
    static_cast<void>(depth);
}

const std::string &MockConfigurationBlock::getName() const
{
    return _name;
}

bool MockConfigurationBlock::isRegex(void) const
{
    return false;
}

// Path: tests/mock_headers/MockLogger.hpp
