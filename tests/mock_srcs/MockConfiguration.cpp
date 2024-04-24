#include "MockConfiguration.hpp"

/*
 * MockConfiguration class is a mock implementation of the IConfiguration interface
 * This mock implementation is used for testing purposes to simulate the behavior of the IConfiguration interface
 * during unit testing without relying on the actual implementation.
 */

// Constructor
MockConfiguration::MockConfiguration() 
    : _blocks(), 
      _strings(), 
      _stringVectors(), 
      _ints(), 
      _size_ts(), 
      _bools() 
{
}
#include <iostream>

// Destructor
MockConfiguration::~MockConfiguration(){};

// TEST methods
void MockConfiguration::setString(const std::string &parameter, const std::string &value)
{
    this->_strings[parameter] = value;
}

void MockConfiguration::setSize_t(const std::string &parameter, size_t value)
{
    this->_size_ts[parameter] = value;
}

void MockConfiguration::setStringVector(const std::string &parameter, const std::vector<std::string> &value)
{
    this->_stringVectors[parameter] = value;
}

// Setters
void MockConfiguration::setInt(const std::string &parameter, int value)
{
    this->_ints[parameter] = value;
}

// Getters
const std::vector<IBlock *> MockConfiguration::getBlocks(const std::string &parameter) const
{
    try {
        return (_blocks.at(parameter));
    } catch (const std::out_of_range &e){
        throw std::out_of_range("Block Key not found " + parameter);
    }
    return  std::vector<IBlock *>();
}

const std::vector<std::string> MockConfiguration::getStringVector(const std::string &parameter) const
{
   try{
        return (_stringVectors.at(parameter));
    } catch (const std::out_of_range &e){
        throw std::out_of_range("String Vector Key not found " + parameter);
    }
    return std::vector<std::string>();
}

const std::string MockConfiguration::getString(const std::string &parameter) const
{
    try{
        return (_strings.at(parameter));
    } catch (const std::out_of_range &e){
        throw std::out_of_range("String Key not found " + parameter);
    }
    return "";
}

int MockConfiguration::getInt(const std::string &parameter) const
{
    try{
        return (_ints.at(parameter));
    } catch (const std::out_of_range &e){
        throw std::out_of_range("Int Key not found " + parameter);
    }
    return -1;
}

size_t MockConfiguration::getSize_t(const std::string &parameter) const
{
    try{
        return (_size_ts.at(parameter));
    } catch (const std::out_of_range &e){
        throw std::out_of_range("Size_t Key not found " + parameter);
    }
    return -1;
}

bool MockConfiguration::getBool(const std::string &parameter) const
{
    try{
        return (_bools.at(parameter));
    } catch (const std::out_of_range &e){
        throw std::out_of_range("Bool Key not found " + parameter);
    }
    return false;
}

// Path: tests/mock_headers/MockLogger.hpp
