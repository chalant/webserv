#include "configuration/Block.hpp"

Block::Block(ILogger &logger) : 
    _logger(logger)
{
}

const std::vector<IBlock *> Block::getBlocks(const std::string &key) const
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

// Path: srcs/Block.cpp
