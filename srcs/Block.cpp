#include "../includes/Block.hpp"

const std::vector<IBlock> Block::getBlocks(const std::string &key)
{
    return _blocks[key];
}

const std::string Block::getString(const std::string &parameter) const
{
    return _data.at(parameter);
}

int Block::getInt(const std::string &parameter) const
{
    return std::stoi(_data.at(parameter));
}

bool Block::getBool(const std::string &parameter) const
{
    return _data.at(parameter) == "true";
}

// Path: srcs/Block.cpp
