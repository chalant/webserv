#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "IBlock.hpp"
#include "ILogger.hpp"
#include <stdexcept>
#include <map>

class Block : public IBlock
{
private:
    std::map<std::string, std::vector<IBlock *> > _blocks;
    std::map<std::string, std::vector<std::string> > _stringVectors;
    std::map<std::string, std::string> _strings;
    std::map<std::string, int> _ints;
    std::map<std::string, size_t> _size_ts;
    std::map<std::string, bool> _bools;
    ILogger &_logger;

public:
    Block(ILogger &logger);

    virtual const std::vector<IBlock *> getBlocks(const std::string &key) const;
    virtual const std::vector<std::string> getStringVector(const std::string &key) const;
    virtual const std::string getString(const std::string &parameter) const;
    virtual int getInt(const std::string &parameter) const;
    virtual size_t getSize_t(const std::string &parameter) const;
    virtual bool getBool(const std::string &parameter) const;
};

#endif // BLOCK_HPP
// Path: includes/Block.hpp
