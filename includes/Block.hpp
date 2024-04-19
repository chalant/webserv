#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "IBlock.hpp"
#include <map>

class Block : public IBlock
{
private:
    std::map<std::string, std::vector<IBlock> > _blocks;
    std::map<std::string, std::string> _data;

public:
    virtual const std::vector<IBlock> getBlocks(const std::string &key);
	virtual const std::string getString(const std::string &parameter) const;
	virtual int getInt(const std::string &parameter) const;
	virtual bool getBool(const std::string &parameter) const;};

#endif // BLOCK_HPP
// Path: includes/Block.hpp
