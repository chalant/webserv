#include "configuration/Block.hpp"
#include <iostream>
#include <iomanip>

Block::Block(ILogger& logger, const std::string name) : 
    _logger(logger), _name(name)
{
}

Block::Block(const Block& parent, const std::string name): 
    _logger(parent._logger), _name(name)
{
}

Block::~Block() {
	for (std::map<std::string, std::vector<std::string> *>::const_iterator it = _directives.begin(); it != _directives.end(); ++it) {
		delete it->second;
	}
}

const std::vector<IBlock*>&	Block::getBlocks(const std::string &key) const
{
    try {
        return _blocks.at(key);
    }
	catch (const std::out_of_range &e){
        _logger.log(DEBUG, "Block::getBlocks: " + key + " not found");
    }
    //return std::vector<IBlock *>();
	throw std::out_of_range("");
}

const std::vector<std::string>& Block::getStringVector(const std::string &key) const
{
    try {
        return *_directives.at(key);
    } 
	catch (const std::out_of_range &e) {
        _logger.log(DEBUG, "Block::getStringVector: " + key + " not found");
	}
	throw std::out_of_range("");
}

const std::string Block::getString(const std::string &key) const
{
    try {
		return _directives.at(key)->at(0);
    } 
	catch (const std::out_of_range &e){
        _logger.log(DEBUG, "Block::getString: " + key + " not found");
    }
    return "";
}

int Block::getInt(const std::string &key, size_t index) const
{
    try {
        return std::stoi(_directives.at(key)->at(index));
    } catch (const std::out_of_range &e) {
        _logger.log(DEBUG, "Block::getInt: " + key + " not found");
    }
	catch (const std::invalid_argument& e) {
		_logger.log(DEBUG, "Block::getInt: " + key + " " + _directives.at(key)->at(index) + " not an integer");
	}
    return -1;
}

size_t Block::getSize_t(const std::string &key, size_t index) const
{
    try {
		return std::stoul(_directives.at(key)->at(index));
	} 
	catch (const std::out_of_range &e) {
        _logger.log(DEBUG, "Block::getSize_t: " + key + " not found");
    }
	catch (const std::invalid_argument& e) {
		_logger.log(DEBUG, "Block::getSize_t: " + key + " " + _directives.at(key)->at(index) + " not an unsigned long");
	}
    return -1;
}

bool Block::getBool(const std::string &key, size_t index) const
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
        _logger.log(DEBUG, "Block::getBool: " + key + " not found");
    }
	catch (const std::invalid_argument& e) {
		_logger.log(DEBUG, "Block::getBool: " + key + " " + _directives.at(key)->at(index) + " not a bool");
	}
    return false;
}

void	Block::addBlock(const std::string& name, IBlock	*block) {
	_blocks[name].push_back(block);
}

//todo: check if it exists first.
void	Block::addDirective(const std::string& name, std::vector<std::string> *parameters) {
	_directives[name] = parameters;
}

void	Block::print(size_t depth = 0) const {
	for (std::map<std::string, std::vector<std::string> *>::const_iterator it = _directives.begin(); it != _directives.end(); ++it) {
		std::cout << std::setw(depth) << "" << it->first << " ";
		for (size_t i = 0; i < it->second->size(); i++) {
			std::cout << (*it->second)[i] << " ";
		}
		std::cout << std::endl;
	}
	//std::cout << "BLOCK SIZE " << _blocks.size() << std::endl;
	for (std::map<std::string, std::vector<IBlock *>>::const_iterator it = _blocks.begin(); it != _blocks.end(); ++it) {
		for (size_t i = 0; i < it->second.size(); i++) {
			std::cout << std::setw(depth) << "" << it->first << " " << std::endl;
			//std::cout << std::setw(depth) << "PRINTING " << it->second[i]->getName() << std::endl;
			it->second[i]->print(depth + 4);
		}
		std::cout << std::endl;
	}
}

const std::string&	Block::getName(void) const {
	return _name;
}

// Path: srcs/Block.cpp
