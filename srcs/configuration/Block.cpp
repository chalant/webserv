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
}

const std::vector<IBlock*> Block::getBlocks(const std::string &key) const
{
    try{
        return _blocks.at(key);
    } catch (const std::out_of_range &e){
        _logger.log(DEBUG, "Block::getBlocks: " + key + " not found");
    }
    return std::vector<IBlock *>();
}

const std::vector<std::string> Block::getStringVector(const std::string &key) const
{
    try{
        return _stringVectors.at(key);
    } catch (const std::out_of_range &e){
        _logger.log(DEBUG, "Block::getStringVector: " + key + " not found");
     }
return std::vector<std::string>();
}

const std::string Block::getString(const std::string &key) const
{
    try{
        return _strings.at(key);
    } catch (const std::out_of_range &e){
        _logger.log(DEBUG, "Block::getString: " + key + " not found");
    }
    return "";
}

int Block::getInt(const std::string &key) const
{
    try {
        return _ints.at(key);
    } catch (const std::out_of_range &e){
        _logger.log(DEBUG, "Block::getInt: " + key + " not found");
    }
    return -1;
}

size_t Block::getSize_t(const std::string &key) const
{
    try{
        return _size_ts.at(key);
    } catch (const std::out_of_range &e){
        _logger.log(DEBUG, "Block::getSize_t: " + key + " not found");
    
    }
    return -1;
}

bool Block::getBool(const std::string &key) const
{
    try{
        return _bools.at(key);
    } catch (const std::out_of_range &e){
        _logger.log(DEBUG, "Block::getBool: " + key + " not found");
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

void	Block::print(size_t depth) const {
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
