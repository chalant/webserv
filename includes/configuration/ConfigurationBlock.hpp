#ifndef CONFIGURATIONBLOCK_HPP
#define CONFIGURATIONBLOCK_HPP

#include "../../includes/configuration/BlockList.hpp"
#include "../../includes/configuration/IConfiguration.hpp"
#include "../configuration/Defaults.hpp"
#include "../logger/ILogger.hpp"
#include <map>
#include <string>

class ConfigurationBlock : public IConfiguration, public BlockList
{
private:
    ILogger &_logger;
    const std::string _name;
    std::map<std::string, BlockList> _blocks;
    std::map<std::string, std::vector<std::string> *> _directives;
	std::vector<std::string>	_parameters;
    Defaults &m_defaults;
	bool	m_is_regex;

public:
    ConfigurationBlock(ILogger &logger, const std::string name,
                       Defaults &defaults);
    ConfigurationBlock(const ConfigurationBlock &parent, const std::string name,
                       Defaults &defaults);
    ~ConfigurationBlock();
    virtual const BlockList &getBlocks(const std::string &key) const;
    virtual const std::vector<std::string> &
    getStringVector(const std::string &key) const;
    virtual const std::string &getString(const std::string &parameter,
                                         size_t index) const;
    virtual int getInt(const std::string &parameter, size_t index) const;
    virtual size_t getSize_t(const std::string &parameter, size_t index) const;
    virtual bool getBool(const std::string &parameter, size_t index) const;
    virtual void addBlock(const std::string &name, IConfiguration *block);
    std::vector<std::string> &addDirective(const std::string &name);
    virtual bool isRegex(void) const;
	virtual void isRegex(bool value);
    virtual const std::string &getName() const;
	virtual std::vector<std::string> &getParameters(void);
    void print(size_t depth) const;
};

#endif // CONFIGURATIONBLOCK_HPP
