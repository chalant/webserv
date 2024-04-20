#ifndef IBLOCK_HPP
#define IBLOCK_HPP

#include <string>
#include <vector>

class IBlock
{
public:
    virtual ~IBlock() {};

    virtual const std::vector<IBlock *> getBlocks(const std::string &) = 0;
    virtual const std::vector<std::string> getStringVector(const std::string &) const = 0;
    virtual const std::string getString(const std::string &parameter) const = 0;
    virtual int getInt(const std::string &parameter) const = 0;
    virtual size_t getSize_t(const std::string &) const = 0;
    virtual bool getBool(const std::string &parameter) const = 0;
};

#endif // IBLOCK_HPP
// Path: includes/IBlock.hpp
