#ifndef CONFIGURATIONBLOCK_HPP
#define CONFIGURATIONBLOCK_HPP

#include "../../includes/configuration/IConfiguration.hpp"
#include "../../includes/configuration/BlockList.hpp"
#include "../configuration/Defaults.hpp"
#include <map>
#include <string>
#include "../logger/ILogger.hpp"

class ConfigurationBlock : public IConfiguration, public BlockList
{
	private:
		ILogger&	_logger;
		const std::string	_name;
		std::map<std::string, BlockList>	_blocks;
		std::map<std::string, std::vector<std::string>* >	_directives;
		Defaults&	m_defaults;
	public:
		ConfigurationBlock(ILogger &logger, const std::string name, Defaults& defaults);
		ConfigurationBlock(const ConfigurationBlock &parent, const std::string name, Defaults& defaults);
		~ConfigurationBlock();
		virtual const BlockList&	getBlocks(const std::string &key) const;
		virtual const std::vector<std::string>&	getStringVector(const std::string &key) const;
		virtual const std::string&	getString(const std::string &parameter, size_t index) const;
		virtual int		getInt(const std::string &parameter, size_t index) const;
		virtual size_t	getSize_t(const std::string &parameter, size_t index) const;
		virtual bool	getBool(const std::string &parameter, size_t index) const;
		virtual	void	addBlock(const std::string& name, IConfiguration *block);
		std::vector<std::string>&	addDirective(const std::string& name);
		virtual bool	isRegex(void) const;
		virtual const std::string&	getName() const;
		void			print(size_t depth) const;
};

#endif // CONFIGURATIONBLOCK_HPP
